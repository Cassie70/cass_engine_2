#pragma once
#include "Camera.hpp"

class OrthographicCamera : public Camera {
public:
    OrthographicCamera(float left, float right, float bottom, float top);

    void SetProjection(float left, float right, float bottom, float top);
    void SetPosition(const cass::Vector3<float>& position);
    void SetRotation(float rotation);

    void SetZoom(float zoom);
    void ZoomAt(float zoomFactor, const cass::Vector3<float>& focusPoint);

    const cass::Vector3<float>& GetPosition() const { return m_Position; }
    float GetRotation() const { return m_Rotation; }
    float GetZoom() const { return m_Zoom; }

private:
    void RecalculateProjectionMatrix();
    void RecalculateViewMatrix();

    cass::Vector3<float> m_Position{ 0.0f, 0.0f, 0.0f };
    float m_Rotation = 0.0f;
    float m_Zoom = 1.0f;

    float m_Left, m_Right, m_Bottom, m_Top;
};
