#include "OrthographicCamera.hpp"
#include <cass_linear.hpp>

OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
    : m_Left(left), m_Right(right), m_Bottom(bottom), m_Top(top)
{
    RecalculateProjectionMatrix();
    RecalculateViewMatrix();
}

void OrthographicCamera::SetPosition(const cass::Vector3<float>& position) {
    m_Position = position;
    RecalculateViewMatrix();
}

void OrthographicCamera::SetRotation(float rotation) {
    m_Rotation = rotation;
    RecalculateViewMatrix();
}

void OrthographicCamera::SetZoom(float zoom)
{
    m_Zoom = zoom;
    RecalculateProjectionMatrix();
}

void OrthographicCamera::ZoomAt(float zoomFactor, const cass::Vector3<float>& focusPoint)
{
    cass::Vector3<float> offset = (focusPoint - m_Position) * (1.0f - zoomFactor / m_Zoom);

    m_Position += offset;
    m_Zoom = zoomFactor;

    RecalculateProjectionMatrix();
    RecalculateViewMatrix();
}

void OrthographicCamera::RecalculateProjectionMatrix() {
    float left = m_Left * m_Zoom;
    float right = m_Right * m_Zoom;
    float bottom = m_Bottom * m_Zoom;
    float top = m_Top * m_Zoom;

    m_Projection = cass::Matrix4<float>::ortho(left, right, bottom, top, -1.0f, 1.0f);
    m_ViewProjection = m_Projection * m_View;
}

void OrthographicCamera::RecalculateViewMatrix()
{
    m_View = cass::Matrix4<float>::identity();
    m_View.rotateZ(-m_Rotation);
    m_View.translate(-m_Position);

    m_ViewProjection = m_Projection * m_View;
}
