#pragma once
#include <cstdint>

namespace cass::engine {
class Renderer {
public:
  static void Init();

  static void BeginFrame();
  static void EndFrame();

  static void SetClearColor(const uint32_t argb);
};
} // namespace cass::engine
