#include <Application.hpp>
#include <Renderer2D.hpp>
#include <FontManager.hpp>
#include <KeyEvent.hpp>
#include <GLFW/glfw3.h>
#include <Input.hpp>

using v3 = cass::Vector3<float>;

class Editor : public Application {
private:
	OrthographicCamera m_Camera;
	cass::Vector2<int> direction;
	cass::Vector3<float> velocity;

	OrthographicCamera ui_Camera;
	WindowProperties props;

	float m_TimeAccumulator = 0.0f;
	int m_FrameCount = 0;
	float accumulator = 0;
	float frameDt = 0;

	uint32_t arial24;

public:
	Editor(const WindowProperties& props) :
		Application(props),
		m_Camera(
			-((float)props.Width) * 0.5f,
			((float)props.Width) * 0.5f,
			-((float)props.Height) * 0.5f,
			((float)props.Height) * 0.5f
		),
		ui_Camera(0, props.Width, props.Height, 0),
		props(props)
	{
		Application::SetClearColor(0xFF121212);
		FontManager::Init();

		arial24 = FontManager::Load("assets/arial.ttf", 24);
	}

protected:
	void OnUpdate(float deltaTime) override {

		direction = { 0,0 };

		v3 newCamPosition = m_Camera.GetPosition();

		if (Input::IsKeyPressed(GLFW_KEY_UP)) {
			direction.y += 1;
		}
		if (Input::IsKeyPressed(GLFW_KEY_DOWN)) {
			direction.y -= 1;
		}
		if (Input::IsKeyPressed(GLFW_KEY_LEFT)) {
			direction.x -= 1;
		}
		if (Input::IsKeyPressed(GLFW_KEY_RIGHT)) {
			direction.x += 1;
		}

		velocity = cass::Vector3<float>(direction, 0.0f).SafeNormalize() * 750;

		newCamPosition += velocity * deltaTime;

		m_Camera.SetPosition(newCamPosition);

		Renderer2D::BeginScene(m_Camera);

		DrawGrid(
			props.Width,
			props.Height,
			16.0f,        // tile size
			0xFF555555,   // color visible
			1.0f          // grosor
		);
		Renderer2D::EndScene();

		Renderer2D::BeginScene(ui_Camera);
		Renderer2D::DrawText({
			.font = arial24,
			.text = Input::GetMousePosition().toString(),
			.position = { 50, 50},
			.scale = { 1.0f, -1.0f }
			});
		Renderer2D::EndScene();

		showInfo(deltaTime);
	}

	void OnEvent(Event& e) override
	{
		if (e.GetType() == EventType::KeyPressed)
		{
			KeyPressedEvent& ke = (KeyPressedEvent&)e;

			if (ke.GetKeyCode() == GLFW_KEY_F11)
			{
				Application::Get().GetWindow().ToggleFullscreen();
				std::cout << "F11 presionado\n";
			}
		}
	}

	void DrawGrid(
		float width,
		float height,
		float tileSize,
		uint32_t color = 0xFF444444,
		float weight = 1.0f
	) {
		float halfW = width * 0.5f;
		float halfH = height * 0.5f;

		// Líneas verticales
		for (float x = -halfW; x <= halfW; x += tileSize) {
			Renderer2D::DrawCartesianLine({
				.start = { x, -halfH },
				.end = { x,  halfH },
				.argb = color,
				.weight = weight
				});
		}

		// Líneas horizontales
		for (float y = -halfH; y <= halfH; y += tileSize) {
			Renderer2D::DrawCartesianLine({
				.start = { -halfW, y },
				.end = {  halfW, y },
				.argb = color,
				.weight = weight
				});
		}
	}

	void showInfo(float deltaTime)
	{
		m_TimeAccumulator += deltaTime;
		m_FrameCount++;

		if (m_TimeAccumulator >= 1.0f)
		{
			int fps = m_FrameCount;

			std::string title =
				"Sandbox | FPS: " + std::to_string(fps) +
				" | Draw Calls: " + std::to_string(Renderer2D::GetStats().DrawCalls) +
				" | Quads: " + std::to_string(Renderer2D::GetStats().QuadCount) +
				" | TexturesSlots: " + std::to_string(Renderer2D::GetStats().TextureCount);


			Application::m_Window->SetTitle(title);

			m_FrameCount = 0;
			m_TimeAccumulator = 0.0f;
		}
	}
};

int main() {
	WindowProperties windowProps = {
		.Width = 1280,
		.Height = 720,
		.Title = "Hola cara de bola",
		.VSync = true,
		.Resizable = true
	};

	Editor app(windowProps);

	app.Run();

	return 0;
}