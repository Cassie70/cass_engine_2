#include <Application.hpp>
#include <Renderer2D.hpp>
#include <FontManager.hpp>
#include <KeyEvent.hpp>
#include <GLFW/glfw3.h>
#include <Input.hpp>
#include <WindowResizeEvent .hpp>
#include <MouseScrolledEvent .hpp>

using v3 = cass::Vector3<float>;

class Editor : public Application {
private:
	OrthographicCamera m_Camera;
	cass::Vector2<int> direction;
	cass::Vector3<float> velocity;

	bool m_Dragging = false;
	cass::Vector2<float> worldMouse;
	cass::Vector2<float> m_LastWorldMouse;
	cass::Vector2<float> m_LastMousePos;

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

		cass::Vector2<float> screen = Input::GetMousePosition();

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

		v3 camPos = m_Camera.GetPosition();
		cass::Vector2<float> mouse = Input::GetMousePosition();

		if (Input::IsMousePressed(GLFW_MOUSE_BUTTON_MIDDLE))
		{
			if (!m_Dragging)
			{
				m_Dragging = true;
				m_LastWorldMouse = ScreenToWorld(mouse);
			}

			// 🔥 calcular con cámara actual
			cass::Vector2<float> currentWorld = ScreenToWorld(mouse);

			cass::Vector2<float> delta = m_LastWorldMouse - currentWorld;

			camPos.x += delta.x;
			camPos.y += delta.y;

			m_Camera.SetPosition(camPos); // 👈 aplicar inmediatamente

			// 🔥 recalcular con la cámara nueva
			m_LastWorldMouse = ScreenToWorld(mouse);
		}
		else
		{
			m_Dragging = false;

			direction = { 0,0 };

			if (Input::IsKeyPressed(GLFW_KEY_UP)) direction.y += 1;
			if (Input::IsKeyPressed(GLFW_KEY_DOWN)) direction.y -= 1;
			if (Input::IsKeyPressed(GLFW_KEY_LEFT)) direction.x -= 1;
			if (Input::IsKeyPressed(GLFW_KEY_RIGHT)) direction.x += 1;

			velocity = cass::Vector3<float>(direction, 0.0f).SafeNormalize() * 400;

			camPos += velocity * deltaTime;
		}

	
		// -------- APPLY FINAL --------
		m_Camera.SetPosition(camPos);

		Renderer2D::BeginScene(m_Camera);
		DrawGridInfinite(
			16.0f,
			0xFF555555,
			1.0f
		);
		Renderer2D::DrawQuad({
			.transform = cass::Matrix4<float>().translate({0,0}).scale(16),
			.origin = {0,0}
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

		if (e.GetType() == EventType::MouseScrolled)
		{
			auto& scroll = (MouseScrolledEvent&)e;

			float zoom = m_Camera.GetZoom();

			// scroll.GetYOffset() suele ser +1 (arriba) o -1 (abajo)
			zoom -= scroll.GetYOffset() * 0.1f;

			// Clamp (muy importante)
			if (zoom < 0.1f) zoom = 0.1f;
			if (zoom > 1.5f) zoom = 1.5f;

			m_Camera.SetZoom(zoom);
		}
	}

	void DrawGridInfinite(
		float tileSize,
		uint32_t color,
		float weight
	) {
		cass::Vector3<float> camPos = m_Camera.GetPosition();
		float zoom = m_Camera.GetZoom();

		float viewWidth = Application::m_Window->GetWidth() * zoom;
		float viewHeight = Application::m_Window->GetHeight() * zoom;

		float left = camPos.x - viewWidth * 0.5f;
		float right = camPos.x + viewWidth * 0.5f;
		float bottom = camPos.y - viewHeight * 0.5f;
		float top = camPos.y + viewHeight * 0.5f;

		float startX = floor(left / tileSize) * tileSize;
		float startY = floor(bottom / tileSize) * tileSize;

		// Verticales
		for (float x = startX; x <= right; x += tileSize) {
			Renderer2D::DrawCartesianLine({
				.start = { x, bottom },
				.end = { x, top },
				.argb = color,
				.weight = weight
				});
		}

		// Horizontales
		for (float y = startY; y <= top; y += tileSize) {
			Renderer2D::DrawCartesianLine({
				.start = { left, y },
				.end = { right, y },
				.argb = color,
				.weight = weight
				});
		}
	}

	cass::Vector2<float> ScreenToWorld(const cass::Vector2<float>& screen)
	{
		float x_ndc = (2.0f * screen.x) / Application::m_Window->GetWidth() - 1.0f;
		float y_ndc = 1.0f - (2.0f * screen.y) / Application::m_Window->GetHeight();

		cass::Vector4<float> clipPos = { x_ndc, y_ndc, 0.0f, 1.0f };

		cass::Matrix4<float> viewProj = m_Camera.GetViewProjection();
		cass::Vector4<float> world = viewProj.inverse() * clipPos;

		return { world.x, world.y };
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