#include "Application.hpp"
#include "Renderer.hpp"
#include <Renderer2D.hpp>
#include "Input.hpp"
#include "../engine/dependencies/glfw/include/GLFW/glfw3.h"

class SandBox : public Application {
private:
	OrthographicCamera m_Camera;

	Texture2D textura = Texture2D("C:\\Users\\Cass\\Downloads\\8JXnLX.gif");
	Texture2D ciruclo = Texture2D("C:\\Users\\Cass\\Desktop\\cass_engine\\app\\yop.jpg");

	float m_TimeAccumulator = 0.0f;
	int m_FrameCount = 0;
	float accumulator = 0;
	float frameDt = 0;

	cass::Vector2<float> playerPosition = { 400.0f, 300.0f };
	cass::Vector2<float> playerSpeed = { 0.0f,0.0f };
	cass::Vector2<float> playerDirection = { 0,0 };
	cass::Vector2<float> playerLastDirection = { 0,0 };
	float speed = 200.0f;

	cass::Vector2<float> ballPos = { 400,600 };
	cass::Vector2<float> ballSpeed = { 0.0f,0.0f };

public:
	SandBox(const WindowProperties& props) : Application(props), m_Camera(
		0.0f, props.Width,
		0.0f, props.Height) {

		Renderer::SetClearColor(0xFF1E1E1E);
	}

protected:
	void OnUpdate(float deltaTime) override {

		const float fixedDt = 1.0f / 60.0f;

		accumulator += deltaTime;

		while (accumulator >= fixedDt) {

			// gravedad (aceleración constante)
			ballSpeed.y -= 20.0f * 98.1f * fixedDt;

			// integración semi-implícita
			ballPos += ballSpeed * fixedDt;

			// colisión con suelo
			if (ballPos.y < 15.0f) {
				ballPos.y = 15.0f;
				ballSpeed.y *= -0.8f; // rebote
			}

			accumulator -= fixedDt;
		}

		Renderer2D::BeginScene(m_Camera);

		Renderer2D::DrawCircle({
			.center = ballPos,
			.radius = 30,
			.argb = 0xFFFF0000
			});

		Renderer2D::EndScene();

		showInfo(deltaTime);
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
				" | Quads: " + std::to_string(Renderer2D::GetStats().QuadCount);


			Application::m_Window->SetTitle(title);

			m_FrameCount = 0;
			m_TimeAccumulator = 0.0f;
		}
	}
};

int main() {

	WindowProperties windowProps = {
		.Width = 800,
		.Height = 600,
		.Title = "Hola cara de bola",
		.VSync = true
	};

	SandBox app(windowProps);

	app.Run();

	return 0;
}