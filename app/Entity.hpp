#pragma once
#include "../engine/dependencies/cassLinear/cass_linear.hpp"

class Entity {
public:
  cass::linear::Vector2<float> position;
  cass::linear::Vector2<float> velocity;
  cass::linear::Vector2<int> direction;
  float speed = 0;
};