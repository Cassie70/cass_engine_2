#pragma once
#include <vector>
#include "SpriteSheet.hpp"

struct SpriteAnimationParams {
    std::vector<cass::Vector2<int>> frames;
    float frameTime = 0;
};


class SpriteAnimation
{
private:
    std::vector<cass::Vector2<int>> frames;
    float frameTime;

    int currentFrame = 0;
    float timer = 0;

public:

    SpriteAnimation() = default;

    SpriteAnimation(const SpriteAnimationParams &params)
        : frames(params.frames), frameTime(params.frameTime) {
    }

    void Update(float dt)
    {
        timer += dt;
        if (timer >= frameTime && !frames.empty())
        {
            timer -= frameTime;
            currentFrame = (currentFrame + 1) % frames.size();
        }
    }

    cass::Vector4<float> GetUV(const SpriteSheet& sheet) const
    {
        if (frames.empty()) return cass::Vector4<float>{};

        const auto& frame = frames[currentFrame];

        int col = frame.x;
        int row = frame.y;

        return sheet.GetUV(row, col);
    }

    void Reset()
    {
        currentFrame = 0;
        timer = 0.0f;
    }

    void SetFrameTime(float time) { frameTime = time; }
    float GetFrameTime() const { return frameTime; }

    int GetCurrentFrame() const { return currentFrame; }
};