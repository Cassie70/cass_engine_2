#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include "WindowProperties.hpp"
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

    if (!glfwInit())
    {
        std::cerr << "Failed to init GLFW\n";
        return;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    m_Window = glfwCreateWindow(
        m_Width,
        m_Height,
        props.Title.c_str(),
        nullptr,
        nullptr
    );

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
