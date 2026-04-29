#include "CameraController.hpp"
#include <Input.hpp>
#include <Application.hpp>
#include <MouseScrolledEvent.hpp>
#include <WindowResizeEvent.hpp>

using namespace cass;

Vector2<float> CameraController::ScreenToWorld(const Vector2<float>& screen, float width, float height)
{
	float x_ndc = (2.0f * screen.x) / width - 1.0f;
	float y_ndc = 1.0f - (2.0f * screen.y) / height;

	Vector4<float> clipPos = { x_ndc, y_ndc, 0.0f, 1.0f };

	Matrix4<float> viewProj = m_Camera.GetViewProjection();
	Vector4<float> world = viewProj.inverse() * clipPos;

	return { world.x, world.y };
}

void CameraController::HandleInputUpdate(float deltaTime, float width, float height)
{
	Vector2<float> screen = Input::GetMousePosition();
	Vector3<float> camPos = m_Camera.GetPosition();
	Vector2<float> mouse = Input::GetMousePosition();

	if (Input::IsKeyPressed(Key::KPAdd)) {
		float zoom = m_Camera.GetZoom();
		if (zoom >= 0.1f) {
			zoom -= 0.01f;
		}
		m_Camera.SetZoom(zoom);
	}


	if (Input::IsKeyPressed(Key::KPSubtract)) {
		float zoom = m_Camera.GetZoom();

		if (zoom <= 1.5f) {
			zoom += 0.01f;
		}
		m_Camera.SetZoom(zoom);
	}

	if (Input::IsMousePressed(Mouse::Middle))
	{
		if (!m_Dragging)
		{
			m_Dragging = true;
			m_LastWorldMouse = ScreenToWorld(mouse, width, height);
		}

		// 🔥 calcular con cámara actual
		Vector2<float> currentWorld = ScreenToWorld(mouse, width, height);

		Vector2<float> delta = m_LastWorldMouse - currentWorld;

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

		if (Input::IsKeyPressed(Key::Up)) direction.y += 1;
		if (Input::IsKeyPressed(Key::Down)) direction.y -= 1;
		if (Input::IsKeyPressed(Key::Left)) direction.x -= 1;
		if (Input::IsKeyPressed(Key::Right)) direction.x += 1;

		velocity = Vector3<float>(direction, 0.0f).SafeNormalize() * 400;

		camPos += velocity * deltaTime;
	}

	worldMouse = ScreenToWorld(mouse, width, height);

	m_Camera.SetPosition(camPos);
}

void CameraController::HandleInputEvent(Event& e)
{
	if (e.GetType() == EventType::WindowResize) {
		auto& resize = (WindowResizeEvent&)e;

		m_Camera.SetProjection(
			-resize.Width * 0.5f,
			resize.Width * 0.5f,
			-resize.Height * 0.5f,
			resize.Height * 0.5f
		);

		ui_Camera.SetProjection(0, resize.Width, 0, resize.Height);
	}

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
