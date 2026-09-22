#pragma once
#include "Event.hpp"

namespace cass::engine {

class MouseScrolledEvent : public Event {

private:
  float XOffset, YOffset;

public:
  MouseScrolledEvent(float xOffset, float yOffset)
      : XOffset(xOffset), YOffset(yOffset) {}

  float GetXOffset() const { return XOffset; }
  float GetYOffset() const { return YOffset; }

  EventType GetType() const override { return EventType::MouseScrolled; }
};
} // namespace cass::engine