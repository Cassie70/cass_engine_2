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

bool Input::IsMousePressed(int buttoncode)
{
    auto& app = Application::Get();
    auto* window = static_cast<GLFWwindow*>(
        app.GetWindow().GetNativeWindow()
        );

    int state = glfwGetMouseButton(window, buttoncode);
    return state == GLFW_PRESS;
}

cass::Vector2<float> Input::GetMousePosition()
{
    auto& window = Application::Get().GetWindow();
    GLFWwindow* glfwWindow = (GLFWwindow*)window.GetNativeWindow();

    double x, y;
    glfwGetCursorPos(glfwWindow, &x, &y);


    return { (float)x,(float)y };
}
