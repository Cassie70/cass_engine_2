#pragma once
#include <cass_linear.hpp>
#include <Mouse.hpp>
#include <Key.hpp>

namespace cass {

    class Input {
    public:
        static bool IsKeyPressed(Key key);
        static bool IsMousePressed(Mouse button);
        static cass::Vector2<float> GetMousePosition();
    };
}
