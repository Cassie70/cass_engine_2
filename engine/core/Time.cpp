#include "Time.hpp"
#include <GLFW/glfw3.h>

float Time::s_LastFrameTime = 0.0f;

float Time::GetTime() {
    return (float)glfwGetTime();
}

float Time::GetDeltaTime() {
    float time = GetTime();
    float delta = time - s_LastFrameTime;
    s_LastFrameTime = time;
    return delta;
}
