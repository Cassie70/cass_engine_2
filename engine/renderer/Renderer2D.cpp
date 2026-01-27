#include "Renderer2D.hpp"
#include <glad/glad.h>
#include <array>
#include <vector>

struct QuadVertex {
	cass::Vector3<float> Position;
	uint32_t ColorARGB;
	cass::Vector2<float> TexCoords;
	float TexIndex = 0;
};

struct Renderer2DData {
	static const uint32_t MaxQuads = 1000;
	static const uint32_t MaxVertices = MaxQuads * 4;
	static const uint32_t MaxIndices = MaxQuads * 6;
	static const uint32_t MaxTextureSlots = 16;

	uint32_t VAO = 0, VBO = 0, EBO = 0;
	QuadVertex* VertexBufferBase = nullptr;
	QuadVertex* VertexBufferPtr = nullptr;

	uint32_t Shader = 0;

	int ViewProjectionLocation = -1;

	uint32_t IndexCount = 0;
	std::array<Texture2D*, MaxTextureSlots> TextureSlots;
	uint32_t TextureSlotIndex = 1;

	Renderer2DStats Stats;
};

static Renderer2DData s_Data;

static uint32_t CompileShader(uint32_t type, const char* source) {
	uint32_t id = glCreateShader(type);
	glShaderSource(id, 1, &source, nullptr);
	glCompileShader(id);

	int success;
	glGetShaderiv(id, GL_COMPILE_STATUS, &success);
	if (!success) {
		char info[1024];
		glGetShaderInfoLog(id, 1024, nullptr, info);
		glDeleteShader(id);
		return 0;
	}

	return id;
}

static uint32_t CreateShader() {
	const char* vertexSrc = R"(
        #version 450 core

        layout(location = 0) in vec3 a_Position;
        layout(location = 1) in uint a_Color;
        layout(location = 2) in vec2 a_TexCoord;
        layout(location = 3) in float a_TexIndex;

        uniform mat4 u_ViewProjection;

        out vec4 v_Color;
        out vec2 v_TexCoord;
        out float v_TexIndex;

        vec4 UnpackARGB(uint c) {
            float a = float((c >> 24) & 0xFF) / 255.0;
            float r = float((c >> 16) & 0xFF) / 255.0;
            float g = float((c >> 8)  & 0xFF) / 255.0;
            float b = float((c)       & 0xFF) / 255.0;
            return vec4(r, g, b, a);
        }

        void main() {
            v_Color = UnpackARGB(a_Color);
            v_TexCoord = a_TexCoord;
            v_TexIndex = a_TexIndex;
            gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
        }
    )";

	const char* fragmentSrc = R"(
        #version 450 core
        in vec4 v_Color;
        in vec2 v_TexCoord;
        in float v_TexIndex;
        out vec4 FragColor;
        uniform sampler2D u_Textures[16];

        void main() {
            int index = int(v_TexIndex);
            vec4 texColor = texture(u_Textures[index], v_TexCoord);
            FragColor = texColor * v_Color;
        }
    )";

	uint32_t program = glCreateProgram();
	uint32_t vs = CompileShader(GL_VERTEX_SHADER, vertexSrc);
	uint32_t fs = CompileShader(GL_FRAGMENT_SHADER, fragmentSrc);

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}

const Renderer2DStats& Renderer2D::GetStats()
{
	return s_Data.Stats;
}

void Renderer2D::ResetStats()
{
	s_Data.Stats = {};
}

void Renderer2D::Init() {
	s_Data.VertexBufferBase = new QuadVertex[s_Data.MaxVertices];

	glGenVertexArrays(1, &s_Data.VAO);
	glBindVertexArray(s_Data.VAO);

	glGenBuffers(1, &s_Data.VBO);
	glBindBuffer(GL_ARRAY_BUFFER, s_Data.VBO);
	glBufferData(GL_ARRAY_BUFFER, s_Data.MaxVertices * sizeof(QuadVertex), nullptr, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0); // position
	glVertexAttribPointer(
		0, 3, GL_FLOAT, GL_FALSE,
		sizeof(QuadVertex),
		(const void*)offsetof(QuadVertex, Position)
	);

	glEnableVertexAttribArray(1); // color
	glVertexAttribIPointer(
		1, 1, GL_UNSIGNED_INT,
		sizeof(QuadVertex),
		(const void*)offsetof(QuadVertex, ColorARGB)
	);

	glEnableVertexAttribArray(2); // texcoord
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,
		sizeof(QuadVertex), (const void*)offsetof(QuadVertex, TexCoords));

	glEnableVertexAttribArray(3); // tex index
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE,
		sizeof(QuadVertex), (const void*)offsetof(QuadVertex, TexIndex));

	std::vector<uint32_t> indices(Renderer2DData::MaxIndices);

	uint32_t offset = 0;
	for (uint32_t i = 0; i < Renderer2DData::MaxIndices; i += 6) {
		indices[i + 0] = offset + 0;
		indices[i + 1] = offset + 1;
		indices[i + 2] = offset + 2;

		indices[i + 3] = offset + 2;
		indices[i + 4] = offset + 3;
		indices[i + 5] = offset + 0;

		offset += 4;
	}

	glGenBuffers(1, &s_Data.EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_Data.EBO);
	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER,
		indices.size() * sizeof(uint32_t),
		indices.data(),
		GL_STATIC_DRAW
	);

	s_Data.Shader = CreateShader();

	s_Data.ViewProjectionLocation =
		glGetUniformLocation(s_Data.Shader, "u_ViewProjection");

	int samplers[Renderer2DData::MaxTextureSlots]{};

	for (uint32_t i = 0; i < Renderer2DData::MaxTextureSlots; i++)
		samplers[i] = i;

	glUseProgram(s_Data.Shader);
	glUniform1iv(
		glGetUniformLocation(s_Data.Shader, "u_Textures"),
		Renderer2DData::MaxTextureSlots,
		samplers
	);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	uint32_t whiteID;
	glGenTextures(1, &whiteID);
	glBindTexture(GL_TEXTURE_2D, whiteID);

	uint32_t whitePixel = 0xffffffff;

	glTexImage2D(
		GL_TEXTURE_2D, 0, GL_RGBA8,
		1, 1, 0,
		GL_RGBA, GL_UNSIGNED_BYTE,
		&whitePixel
	);

	Texture2D* whiteTexture = new Texture2D(whiteID, 1, 1);

	s_Data.TextureSlots[0] = whiteTexture;

}

