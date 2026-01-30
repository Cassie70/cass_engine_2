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
    std::string GetTitle() const { return m_Title; }

    void SetVSync(bool enabled);
    void SetTitle(const std::string& title);
    bool IsVSync() const { return m_VSync; }

    void* GetNativeWindow() const { return m_Window; } // GLFWwindow*

    bool ShouldClose() const;

private:
    void Init(const WindowProperties& props);
    void Shutdown();

private:
    void* m_Window; // GLFWwindow*
    unsigned int m_Width, m_Height;
    std::string m_Title;
    bool m_VSync;
};
