#include "SpriteSheet.hpp"
struct SpriteAnimation
{
    std::vector<cass::Vector2<int>> frames;
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
        const auto& frame = frames[currentFrame];

        int col = frame.x;
        int row = frame.y;

        return sheet.GetUV(row, col);
    }
};
