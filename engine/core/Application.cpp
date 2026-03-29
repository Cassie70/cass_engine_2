#include "Application.hpp"
#include "Renderer.hpp"
#include "Time.hpp"
#include <Renderer2D.hpp>

Application* Application::s_Instance = nullptr;

Application::Application(const WindowProperties& props)
{
    s_Instance = this;
    m_Window = new Window(props);
    deltaTime = 0;
    Renderer::Init();
    Renderer2D::Init();
}

Application::~Application()
{
    delete m_Window;
}

void Application::Run()
{
    while (!m_Window->ShouldClose())
    {
        deltaTime = Time::GetDeltaTime();

        Renderer::BeginFrame();
        Renderer2D::ResetStats();
        OnUpdate(deltaTime);
        Renderer::EndFrame();
        m_Window->Update();
    }
}

void Application::SetClearColor(const uint32_t argb)
{
    Renderer::SetClearColor(argb);
}

