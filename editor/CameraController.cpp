#include "CameraController.hpp"
#include <Input.hpp>
#include <GLFW/glfw3.h>
#include <Application.hpp>
#include <MouseScrolledEvent .hpp>

cass::Vector2<float> CameraController::ScreenToWorld(const cass::Vector2<float>& screen, float width, float height)
{
	float x_ndc = (2.0f * screen.x) / width - 1.0f;
	float y_ndc = 1.0f - (2.0f * screen.y) / height;

	cass::Vector4<float> clipPos = { x_ndc, y_ndc, 0.0f, 1.0f };

	cass::Matrix4<float> viewProj = m_Camera.GetViewProjection();
	cass::Vector4<float> world = viewProj.inverse() * clipPos;

	return { world.x, world.y };
}

void CameraController::HandleInputUpdate(float deltaTime, float width, float height)
{
	cass::Vector2<float> screen = Input::GetMousePosition();
	cass::Vector3 camPos = m_Camera.GetPosition();
	cass::Vector2<float> mouse = Input::GetMousePosition();

	if (Input::IsKeyPressed(GLFW_KEY_KP_ADD)) {
		float zoom = m_Camera.GetZoom();
		if (zoom >= 0.1f) {
			zoom -= 0.01f;
		}
		m_Camera.SetZoom(zoom);
	}


	if (Input::IsKeyPressed(GLFW_KEY_KP_SUBTRACT)) {
		float zoom = m_Camera.GetZoom();

		if (zoom <= 1.5f) {
			zoom += 0.01f;
		}
		m_Camera.SetZoom(zoom);
	}

	if (Input::IsMousePressed(GLFW_MOUSE_BUTTON_MIDDLE))
	{
		if (!m_Dragging)
		{
			m_Dragging = true;
			m_LastWorldMouse = ScreenToWorld(mouse, width, height);
		}

		// 🔥 calcular con cámara actual
		cass::Vector2<float> currentWorld = ScreenToWorld(mouse, width, height);

		cass::Vector2<float> delta = m_LastWorldMouse - currentWorld;

		camPos.x += delta.x;
		camPos.y += delta.y;

		m_Camera.SetPosition(camPos); // 👈 aplicar inmediatamente

		// 🔥 recalcular con la cámara nueva
		m_LastWorldMouse = ScreenToWorld(mouse, width, height);
	}
	else
	{
		m_Dragging = false;

		direction = { 0,0 };

		if (Input::IsKeyPressed(GLFW_KEY_UP)) direction.y += 1;
		if (Input::IsKeyPressed(GLFW_KEY_DOWN)) direction.y -= 1;
		if (Input::IsKeyPressed(GLFW_KEY_LEFT)) direction.x -= 1;
		if (Input::IsKeyPressed(GLFW_KEY_RIGHT)) direction.x += 1;

		velocity = cass::Vector3<float>(direction, 0.0f).SafeNormalize() * 400;

		camPos += velocity * deltaTime;
	}

	worldMouse = ScreenToWorld(mouse, width, height);

	m_Camera.SetPosition(camPos);
}

void CameraController::HandleInputEvent(Event& e)
{
	if (e.GetType() == EventType::MouseScrolled)
	{
		auto& scroll = (MouseScrolledEvent&)e;

		float zoom = m_Camera.GetZoom();

		zoom -= scroll.GetYOffset() * 0.1f;

		if (zoom < 0.1f) zoom = 0.1f;
		if (zoom > 1.5f) zoom = 1.5f;

		m_Camera.SetZoom(zoom);
	}
}
