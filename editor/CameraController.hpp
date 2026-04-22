#pragma once
#include <cass_linear.hpp>
#include <camera/OrthographicCamera.hpp>
#include <Event.hpp>

class CameraController {
private:
	OrthographicCamera &m_Camera;

	cass::Vector2<int> direction;
	cass::Vector3<float> velocity;

	bool m_Dragging = false;
	cass::Vector2<float> worldMouse;
	cass::Vector2<float> m_LastWorldMouse;
	cass::Vector2<float> m_LastMousePos;

	cass::Vector2<float> ScreenToWorld(const cass::Vector2<float>& screen, float width, float height);

public:
	CameraController(OrthographicCamera& camera): m_Camera(camera) {}

	void HandleInputUpdate(float deltaTime, float width, float height);
	void HandleInputEvent(Event& e);

	cass::Vector2<float> getWorldMouse() const { return worldMouse; };
};