#pragma once

namespace cass {

    class Time {
    public:
        static float GetTime();
        static float GetDeltaTime();

    private:
        static float s_LastFrameTime;
    };
}