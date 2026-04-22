#pragma once

struct SpriteSheetParams {
    int textureWidth = 0;
    int textureHeight = 0;
    int spriteWidth = 0;
    int spriteHeight = 0;
    int rows = 0;
    int cols = 0;
    cass::Vector2<int> spacing = { 0, 0 };
    cass::Vector2<int> offset = { 0, 0 };
};

class SpriteSheet
{
private:
    cass::Vector2<float> spriteSizeUV{ 0.0f, 0.0f };
    cass::Vector2<float> spacingUV{ 0.0f, 0.0f };
    cass::Vector2<float> offsetUV{ 0.0f, 0.0f };
public:

    int rows = 0;
    int cols = 0;

    SpriteSheet() = default;

    SpriteSheet(const SpriteSheetParams &params)
        : rows(params.rows), cols(params.cols)
    {
        const float invW = 1.0f / params.textureWidth;
        const float invH = 1.0f / params.textureHeight;

        spriteSizeUV = {
            params.spriteWidth * invW,
            params.spriteHeight * invH
        };

        spacingUV = {
            params.spacing.x * invW,
            params.spacing.y * invH
        };

        offsetUV = {
            params.offset.x * invW,
            params.offset.y * invH
        };
    }

    cass::Vector4<float> GetUV(int row, int col) const { 
        float u0 = offsetUV.x + col * (spriteSizeUV.x + spacingUV.x); 
        float v0 = offsetUV.y + row * (spriteSizeUV.y + spacingUV.y); 
        float u1 = u0 + spriteSizeUV.x; float v1 = v0 + spriteSizeUV.y; 
        return { u0, v0, u1, v1 }; 
    }
};