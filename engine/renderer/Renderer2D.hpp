#pragma once
#include <camera/OrthographicCamera.hpp>
#include <cass_linear.hpp>

#include "Texture2D.hpp"

namespace cass::engine {
enum class Shape : uint8_t { Quad = 0, Text = 1, Circle = 2 };

struct Renderer2DStats {
  uint32_t DrawCalls = 0;
  uint32_t QuadCount = 0;
  uint32_t VertexCount = 0;
  uint32_t IndexCount = 0;
  uint32_t TextureCount = 0;
};

struct QuadProperties {
  linear::Matrix4<float> transform;
  uint32_t argb = 0xFFFFFFFF;
  Texture2D *texture = nullptr;
  linear::Vector4<float> uv = {0, 0, 1, 1};
  linear::Vector2<float> origin = {0, 0};
  Shape shape = Shape::Quad;
};

struct CartesianLineProperties {
  linear::Vector2<float> start;
  linear::Vector2<float> end;
  uint32_t argb = 0xFFFFFFFF;
  float weight = 1;
  float origin = 0.5;
};

struct PolarLineProperties {
  linear::Vector2<float> start;
  float length;
  float angle;
  uint32_t argb = 0xFFFFFFFF;
  float weight = 1;
  float origin = 0.5;
};

struct CircleProperties {
  linear::Vector2<float> position;
  float radius;
  uint32_t argb = 0xFFFFFFFF;
  Texture2D *texture = nullptr;
};

struct SpriteProperties {
  linear::Vector2<float> position;
  linear::Vector2<float> size;
  float angle = 0.0f;
  Texture2D *texture = nullptr;
  linear::Vector4<float> uv = {0, 0, 1, 1};
  linear::Vector2<float> origin = {0, 0};
  bool flipX = false;
  bool flipY = false;
};

struct TextProperties {
  const uint32_t font;
  const std::string &text;
  linear::Vector2<float> position;
  uint32_t argb = 0xFFFFFFFF;
  linear::Vector2<float> spacing = {0, 0};
  bool kerning = true;
  linear::Vector2<float> scale = {1.0f, 1.0f};
  float angle = 0.0f;
};

class Renderer2D {
public:
  static const Renderer2DStats &GetStats();
  static void ResetStats();
  static void Init();
  static void ShutDown();
  static void BeginScene(const OrthographicCamera &camera);
  static void EndScene();

  static void DrawQuad(const QuadProperties &properties);
  static void DrawCartesianLine(const CartesianLineProperties &properties);
  static void DrawPolarLine(const PolarLineProperties &properties);
  static void DrawCircle(const CircleProperties &properties);
  static void DrawSprite(const SpriteProperties &properties);
  static linear::Vector2<float> DrawText(const TextProperties &properties);
};
} // namespace cass::engine