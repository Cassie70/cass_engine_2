#include "Application.hpp"
#include "Renderer.hpp"
#include <Renderer2D.hpp>
#include "../engine/dependencies/glfw/include/GLFW/glfw3.h"
#include <filesystem>
#include <iostream>
#include <Input.hpp>
#include "SpriteAnimation.hpp"
#include "Player.hpp"

const int originalTileSize = 16;
const int scale = 3;
const int tileSize = originalTileSize * scale;
const int screenCols = 16;
const int screenRows = 12;

class SandBox : public Application {
private:
	OrthographicCamera m_Camera;
	WindowProperties props;

	float m_TimeAccumulator = 0.0f;
	int m_FrameCount = 0;
	float accumulator = 0;
	float frameDt = 0;

	Player player;


public:
	SandBox(const WindowProperties& props) : 
		Application(props), 
		m_Camera(0.0f, props.Width / tileSize, 0.0f, props.Height / tileSize), 
		props(props) 
	{
		Renderer::SetClearColor(0xFE9494FC);
	}

protected:
	void OnUpdate(float deltaTime) override {

		const float fixedDt = 1.0f / 60.0f;

		accumulator += deltaTime;

		player.handleInput(deltaTime);

		while (accumulator >= fixedDt)
		{


			accumulator -= fixedDt;
		}

		Renderer2D::BeginScene(m_Camera);
		player.draw();
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
				" | Quads: " + std::to_string(Renderer2D::GetStats().QuadCount) +
				" | TexturesSlots: " + std::to_string(Renderer2D::GetStats().TextureCount);


			Application::m_Window->SetTitle(title);

			m_FrameCount = 0;
			m_TimeAccumulator = 0.0f;
		}
	}
};

int main() {

	const int screenWidth = tileSize * screenCols;
	const int screenHeight = tileSize * screenRows;

	WindowProperties windowProps = {
		.Width = screenWidth,
		.Height = screenHeight,
		.Title = "Hola cara de bola",
		.VSync = true
	};

	SandBox app(windowProps);

	app.Run();

	return 0;
}