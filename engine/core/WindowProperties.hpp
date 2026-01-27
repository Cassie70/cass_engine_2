#pragma once
#include <string>

struct WindowProperties {
    unsigned int Width = 1280;
    unsigned int Height = 1280;
    std::string Title = "Application";
    bool VSync = false;
};