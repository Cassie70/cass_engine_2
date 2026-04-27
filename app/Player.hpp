#pragma once

#include <Renderer2D.hpp>
#include <Input.hpp>
#include <SpriteSheet.hpp>
#include <SpriteAnimation.hpp>

#include "Entity.hpp"
#include "TileManager.hpp"

enum class Direction
{
	UP, DOWN, LEFT, RIGHT
};

class Player : public Entity {

private:

	cass::Texture2D texture;
	cass::SpriteSheet playerSS;
	cass::SpriteAnimation* currentAnim;
	cass::SpriteAnimation frontIdle;
	cass::SpriteAnimation rightIdle;
	cass::SpriteAnimation upIdle;
	cass::SpriteAnimation walkRight;
	cass::SpriteAnimation walkUp;
	cass::SpriteAnimation walkDown;
	Direction orientation;
	bool walkLeft;
	float colliderSize;
	float halfCollider;
public:
	Player(): texture("assets/diablito.png",{}) {

		playerSS = cass::SpriteSheetParams{
			.textureWidth = (int)texture.GetWidth(),
			.textureHeight = (int)texture.GetHeight(),
			.spriteWidth = 16,
			.spriteHeight = 16,
			.rows = 3,
			.cols = 4,
			.spacing = { 1, 1 },
		};

		frontIdle = cass::SpriteAnimationParams{
			.frames = {{0,2}, {1,2}},
			.frameTime = 1.0f / 6.0f
		};

		rightIdle = cass::SpriteAnimationParams{
			.frames = {{0,1}, {1,1}},
			.frameTime = 1.0f / 6.0f
		};

		upIdle = cass::SpriteAnimationParams{
			.frames = {{0,0},{1,0}},
			.frameTime = 1.0f / 6.0f
		};

		walkRight = cass::SpriteAnimationParams{
			.frames = {{2,1}, {3,1}},
			.frameTime = 1.0f/6.0f
		};
		
		walkDown = cass::SpriteAnimationParams{
			.frames = {{2,2}, {3,2}},
			.frameTime = 1.0f / 6.0f
		};

		walkUp = cass::SpriteAnimationParams{
			.frames = {{2,0}, {3,0}},
			.frameTime = 1.0f / 6.0f
		};

		currentAnim = &frontIdle;
		setDefaultValues();
	}

	void setDefaultValues() {
		direction = { 0, 0 };
		position = { 3, 3 };
		speed = 6;
		colliderSize = 0.5f;
		halfCollider = colliderSize / 2.0f;
		orientation = Direction::DOWN;
	}

	void handleInput() {

		direction = { 0, 0 };
		

		if (cass::Input::IsKeyPressed(cass::Key::Up)) {
			direction.y += 1;
			currentAnim = &walkUp;
			orientation = Direction::UP;
		}
		if (cass::Input::IsKeyPressed(cass::Key::Down)) {
			direction.y -= 1;
			currentAnim = &walkDown;
			orientation = Direction::DOWN;
		}
		if (cass::Input::IsKeyPressed(cass::Key::Left)) {
			direction.x -= 1;
			currentAnim = &walkRight;
			orientation = Direction::LEFT;
			walkLeft = true;
			
		}
		if (cass::Input::IsKeyPressed(cass::Key::Right)) {
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

		velocity = cass::Vector2<float>(direction).SafeNormalize() * speed;
	}

	void update(float deltaTime, TileManager &tileManager) {
		const float EPS = 0.001f;
		cass::Vector2<float> nextPosition = position;
		nextPosition.x += velocity.x * deltaTime;
		{
			int left = (int)floor(nextPosition.x - halfCollider);
			int right = (int)floor(nextPosition.x + halfCollider);
			int bottom = (int)floor(position.y - halfCollider);
			int top = (int)floor(position.y + halfCollider);
			
			for (int y = bottom; y <= top; y++) {
				if (velocity.x > 0) {
					if (tileManager.IsSolid(right, y)) {
						nextPosition.x = right - halfCollider - EPS;
						break;
					}
				}
				else if (velocity.x < 0) {
					if (tileManager.IsSolid(left, y)) {
						nextPosition.x = left + 1 + halfCollider + EPS;
						break;
					}
				}
			}
		}

		position.x = nextPosition.x;

		nextPosition.y += velocity.y * deltaTime;

		{
			int left = (int)floor(position.x - halfCollider);
			int right = (int)floor(position.x + halfCollider);
			int bottom = (int)floor(nextPosition.y - halfCollider);
			int top = (int)floor(nextPosition.y + halfCollider);

			for (int x = left; x <= right; x++) {
				if (velocity.y > 0) {
					if (tileManager.IsSolid(x, top)) {
						nextPosition.y = top - halfCollider - EPS;
						break;
					}
				}
				else if (velocity.y < 0) {
					if (tileManager.IsSolid(x, bottom)) {
						nextPosition.y = bottom + 1 + halfCollider + EPS;
						break;
					}
				}
			}
		}

		position.y = nextPosition.y;

		currentAnim->Update(deltaTime);
	}

	void draw() {
		cass::Renderer2D::DrawSprite({
			.position = position,
			.size = {1,1},
			.texture = &texture,
			.uv = currentAnim->GetUV(playerSS),
			.origin = {0.5,0.5},
			.flipX = walkLeft,
		});
		/*
		Renderer2D::DrawQuad({
			.transform = cass::Matrix4<float>().translate(position).scale(colliderSize),
			.origin = {0.5,0.5},
		});
		*/
	}
};