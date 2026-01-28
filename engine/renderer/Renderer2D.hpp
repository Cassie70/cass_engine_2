#pragma once
#include <cass_linear.hpp>
#include "Texture2D.hpp"
#include <camera/OrthographicCamera.hpp>

struct Renderer2DStats {
	uint32_t DrawCalls = 0;
	uint32_t QuadCount = 0;
	uint32_t VertexCount = 0;
	uint32_t IndexCount = 0;
};


struct QuadProperties {
    cass::Matrix4<float>transform;
    uint32_t argb = 0xFFFFFFFF;
    Texture2D* texture = nullptr;
    cass::Vector2<float> origin = { 0, 0 };
    cass::Vector4<float> uv = { 0, 0, 1, 1 };
};

struct LineProperties {
	cass::Vector2<float> start;
	cass::Vector2<float> end;
	float weight = 1;
	uint32_t argb = 0xFFFFFFFF;
};

class Renderer2D {
public:
	static const Renderer2DStats& GetStats();
	static void ResetStats();
	static void Init();
	static void ShutDown();
	static void BeginScene(const OrthographicCamera &camera);
	static void EndScene();

	static void DrawQuad(const QuadProperties &properties);
	static void DrawLine(const LineProperties &properties);

};