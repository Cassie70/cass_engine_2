#include "Texture2D.hpp"
#include <glad/glad.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <iostream>

Texture2D::Texture2D(const std::string& path)
{
    int width, height, channels;

    stbi_set_flip_vertically_on_load(1);
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 4);

    if (!data) {
        std::cerr << "Failed to load texture: " << path << std::endl;
        return;
    }

    m_Width = width;
    m_Height = height;

    glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
    glTextureStorage2D(m_RendererID, 1, GL_RGBA8, m_Width, m_Height);

    glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTextureSubImage2D(
        m_RendererID,
        0,
        0, 0,
        m_Width, m_Height,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        data
    );

    stbi_image_free(data);

}

Texture2D::Texture2D(uint32_t width, uint32_t height, const unsigned char* data)
{
    m_Width = width;
    m_Height = height;

    glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);

    // FreeType genera 1 canal (rojo)
    glTextureStorage2D(m_RendererID, 1, GL_R8, m_Width, m_Height);

    glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTextureSubImage2D(
        m_RendererID,
        0,
        0, 0,
        m_Width, m_Height,
        GL_RED,
        GL_UNSIGNED_BYTE,
        data
    );
}

Texture2D::~Texture2D() {
    if (m_RendererID)
        glDeleteTextures(1, &m_RendererID);
}

void Texture2D::Bind(uint32_t slot) const {
    glBindTextureUnit(slot, m_RendererID);
}

Texture2D::Texture2D(uint32_t id, uint32_t w, uint32_t h)
    : m_RendererID(id), m_Width(w), m_Height(h) {
}
