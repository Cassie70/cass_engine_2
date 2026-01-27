#pragma once
#include <string>
#include "WindowProperties.hpp"

class Window
{
public:
    Window(const WindowProperties& props);
    ~Window();

    void Update();

    unsigned int GetWidth() const { return m_Width; }
    unsigned int GetHeight() const { return m_Height; }

    void SetVSync(bool enabled);
    bool IsVSync() const { return m_VSync; }

    void* GetNativeWindow() const { return m_Window; } // GLFWwindow*

private:
    void Init(const WindowProperties& props);
    void Shutdown();

private:
    void* m_Window; // GLFWwindow*
    unsigned int m_Width, m_Height;
    bool m_VSync;
};
