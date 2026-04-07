#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Window.hpp"

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

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to init GLAD\n";
        return;
    }

    SetVSync(m_VSync);

    glViewport(0, 0, m_Width, m_Height);

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