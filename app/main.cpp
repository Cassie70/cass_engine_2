#include "Application.hpp"
#include "Renderer.hpp"
#include <Renderer2D.hpp>
#include "../engine/dependencies/glfw/include/GLFW/glfw3.h"
#include <filesystem>
#include <iostream>
#include <Input.hpp>
#include "SpriteAnimation.hpp"
#include "Player.hpp"
#include "TileManager.hpp"

const int originalTileSize = 16;
const int scale = 4;
const int tileSize = originalTileSize * scale;
const int screenCols = 16;
const int screenRows = 12;

using v3 = cass::Vector3<float>;


class SandBox : public Application {
private:
	OrthographicCamera m_Camera;
	WindowProperties props;

	float m_TimeAccumulator = 0.0f;
	int m_FrameCount = 0;
	float accumulator = 0;
	float frameDt = 0;

	Player player;
	TileManager tileManager;

public:
	SandBox(const WindowProperties& props) : 
		Application(props), 
		m_Camera(
			-(props.Width / static_cast<float>(tileSize)) * 0.5f,
			(props.Width / static_cast<float>(tileSize)) * 0.5f,
			-(props.Height / static_cast<float>(tileSize)) * 0.5f,
			(props.Height / static_cast<float>(tileSize)) * 0.5f
		),
		props(props),
		tileManager("assets/atlas.png", "assets/level1.txt")
	{
		m_Camera.SetPosition({ player.position,0.0f });
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

		v3 newCameraPosition = m_Camera.GetPosition() + (v3(player.position, 0.0f) - m_Camera.GetPosition()) * 0.1f;

		m_Camera.SetPosition(newCameraPosition);

		Renderer2D::BeginScene(m_Camera);
		tileManager.draw(m_Camera.GetPosition(), screenCols, screenRows);
		player.draw();
		Renderer2D::EndScene();

		//showInfo(deltaTime);
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