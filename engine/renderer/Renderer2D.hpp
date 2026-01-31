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
    cass::Vector4<float> uv = { 0, 0, 1, 1 };
    cass::Vector2<float> origin = { 0, 0 };
	bool isCircle = false;
};

struct CartesianLineProperties {
	cass::Vector2<float> start;
	cass::Vector2<float> end;
	uint32_t argb = 0xFFFFFFFF;
	float weight = 1;
	float origin = 0.5;
};

struct PolarLineProperties {
	cass::Vector2<float> start;
	float length;
	float angle;
	uint32_t argb = 0xFFFFFFFF;
	float weight = 1;
	float origin = 0.5;
};

struct CircleProperties {
	cass::Vector2<float> center;
	float radius;
	uint32_t argb = 0xFFFFFFFF;
	Texture2D* texture = nullptr;
};

struct SpriteProperties {
	cass::Vector2<float> position;
	cass::Vector2<float> size;
	float angle;
	Texture2D* texture = nullptr;
	cass::Vector4<float> uv = { 0, 0, 1, 1 };
	cass::Vector2<float> origin = { 0, 0 };
	bool flipX = false;
	bool flipY = false;

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
	static void DrawCartesianLine(const CartesianLineProperties &properties);
	static void DrawPolarLine(const PolarLineProperties &properties);
	static void DrawCircle(const CircleProperties &properties);
	static void DrawSprite(const SpriteProperties& properties);

};