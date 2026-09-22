#pragma once
#include <cass_linear.hpp>

namespace cass::engine {

class Camera {
public:
  virtual ~Camera() = default;

  const linear::Matrix4<float> &GetProjection() const { return m_Projection; }
  const linear::Matrix4<float> &GetView() const { return m_View; }
  const linear::Matrix4<float> &GetViewProjection() const {
    return m_ViewProjection;
  }

protected:
  linear::Matrix4<float> m_Projection{1.0f};
  linear::Matrix4<float> m_View{1.0f};
  linear::Matrix4<float> m_ViewProjection{1.0f};
};
} // namespace cass::engine
