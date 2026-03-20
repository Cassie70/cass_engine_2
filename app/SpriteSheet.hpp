class SpriteSheet
{
public:
    using Vec2 = cass::Vector2<float>;
    using Vec2i = cass::Vector2<int>;

    int rows{ 0 };
    int cols{ 0 };

    Vec2 spriteSizeUV{ 0.0f, 0.0f };
    Vec2 spacingUV{ 0.0f, 0.0f };
    Vec2 offsetUV{ 0.0f, 0.0f };

    // 🔹 Constructor por defecto
    SpriteSheet() = default;

    // 🔹 Constructor principal
    SpriteSheet(
        int textureWidth, int textureHeight,
        int spriteWidth, int spriteHeight,
        int rows, int cols,
        Vec2i spacing = { 0, 0 },
        Vec2i offset = { 0, 0 }
    )
        : rows(rows), cols(cols)
    {
        const float invW = 1.0f / textureWidth;
        const float invH = 1.0f / textureHeight;

        spriteSizeUV = {
            spriteWidth * invW,
            spriteHeight * invH
        };

        spacingUV = {
            spacing.x * invW,
            spacing.y * invH
        };

        offsetUV = {
            offset.x * invW,
            offset.y * invH
        };
    }

    cass::Vector4<float> GetUV(int row, int col) const { 
        float u0 = offsetUV.x + col * (spriteSizeUV.x + spacingUV.x); 
        float v0 = offsetUV.y + row * (spriteSizeUV.y + spacingUV.y); 
        float u1 = u0 + spriteSizeUV.x; float v1 = v0 + spriteSizeUV.y; 
        return { u0, v0, u1, v1 }; 
    }
};