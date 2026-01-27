#include "OrthographicCamera.hpp"
#include <cass_linear.hpp>

OrthographicCamera::OrthographicCamera(
    float left, float right, float bottom, float top)
{
    m_Projection = cass::Matrix4<float>::ortho(
        left, right, bottom, top, -1.0f, 1.0f
    );
    m_View = cass::Matrix4<float>::identity();
    m_ViewProjection = m_Projection * m_View;
}

void OrthographicCamera::SetPosition(const cass::Vector3<float>& position) {
    m_Position = position;
    RecalculateViewMatrix();
}

void OrthographicCamera::SetRotation(float rotation) {
    m_Rotation = rotation;
    RecalculateViewMatrix();
}

void OrthographicCamera::RecalculateViewMatrix()
{
    m_View = cass::Matrix4<float>::identity();
    m_View.rotateZ(-m_Rotation);
    m_View.translate(-m_Position);

    m_ViewProjection = m_View * m_Projection;
}
