#include "Application.hpp"
#include "Renderer.hpp"
#include "Time.hpp"
#include <Renderer2D.hpp>

Application* Application::s_Instance = nullptr;

Application::Application(const WindowProperties& props)
{
    s_Instance = this;
    m_Window = new Window(props);
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
        float deltaTime = Time::GetDeltaTime();

        Renderer::BeginFrame();

        OnUpdate(deltaTime);

        Renderer::EndFrame();
        m_Window->Update();
    }
}

