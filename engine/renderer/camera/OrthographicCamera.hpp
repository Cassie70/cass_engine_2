#pragma once
#include "Camera.hpp"

class OrthographicCamera : public Camera {
public:
    OrthographicCamera(float left, float right, float bottom, float top);

    void SetPosition(const cass::Vector3<float>& position);
    void SetRotation(float rotation);

    const cass::Vector3<float>& GetPosition() const { return m_Position; }
    float GetRotation() const { return m_Rotation; }

private:
    void RecalculateViewMatrix();

    cass::Vector3<float> m_Position{ 0.0f, 0.0f, 0.0f };
    float m_Rotation = 0.0f;
};
