#pragma once
#include "Event.hpp"

namespace cass::engine {

class WindowResizeEvent : public Event {
public:
  WindowResizeEvent(int w, int h) : Width(w), Height(h) {}

  virtual EventType GetType() const override { return EventType::WindowResize; }

  int Width, Height;
};
} // namespace cass::engine