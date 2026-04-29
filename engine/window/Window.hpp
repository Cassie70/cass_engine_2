#pragma once
#include <string>
#include <functional>
#include "Event.hpp"
#include <CursorType.hpp>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <WindowProperties.hpp>

namespace cass {
    class Window
    {
    public:

        std::unordered_map<CursorType, GLFWcursor*> m_Cursors;

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
        void SetCursor(CursorType type);
        void SetCursorVisible(bool visible);

        void* GetNativeWindow() const { return m_Window; } // GLFWwindow*
        void DispatchInitialResize();
        bool ShouldClose() const;

    private:
        void Init(const WindowProperties& props);
        void Shutdown();

    private:
        CursorType m_CurrentCursor = CursorType::Arrow;
        EventCallbackFn m_EventCallback;
        void* m_Window; // GLFWwindow*
        unsigned int m_Width, m_Height;
        int m_WindowWidth, m_WindowHeight;
        std::string m_Title;
        bool m_VSync;
        bool m_Fullscreen = false;
        int m_WindowPosX, m_WindowPosY;
    };
}
