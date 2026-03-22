#pragma once

#include "Entity.hpp"
#include "Renderer2D.hpp"
#include "Input.hpp"

enum class Direction
{
	UP, DOWN, LEFT, RIGHT
};

class Player : public Entity {

	Texture2D texture;
	SpriteSheet playerSS;
	SpriteAnimation* currentAnim;
	SpriteAnimation frontIdle;
	SpriteAnimation rightIdle;
	SpriteAnimation upIdle;
	SpriteAnimation walkRight;
	SpriteAnimation walkUp;
	SpriteAnimation walkDown;

	cass::Vector2<int> direction;
	Direction orientation;
	bool walkLeft;

	
public:
	Player(): texture("assets/diablito.png",{}) {

		playerSS = SpriteSheetParams{
			.textureWidth = (int)texture.GetWidth(),
			.textureHeight = (int)texture.GetHeight(),
			.spriteWidth = 16,
			.spriteHeight = 16,
			.rows = 3,
			.cols = 4,
			.spacing = { 1, 1 },
		};

		frontIdle = SpriteAnimationParams{
			.frames = {{0,2}, {1,2}},
			.frameTime = 1.0f / 6.0f
		};

		rightIdle = SpriteAnimationParams{
			.frames = {{0,1}, {1,1}},
			.frameTime = 1.0f / 6.0f
		};

		upIdle = SpriteAnimationParams{
			.frames = {{0,0},{1,0}},
			.frameTime = 1.0f / 6.0f
		};

		walkRight = SpriteAnimationParams{
			.frames = {{2,1}, {3,1}},
			.frameTime = 1.0f/6.0f
		};
		
		walkDown = SpriteAnimationParams{
			.frames = {{2,2}, {3,2}},
			.frameTime = 1.0f / 6.0f
		},

		walkUp = SpriteAnimationParams{
				.frames = {{2,0}, {3,0}},
				.frameTime = 1.0f / 6.0f
		},


		currentAnim = &frontIdle;
		setDefaultValues();
	}

	void setDefaultValues() {
		direction = { 0,0 };
		position = { 8,6 };
		speed = 6;
		orientation = Direction::DOWN;
	}

	void handleInput(float deltaTime) {

		direction = { 0, 0 };
		

		if (Input::IsKeyPressed(GLFW_KEY_UP)) {
			direction.y += 1;
			currentAnim = &walkUp;
			orientation = Direction::UP;
		}
		else if (Input::IsKeyPressed(GLFW_KEY_DOWN)) {
			direction.y -= 1;
			currentAnim = &walkDown;
			orientation = Direction::DOWN;
		}
		else if (Input::IsKeyPressed(GLFW_KEY_LEFT)) {
			direction.x -= 1;
			currentAnim = &walkRight;
			orientation = Direction::LEFT;
			walkLeft = true;
			
		}
		else if (Input::IsKeyPressed(GLFW_KEY_RIGHT)) {
			direction.x += 1;
			currentAnim = &walkRight;
			walkLeft = false;
			orientation = Direction::RIGHT;
		}

		if (direction.x == 0 && direction.y == 0) {
			switch (orientation) {
				case Direction::DOWN:currentAnim = &frontIdle;break;
				case Direction::UP:currentAnim = &upIdle;break;
				case Direction::LEFT:currentAnim = &rightIdle;break;
				case Direction::RIGHT:currentAnim = &rightIdle;break;
			}
		}
 
		velocity = cass::Vector2<float>(direction) * speed;
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