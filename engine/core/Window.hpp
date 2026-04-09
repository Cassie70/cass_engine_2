#pragma once
#include <string>
#include <functional>
#include <Event.hpp>

struct WindowProperties {
    unsigned int Width = 1280;
    unsigned int Height = 1280;
    std::string Title = "Application";
    bool VSync = false;

    bool Fullscreen = false;
    bool Resizable = true;
    bool Decorated = true;
    bool Maximized = false;
    int PosX = -1;
    int PosY = -1;
};

class Window
{
public:

    using EventCallbackFn = std::function<void(Event&)>;

    void SetEventCallback(const EventCallbackFn& callback) {
        m_EventCallback = callback;
    }

    Window(const WindowProperties& props);
    ~Window();

    void Update();

    unsigned int GetWidth() const { return m_Width; }
    unsigned int GetHeight() const { return m_Height; }
    std::string GetTitle() const { return m_Title; }
    void ToggleFullscreen();

    void SetVSync(bool enabled);
    void SetTitle(const std::string& title);
    bool IsVSync() const { return m_VSync; }

    void* GetNativeWindow() const { return m_Window; } // GLFWwindow*
    void DispatchInitialResize();
    bool ShouldClose() const;

private:
    void Init(const WindowProperties& props);
    void Shutdown();

private:
    EventCallbackFn m_EventCallback;
    void* m_Window; // GLFWwindow*
    unsigned int m_Width, m_Height;
    int m_WindowWidth, m_WindowHeight;
    std::string m_Title;
    bool m_VSync;
    bool m_Fullscreen = false;
    int m_WindowPosX, m_WindowPosY;
};
