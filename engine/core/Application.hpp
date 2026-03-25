#pragma once
#include "WindowProperties.hpp"
#include <Window.hpp>

class Application {
private:
	static Application* s_Instance;
	float deltaTime;

public:
	Application(const WindowProperties& props);
	virtual ~Application();
	void Run();

	inline static Application& Get() { return *s_Instance; }
	inline Window& GetWindow() { return *m_Window; }

protected:
	void SetClearColor(const uint32_t argb);
	virtual void OnUpdate(float deltaTime){}
    Window* m_Window;
};