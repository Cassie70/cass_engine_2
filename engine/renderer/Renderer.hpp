#pragma once
#include <cstdint>

class Renderer {
public:
    static void Init();

    static void BeginFrame();
    static void EndFrame();

    static void SetClearColor(const uint32_t argb);
};
