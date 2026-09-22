#pragma once
#include <Event.hpp>
#include <camera/OrthographicCamera.hpp>
#include <cass_linear.hpp>

class CameraController {
private:
  cass::engine::OrthographicCamera &m_Camera;
  cass::engine::OrthographicCamera &ui_Camera;

  cass::linear::Vector2<int> direction;
  cass::linear::Vector3<float> velocity;

  bool m_Dragging = false;
  cass::linear::Vector2<float> worldMouse;
  cass::linear::Vector2<float> m_LastWorldMouse;
  cass::linear::Vector2<float> m_LastMousePos;

  cass::linear::Vector2<float> ScreenToWorld(
    const cass::linear::Vector2<float> &screen, float width, float height
  );

public:
  CameraController(
    cass::engine::OrthographicCamera &mainCamera,
    cass::engine::OrthographicCamera &uiCamera
  )
      : m_Camera(mainCamera), ui_Camera(uiCamera) {}

  void HandleInputUpdate(float deltaTime, float width, float height);
  void HandleInputEvent(cass::engine::Event &e);

  cass::linear::Vector2<float> getWorldMouse() const { return worldMouse; };
};