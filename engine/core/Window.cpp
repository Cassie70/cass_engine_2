#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Window.hpp"
#include <KeyEvent.hpp>
#include <WindowResizeEvent .hpp>

Window::Window(const WindowProperties& props)
{
    Init(props);
}

Window::~Window()
{
    Shutdown();
}

void Window::Init(const WindowProperties& props)
{
    m_Width = props.Width;
    m_Height = props.Height;
    m_VSync = props.VSync;
    m_Title = props.Title;

    if (!glfwInit())
    {
        std::cerr << "Failed to init GLFW\n";
        return;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_RESIZABLE, props.Resizable ? GLFW_TRUE : GLFW_FALSE);
    glfwWindowHint(GLFW_DECORATED, props.Decorated ? GLFW_TRUE : GLFW_FALSE);
    glfwWindowHint(GLFW_MAXIMIZED, props.Maximized ? GLFW_TRUE : GLFW_FALSE);

    GLFWmonitor* monitor = nullptr;

    if (props.Fullscreen)
    {
        monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);

        m_Width = mode->width;
        m_Height = mode->height;
    }

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    m_Window = glfwCreateWindow(
        m_Width,
        m_Height,
        props.Title.c_str(),
        monitor,
        nullptr
    );

    if (!props.Fullscreen && props.PosX >= 0 && props.PosY >= 0)
    {
        glfwSetWindowPos((GLFWwindow*)m_Window, props.PosX, props.PosY);
    }

    if (!m_Window)
    {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent((GLFWwindow*)m_Window);
    glfwSetWindowUserPointer((GLFWwindow*)m_Window, this);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to init GLAD\n";
        return;
    }

    SetVSync(m_VSync);

    glViewport(0, 0, m_Width, m_Height);

    glfwSetKeyCallback((GLFWwindow*)m_Window,
        [](GLFWwindow* window, int key, int scancode, int action, int mods) {

            Window* win = (Window*)glfwGetWindowUserPointer(window);

            if (action == GLFW_PRESS) {
                KeyPressedEvent e(key);
                win->m_EventCallback(e);
            }
            else if (action == GLFW_RELEASE) {
                KeyReleasedEvent e(key);
                win->m_EventCallback(e);
            }
        });

    glfwSetFramebufferSizeCallback((GLFWwindow*)m_Window,
        [](GLFWwindow* window, int width, int height)
        {
            Window* win = (Window*)glfwGetWindowUserPointer(window);

            // Actualizar dimensiones internas
            win->m_Width = width;
            win->m_Height = height;

            glViewport(0, 0, width, height);

            WindowResizeEvent e(width, height);
            win->m_EventCallback(e);
        });

    std::cout << "Renderer: " << glGetString(GL_RENDERER) << "\n";
    std::cout << "OpenGL: " << glGetString(GL_VERSION) << "\n";
}

void Window::Shutdown()
{
    glfwDestroyWindow((GLFWwindow*)m_Window);
    glfwTerminate();
}

void Window::Update()
{
    glfwSwapBuffers((GLFWwindow*)m_Window);
    glfwPollEvents();
}

void Window::ToggleFullscreen()
{
    GLFWwindow* window = (GLFWwindow*)m_Window;

    m_Fullscreen = !m_Fullscreen;

    if (m_Fullscreen)
    {
        // Guardar estado actual
        glfwGetWindowPos(window, &m_WindowPosX, &m_WindowPosY);
        glfwGetWindowSize(window, &m_WindowWidth, &m_WindowHeight);

        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);

        glfwSetWindowMonitor(
            window,
            monitor,
            0, 0,
            mode->width,
            mode->height,
            mode->refreshRate
        );

        m_Width = mode->width;
        m_Height = mode->height;
    }
    else
    {
        glfwSetWindowMonitor(
            window,
            nullptr,
            m_WindowPosX,
            m_WindowPosY,
            m_WindowWidth,
            m_WindowHeight,
            0
        );

        m_Width = m_WindowWidth;
        m_Height = m_WindowHeight;
    }
}

void Window::SetVSync(bool enabled)
{
    glfwSwapInterval(enabled ? 1 : 0);
    m_VSync = enabled;
}

bool Window::ShouldClose() const
{
    return glfwWindowShouldClose((GLFWwindow*)m_Window);
}

void Window::SetTitle(const std::string& title)
{
    m_Title = title;
    glfwSetWindowTitle((GLFWwindow*)m_Window, title.c_str());
}

void Window::DispatchInitialResize()
{
    int width, height;
    glfwGetFramebufferSize((GLFWwindow*)m_Window, &width, &height);
    glViewport(0, 0, width, height);
    WindowResizeEvent e(width, height);
    m_EventCallback(e);
}
