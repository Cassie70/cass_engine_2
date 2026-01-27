#pragma once
#include "WindowProperties.hpp"
#include <Window.hpp>

class Application {
public:
	Application(const WindowProperties& props);
	virtual ~Application();
	void Run();


protected:
	virtual void OnUpdate(float deltaTime){}
    Window* m_Window;
};