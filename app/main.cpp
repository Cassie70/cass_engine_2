#include "Application.hpp"
#include "Renderer.hpp"
#include <Renderer2D.hpp>
#include "../engine/dependencies/glfw/include/GLFW/glfw3.h"
#include <filesystem>
#include <iostream>
#include <Input.hpp>
#include "SpriteAnimation.hpp"

struct AABB {
	float minX, minY;
	float maxX, maxY;
};


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
	float velocity = 100;
	float jumpVelocity = 750;
	const float gravity = -2000.0f;
	bool isGrounded = false;

	float m_TimeAccumulator = 0.0f;
	int m_FrameCount = 0;
	float accumulator = 0;
	float frameDt = 0;

	SpriteAnimation* currentAnim;
	SpriteAnimation walk;
	SpriteAnimation idle;
	SpriteAnimation jump;

	cass::Vector2<float> floorPos;
	cass::Vector2<float> floorSize;
	cass::Vector2<float> floorOrigin;



public:
	SandBox(const WindowProperties& props) : Application(props), m_Camera(
		0.0f, props.Width/3,
		0.0f, props.Height/3), props(props) {

		Renderer::SetClearColor(0xFF1E1E1E);

		marioPosition = { 20, 60 };
		marioVelocity = { 0,0 };
		marioDimensions = { 18,16 };
		marioOrigin = { 0.5,0.5 };
		
		floorPos = { 0,0 };
		floorSize = { 128,32 };
		floorOrigin = { 0,0 };

		walk = {
		.rows = 1,
		.cols = 6,
		.frames = { 1, 2, 3 },
		.frameTime = 1.0f / 12.0f
		};

		idle = {
		.rows = 1,
		.cols = 6,
		.frames = { 0 },
		.frameTime = 1.0f / 12.0f
		};

		jump = {
			.rows = 1,
			.cols = 6,
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

		marioVelocity.x = 0;

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

		// --- ANIMACIÓN (prioridad correcta) ---
		SpriteAnimation* newAnim;
		if (!isGrounded) {
			newAnim = &jump;
		}
		else if (moving) {
			newAnim = &walk;          // caminando
		}
		else {
			newAnim = &idle;          // quieto
		}

		// Cambio limpio de animación
		if (newAnim != currentAnim) {
			currentAnim = newAnim;
		}

		currentAnim->Update(deltaTime);

		// --- LUEGO SIMULACIÓN ---
		accumulator += deltaTime;

		while (accumulator >= fixedDt) {

			// ====== GRAVEDAD ======
			marioVelocity.y += gravity * fixedDt;

			// ====== CAJA DEL SUELO ======
			AABB floorBox = MakeAABB(
				floorPos,
				floorSize,
				floorOrigin
			);

			// =====================
			// MOVIMIENTO X
			// =====================
			marioPosition.x += marioVelocity.x * fixedDt;

			ResolveCollisionX(
				marioPosition,
				marioVelocity,
				marioDimensions,
				marioOrigin,
				floorBox
			);

			// =====================
			// MOVIMIENTO Y
			// =====================
			isGrounded = false;

			marioPosition.y += marioVelocity.y * fixedDt;

			ResolveCollisionY(
				marioPosition,
				marioVelocity,
				marioDimensions,
				marioOrigin,
				floorBox,
				isGrounded
			);

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
			.uv = currentAnim->GetUV(),
			.origin = marioOrigin,
			.flipX = marioLeft
			});

			Renderer2D::DrawSprite({
		.position = floorPos,
		.size = floorSize,
		.texture = &floorBricks,
		.origin = floorOrigin
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

	bool Intersects(const AABB& a, const AABB& b) {
		return a.maxX > b.minX &&
			a.minX < b.maxX &&
			a.maxY > b.minY &&
			a.minY < b.maxY;
	}

	AABB MakeAABB(
		const cass::Vector2<float>& position,
		const cass::Vector2<float>& size,
		const cass::Vector2<float>& origin
	) {
		AABB box;

		box.minX = position.x - size.x * origin.x;
		box.minY = position.y - size.y * origin.y;

		box.maxX = box.minX + size.x;
		box.maxY = box.minY + size.y;

		return box;
	}

	void ResolveCollisionX(
		cass::Vector2<float>& pos,
		cass::Vector2<float>& vel,
		const cass::Vector2<float>& size,
		const cass::Vector2<float>& origin,
		const AABB& other
	) {
		AABB box = MakeAABB(pos, size, origin);

		if (!Intersects(box, other))
			return;

		if (vel.x > 0) {
			// chocando por la derecha
			pos.x = other.minX - size.x * (1.0f - origin.x);
		}
		else if (vel.x < 0) {
			// chocando por la izquierda
			pos.x = other.maxX + size.x * origin.x;
		}

		vel.x = 0;
	}

	void ResolveCollisionY(
		cass::Vector2<float>& pos,
		cass::Vector2<float>& vel,
		const cass::Vector2<float>& size,
		const cass::Vector2<float>& origin,
		const AABB& other,
		bool& grounded
	) {
		AABB box = MakeAABB(pos, size, origin);

		if (!Intersects(box, other))
			return;

		if (vel.y > 0) {
			// golpea techo
			pos.y = other.minY - size.y * (1.0f - origin.y);
			vel.y = 0;
		}
		else if (vel.y < 0) {
			// cae sobre piso
			pos.y = other.maxY + size.y * origin.y;
			vel.y = 0;
			grounded = true;
		}
	}



};

int main() {

	WindowProperties windowProps = {
		.Width = 256*3,
		.Height = 240*3,
		.Title = "Hola cara de bola",
		.VSync = true
	};

	SandBox app(windowProps);

	app.Run();

	return 0;
}