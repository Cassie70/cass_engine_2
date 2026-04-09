#pragma once
#include "Event.hpp"

class WindowResizeEvent: public Event {
public:
    WindowResizeEvent(int w, int h) : Width(w), Height(h) {}

    virtual EventType GetType() const override {
        return EventType::WindowResize;
    }

    int Width, Height;
};