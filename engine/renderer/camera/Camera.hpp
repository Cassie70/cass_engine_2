#pragma once
#include <cass_linear.hpp>

class Camera {
public:
    virtual ~Camera() = default;

    const cass::Matrix4<float>& GetProjection() const { return m_Projection; }
    const cass::Matrix4<float>& GetView() const { return m_View; }
    const cass::Matrix4<float>& GetViewProjection() const { return m_ViewProjection; }

protected:
    cass::Matrix4<float> m_Projection{ 1.0f };
    cass::Matrix4<float> m_View{ 1.0f };
    cass::Matrix4<float> m_ViewProjection{ 1.0f };
};
