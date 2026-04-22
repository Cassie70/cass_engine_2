#pragma once

enum class EventType {
    None = 0,
    KeyPressed, KeyReleased,
    MousePressed, MouseReleased,
    MouseMoved, MouseScrolled,
    WindowClose, WindowResize
};