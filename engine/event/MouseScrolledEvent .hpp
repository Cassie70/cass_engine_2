#pragma once
#include "Event.hpp"

class MouseScrolledEvent : public Event
{
public:
    MouseScrolledEvent(float xOffset, float yOffset)
        : XOffset(xOffset), YOffset(yOffset) {
    }

    float GetXOffset() const { return XOffset; }
    float GetYOffset() const { return YOffset; }

    EventType GetType() const override { return EventType::MouseScrolled; }

private:
    float XOffset, YOffset;
};