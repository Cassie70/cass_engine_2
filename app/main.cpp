#include "Application.hpp"
#include "Renderer.hpp"
#include <Renderer2D.hpp>
#include "../engine/dependencies/glfw/include/GLFW/glfw3.h"
#include <filesystem>
#include <iostream>
#include <Input.hpp>
#include "SpriteAnimation.hpp"

class SandBox : public Application {
private:
	OrthographicCamera m_Camera;
	WindowProperties props;

	Texture2D textura = Texture2D("assets/thumb.jpg");
	Texture2D mario = Texture2D("assets/mario.png");
	Texture2D floorBricks = Texture2D("assets/floorbricks.png");

	cass::Vector2<float> marioPosition;
	cass::Vector2<float> marioVelocity;
	cass::Vector2<float> marioDimensions;
	cass::Vector2<float> marioOrigin;
	bool marioLeft = false;
	float velocity = 300;
	float jumpVelocity = 750;
	const float gravity = -2000.0f;
	bool isGrounded = false;

	float m_TimeAccumulator = 0.0f;
	int m_FrameCount = 0;
	float accumulator = 0;
	float frameDt = 0;

	SpriteSheet marioSS = {
	.rows = 1,
	.cols = 6,
	.paddingX = 0.0f,
	.paddingY = 0.0f,
	.offsetY = 0.0f
	};
	SpriteAnimation* currentAnim;
	SpriteAnimation walk;
	SpriteAnimation idle;
	SpriteAnimation jump;

public:
	SandBox(const WindowProperties& props) : Application(props), m_Camera(
		0.0f, props.Width,
		0.0f, props.Height), props(props) {

		Renderer::SetClearColor(0xFF1E1E1E);
		Renderer2D::LoadFont("assets/arial.ttf", 128);

		marioPosition = { 20, 60 };
		marioVelocity = { 0,0 };
		marioDimensions = { 64,64 };
		marioOrigin = { 0.5,0.5 };
	
		walk = {
		.frames = { 1, 2, 3 },
		.frameTime = 1.0f / 12.0f
		};

		idle = {
		.frames = { 0 },
		.frameTime = 1.0f / 12.0f
		};

		jump = {
			.frames = { 5 },
			.frameTime = 1.0f / 12.0f
		};


		currentAnim = &idle;
	}

protected:
	void OnUpdate(float deltaTime) override {

		const float fixedDt = 1.0f / 60.0f;

		marioVelocity.x = 0;

		bool moving = false;


		// Movimiento horizontal
		if (Input::IsKeyPressed(GLFW_KEY_LEFT)) {
			marioVelocity.x = -velocity;
			marioLeft = true;
			moving = true;
		}

		if (Input::IsKeyPressed(GLFW_KEY_RIGHT)) {
			marioVelocity.x = velocity;
			marioLeft = false;
			moving = true;
		}


		if (Input::IsKeyPressed(GLFW_KEY_UP) && isGrounded) {
			marioVelocity.y = jumpVelocity;
			isGrounded = false;
		}

		marioVelocity.y += gravity * deltaTime;

		SpriteAnimation* newAnim;
		if (!isGrounded) {
			newAnim = &jump;
		}
		else if (moving) {
			newAnim = &walk;
		}
		else {
			newAnim = &idle;
		}

		if (newAnim != currentAnim) {
			currentAnim = newAnim;
		}

		currentAnim->Update(deltaTime);

		accumulator += deltaTime;

		while (accumulator >= fixedDt) {
			marioPosition += marioVelocity * fixedDt;


			if (marioPosition.y < marioDimensions.y / 2) { 
				marioPosition.y = marioDimensions.y / 2; 
				marioVelocity.y = 0; isGrounded = true; 
			} 
			if (marioPosition.x < marioDimensions.x / 2) { 
				marioPosition.x = marioDimensions.x / 2;
				marioVelocity.x = 0; 
			}

			accumulator -= fixedDt;
		}


		
		if(marioPosition.x > props.Width / 6)
		m_Camera.SetPosition({
			marioPosition.x - (props.Width / 6),
			0,0
		});
		
		Renderer2D::BeginScene(m_Camera);

		Renderer2D::DrawSprite({
			.position = marioPosition,
			.size = marioDimensions,
			.texture = &mario,
			.uv = currentAnim->GetUV(marioSS),
			.origin = marioOrigin,
			.flipX = marioLeft
			});

		Renderer2D::DrawText({
			.fontKey = "assets/arial.ttf@128",
			.text = "ola cara de bola",
			.position = {400,300},
			.argb = 0xFFFF0000
			});

		Renderer2D::DrawText({
			.fontKey = "assets/arial.ttf@128",
			.text = "ola cara de bola",
			.position = {400,200},
			.scale = {0.5, 0.5},
			.argb = 0xFFFF0000
			});

		Renderer2D::DrawText({
			.fontKey = "assets/arial.ttf@128",
			.text = "ola cara de bola",
			.position = {400,150},
			.scale = {0.25, 0.25},
			.argb = 0xFFFF0000
		});

		Renderer2D::DrawText({
			.fontKey = "assets/arial.ttf@128",
			.text = "ola cara de bola",
			.position = {400,100},
			.scale = {0.1, 0.1},
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
		.Width = 1920,
		.Height = 1080,
		.Title = "Hola cara de bola",
		.VSync = true
	};

	SandBox app(windowProps);

	app.Run();

	return 0;
}