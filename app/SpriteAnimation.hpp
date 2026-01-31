#include <vector>

struct SpriteAnimation
{
    int rows;
    int cols;

    std::vector<int> frames;

    float frameTime;

    int currentFrame = 0;
    float timer = 0;

    cass::Vector4<float> GetUV()
    {
        int frameIndex = frames[currentFrame];

        int col = frameIndex % cols;
        int row = frameIndex / cols;

        float w = 1.0f / cols;
        float h = 1.0f / rows;

        float u0 = col * w;
        float v0 = row * h;

        return { u0, v0, u0 + w, v0 + h };
    }

    void Update(float dt)
    {
        timer += dt;
        if (timer >= frameTime)
        {
            timer -= frameTime;
            currentFrame = (currentFrame + 1) % frames.size();
        }
    }
};
