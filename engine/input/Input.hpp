#pragma once
#include <Key.hpp>
#include <Mouse.hpp>
#include <cass_linear.hpp>

namespace cass::engine {

class Input {
public:
  static bool IsKeyPressed(Key key);
  static bool IsMousePressed(Mouse button);
  static linear::Vector2<float> GetMousePosition();
};
} // namespace cass::engine
