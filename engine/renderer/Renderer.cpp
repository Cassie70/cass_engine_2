#include "Renderer.hpp"
#include <glad/glad.h>

void Renderer::Init()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Renderer::BeginFrame()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::EndFrame()
{
}

void Renderer::SetClearColor(const uint32_t argb) {
	float a = ((argb >> 24) & 0xFF) / 255.0f;
	float r = ((argb >> 16) & 0xFF) / 255.0f;
	float g = ((argb >> 8) & 0xFF) / 255.0f;
	float b = (argb & 0xFF) / 255.0f;
    glClearColor(r, g, b, a);
}
