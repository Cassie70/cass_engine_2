#pragma once
#include "../engine/dependencies/cassLinear/cass_linear.hpp"

class Entity {
	public:
		cass::Vector2<float> position;
		cass::Vector2<float> velocity;
		cass::Vector2<int> direction;
		float speed;
};