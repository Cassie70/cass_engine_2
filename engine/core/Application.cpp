#include "Application.hpp"
#include "Renderer.hpp"
#include "Time.hpp"
#include <Renderer2D.hpp>

Application::Application(const WindowProperties& props)
{
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
    while (true)
    {
        float deltaTime = Time::GetDeltaTime();

        Renderer::BeginFrame();

        OnUpdate(deltaTime);

        Renderer::EndFrame();
        m_Window->Update();
    }
}

