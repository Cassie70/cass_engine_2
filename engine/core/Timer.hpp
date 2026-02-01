#pragma once
#include <functional>

class Timer {
public:
    using Callback = std::function<void()>;

    Timer() = default;

    void Start(float time, Callback cb, bool repeat = false)
    {
        duration = time;
        callback = cb;
        repeating = repeat;
        elapsed = 0.0f;
        active = true;
        paused = false;
    }

    void Stop()
    {
        active = false;
    }

    void Pause()
    {
        paused = true;
    }

    void Resume()
    {
        paused = false;
    }

    void Update(float dt)
    {
        if (!active || paused) return;

        elapsed += dt;

        if (elapsed >= duration)
        {
            if (callback)
                callback();

            if (repeating)
                elapsed = 0.0f;
            else
                active = false;
        }
    }

    bool Active() const { return active; }

private:
    float duration = 0;
    float elapsed = 0;

    bool active = false;
    bool paused = false;
    bool repeating = false;

    Callback callback;
};
