#include "Input.hpp"
#include <GLFW/glfw3.h>
#include "Application.hpp"

bool Input::IsKeyPressed(int keycode) {
    auto& app = Application::Get();
    auto* window = static_cast<GLFWwindow*>(
        app.GetWindow().GetNativeWindow()
        );

    int state = glfwGetKey(window, keycode);
    return state == GLFW_PRESS || state == GLFW_REPEAT;
}