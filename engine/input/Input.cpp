#include "Input.hpp"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "Application.hpp"

namespace cass {

    bool Input::IsKeyPressed(Key key) {
        auto& app = Application::Get();
        auto* window = static_cast<GLFWwindow*>(
            app.GetWindow().GetNativeWindow()
            );

        int state = glfwGetKey(window, static_cast<int>(key));
        return state == GLFW_PRESS || state == GLFW_REPEAT;
    }

    bool Input::IsMousePressed(Mouse button)
    {
        auto& app = Application::Get();
        auto* window = static_cast<GLFWwindow*>(
            app.GetWindow().GetNativeWindow()
            );

        int state = glfwGetMouseButton(window, static_cast<int>(button));
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
}
