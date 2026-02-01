#include "SpriteSheet.hpp"
struct SpriteAnimation
{
    std::vector<int> frames;
    float frameTime;

    int currentFrame = 0;
    float timer = 0;

    void Update(float dt)
    {
        timer += dt;
        if (timer >= frameTime)
        {
            timer -= frameTime;
            currentFrame = (currentFrame + 1) % frames.size();
        }
    }

    cass::Vector4<float> GetUV(const SpriteSheet& sheet) const
    {
        int frameIndex = frames[currentFrame];

        int col = frameIndex % sheet.cols;
        int row = frameIndex / sheet.cols;

        float spriteW = 1.0f / sheet.cols;
        float spriteH = 1.0f / sheet.rows;

        // Considerando offsets y padding
        float u0 = sheet.offsetX + col * (spriteW + sheet.paddingX);
        float v0 = sheet.offsetY + row * (spriteH + sheet.paddingY);

        float u1 = u0 + spriteW;
        float v1 = v0 + spriteH;

        return { u0, v0, u1, v1 };
    }
};
