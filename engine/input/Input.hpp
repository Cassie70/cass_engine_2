#pragma once
#include <cass_linear.hpp>

class Input {
public:
    static bool IsKeyPressed(int keycode);
    static bool IsMousePressed(int buttoncode);
    static cass::Vector2<float> GetMousePosition();
};
