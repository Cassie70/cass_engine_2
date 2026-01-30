#pragma once
#include "WindowProperties.hpp"
#include <Window.hpp>

class Application {
private:
	static Application* s_Instance;

public:
	Application(const WindowProperties& props);
	virtual ~Application();
	void Run();

	inline static Application& Get() { return *s_Instance; }
	inline Window& GetWindow() { return *m_Window; }

protected:
	virtual void OnUpdate(float deltaTime){}
    Window* m_Window;
};