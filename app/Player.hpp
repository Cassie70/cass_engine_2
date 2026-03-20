#pragma once

#include "Entity.hpp"
#include "Renderer2D.hpp"
#include "Input.hpp"

class Player : public Entity {

	Texture2D texture;
	SpriteSheet playerSS;
	SpriteAnimation* currentAnim;
	SpriteAnimation walkRight;
	SpriteAnimation frontIdle;
	SpriteAnimation walkUp;
	SpriteAnimation walkDown;

	cass::Vector2<float> direction;
	bool walkLeft;

	
public:
	Player(): texture("assets/diablito.png",{}) {

		playerSS = SpriteSheet(
			67, 50,
			16, 16,
			3, 4,
			{ 1, 1 },
			{ 0, 0 }
		);

		frontIdle = {
			.frames = {{0,2}, {1,2}},
			.frameTime = 1.0f / 6.0f
		};

		walkRight = {
			.frames = {{1,1}, {2,1}},
			.frameTime = 1.0f/6.0f
		};
		
		walkDown = {
			.frames = {{2,2}, {3,2}},
			.frameTime = 1.0f / 6.0f
		},

		walkUp = {
				.frames = {{1,0}, {2,0}},
				.frameTime = 1.0f / 6.0f
		},


		currentAnim = &frontIdle;
		setDefaultValues();
	}

	void setDefaultValues() {
		direction = { 0,0 };
		position = { 8,6 };
		speed = 6;
	}

	void handleInput(float deltaTime) {

		direction = { 0, 0 };
		

		if (Input::IsKeyPressed(GLFW_KEY_UP)) {
			direction.y += 1;
			currentAnim = &walkUp;
		}
		else if (Input::IsKeyPressed(GLFW_KEY_DOWN)) {
			direction.y -= 1;
			currentAnim = &walkDown;
		}
		else if (Input::IsKeyPressed(GLFW_KEY_LEFT)) {
			direction.x -= 1;
			currentAnim = &walkRight;
			walkLeft = true;
			
		}
		else if (Input::IsKeyPressed(GLFW_KEY_RIGHT)) {
			direction.x += 1;
			currentAnim = &walkRight;
			walkLeft = false;
		}

		velocity = direction * speed;
		position += velocity * deltaTime;

		currentAnim->Update(deltaTime);


	}

	void draw() {
		Renderer2D::DrawSprite({
			.position = position,
			.size = {1,1},
			.texture = &texture,
			.uv = currentAnim->GetUV(playerSS),
			.origin = {0.5,0.5},
			.flipX = walkLeft,
		});
	}
};