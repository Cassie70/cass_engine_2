#pragma once

#include <string>
#include <cstdint>

class Renderer2D; // forward declaration

class Texture2D {
public:
    Texture2D(const std::string& path);
    ~Texture2D();

    uint32_t GetWidth() const { return m_Width; }
    uint32_t GetHeight() const { return m_Height; }

    Texture2D(const Texture2D&) = delete;
    Texture2D& operator=(const Texture2D&) = delete;

private:
    void Bind(uint32_t slot = 0) const;
    Texture2D(uint32_t rendererID, uint32_t width, uint32_t height);

    uint32_t m_Width = 0;
    uint32_t m_Height = 0;
    uint32_t m_RendererID = 0;

    friend class Renderer2D;
};