#pragma once
enum class EventType {
    None = 0,
    KeyPressed, KeyReleased,
    MouseButtonPressed, MouseButtonReleased,
    MouseMoved, MouseScrolled,
    WindowClose, WindowResize
};

class Event {
public:
    virtual EventType GetType() const = 0;
    bool Handled = false;
};