#pragma once
#include <vector>
#include "Timer.hpp"
#include <functional>

class Scheduler {
public:
    static void Update(float dt)
    {
        for (auto& t : timers)
            t.Update(dt);
    }

    // Ejecutar una vez
    static void After(float time, Timer::Callback cb)
    {
        Timer t;
        t.Start(time, cb, false);
        timers.push_back(t);
    }

    // Ejecutar repetidamente
    static void Every(float time, Timer::Callback cb)
    {
        Timer t;
        t.Start(time, cb, true);
        timers.push_back(t);
    }

    static void Clear()
    {
        timers.clear();
    }

private:
    static inline std::vector<Timer> timers;
};