void Renderer2D::ShutDown()
{
	delete[] s_Data.VertexBufferBase;

	glDeleteProgram(s_Data.Shader);
	glDeleteBuffers(1, &s_Data.VBO);
	glDeleteBuffers(1, &s_Data.EBO);
	glDeleteVertexArrays(1, &s_Data.VAO);
}

void Renderer2D::BeginScene(const OrthographicCamera& camera) {
	glUseProgram(s_Data.Shader);
	glUniformMatrix4fv(
		s_Data.ViewProjectionLocation,
		1,
		GL_TRUE,
		&camera.GetViewProjection().m[0][0]
	);

	s_Data.IndexCount = 0;
	s_Data.VertexBufferPtr = s_Data.VertexBufferBase;
	s_Data.TextureSlotIndex = 1;

	s_Data.Stats = {};
}
void Renderer2D::EndScene()
{
	if (s_Data.IndexCount == 0)
		return;

	uint32_t size = (uint32_t)(
		(uint8_t*)s_Data.VertexBufferPtr -
		(uint8_t*)s_Data.VertexBufferBase
		);

	glBindBuffer(GL_ARRAY_BUFFER, s_Data.VBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, size, s_Data.VertexBufferBase);

	for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
		s_Data.TextureSlots[i]->Bind(i);

	glUseProgram(s_Data.Shader);
	glBindVertexArray(s_Data.VAO);

	glDrawElements(GL_TRIANGLES, s_Data.IndexCount, GL_UNSIGNED_INT, nullptr);
	s_Data.Stats.DrawCalls++;

	s_Data.IndexCount = 0;
	s_Data.VertexBufferPtr = s_Data.VertexBufferBase;
	s_Data.TextureSlotIndex = 1;
}

void Renderer2D::DrawQuad(const QuadProperties& properties) {
	Texture2D* texture = properties.texture
		? properties.texture
		: s_Data.TextureSlots[0];

	if (s_Data.IndexCount >= s_Data.MaxIndices)
		EndScene(); // flush (batch lleno)

	if (s_Data.TextureSlotIndex >= s_Data.MaxTextureSlots) {
		EndScene();
		s_Data.IndexCount = 0;
		s_Data.VertexBufferPtr = s_Data.VertexBufferBase;
		s_Data.TextureSlotIndex = 1;
	}

	float textureIndex = 0.0f;

	for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++) {
		if (s_Data.TextureSlots[i] == texture) {
			textureIndex = (float)i;
			break;
		}
	}

	if (textureIndex == 0.0f && texture != s_Data.TextureSlots[0]) {

		if (s_Data.TextureSlotIndex >= s_Data.MaxTextureSlots)
			EndScene();

		textureIndex = (float)s_Data.TextureSlotIndex;
		s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
		s_Data.TextureSlotIndex++;
	}

	cass::Vector4<float> uv = properties.uv;

	cass::Vector2<float> texCoords[4] = {
		{ uv.x, uv.y }, // bottom-left
		{ uv.z, uv.y }, // bottom-right
		{ uv.z, uv.t }, // top-right
		{ uv.x, uv.t }  // top-left
	};

	cass::Vector2<float> o = properties.origin;

	cass::Vector4<float> quadPositions[4] = {
		{-o.x,-o.y,0,1},
		{1.0f - o.x,-o.y,0,1},
		{1.0f - o.x,1.0f - o.y,0,1},
		{-o.x,1.0f - o.y,0,1}
	};

	for (int i = 0; i < 4; i++) {

		cass::Vector4<float> worldPos =
			properties.transform * quadPositions[i];

		s_Data.VertexBufferPtr->Position = {
			worldPos.x,
			worldPos.y,
			worldPos.z
		};

		s_Data.VertexBufferPtr->ColorARGB = properties.argb;
		s_Data.VertexBufferPtr->TexCoords = texCoords[i];
		s_Data.VertexBufferPtr->TexIndex = textureIndex;

		s_Data.VertexBufferPtr++;
	}

	s_Data.IndexCount += 6;
	s_Data.Stats.QuadCount++;
}
