#include <Application.hpp>
#include <Renderer2D.hpp>
#include <FontManager.hpp>
#include <KeyEvent.hpp>
#include <GLFW/glfw3.h>
#include <Input.hpp>
#include <WindowResizeEvent .hpp>

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

		if (Input::IsKeyPressed(GLFW_KEY_KP_ADD)) {
			float zoom = m_Camera.GetZoom();
			if (zoom >= 0.1f) {
				zoom -= 0.01f;
			}
			m_Camera.SetZoom(zoom);
		}


		if (Input::IsKeyPressed(GLFW_KEY_KP_SUBTRACT)) {
			float zoom = m_Camera.GetZoom();

			if (zoom <= 1.5f) {
				zoom += 0.01f;
			}
			m_Camera.SetZoom(zoom);
		}

		velocity = cass::Vector3<float>(direction, 0.0f).SafeNormalize() * 750;

		newCamPosition += velocity * deltaTime;

		m_Camera.SetPosition(newCamPosition);


		cass::Vector2<float> screen = Input::GetMousePosition();

		float x_ndc = (2.0f * screen.x) / Application::m_Window->GetWidth() - 1.0f;
		float y_ndc = 1.0f - (2.0f * screen.y) / Application::m_Window->GetHeight();

		cass::Vector4<float> clipPos = {
			x_ndc,
			y_ndc,
			0.0f,
			1.0f
		};

		cass::Matrix4<float> viewProj = m_Camera.GetViewProjection();
		cass::Vector4<float> world = viewProj.inverse() * clipPos;

		cass::Vector2<float> worldMouse = {
			world.x,
			world.y
		};

		Renderer2D::BeginScene(m_Camera);
		DrawGrid(
			props.Width,
			props.Height,
			16.0f,        // tile size
			0xFF555555,   // color visible
			1.0f          // grosor
		);
		Renderer2D::DrawQuad({
			.transform = cass::Matrix4<float>().translate({0,0}).scale(16),
			.origin = {0.5,0.5}
			});
		Renderer2D::EndScene();

		Renderer2D::BeginScene(ui_Camera);
		Renderer2D::DrawText({
			.font = arial24,
			.text = "Screen: " + Input::GetMousePosition().toString() +
					" | World: " + worldMouse.toString(),
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

		if (e.GetType() == EventType::WindowResize)
		{
			auto& resize = (WindowResizeEvent&)e;

			m_Camera.SetProjection(
				-resize.Width * 0.5f,
				resize.Width * 0.5f,
				-resize.Height * 0.5f,
				resize.Height * 0.5f
			);

			ui_Camera.SetProjection(0, resize.Width, resize.Height, 0);
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
		.Maximized = true
	};

	Editor app(windowProps);

	app.Run();

	return 0;
}