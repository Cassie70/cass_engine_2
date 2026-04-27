#pragma once
#include "Event.hpp"
#include <Key.hpp>

namespace cass {

    class KeyEvent : public Event {
    protected:
        Key m_KeyCode;
    public:
        Key GetKeyCode() const { return m_KeyCode; }
    };

    class KeyPressedEvent : public KeyEvent {
    public:
        KeyPressedEvent(Key key) { m_KeyCode = key; }

        EventType GetType() const override {
            return EventType::KeyPressed;
        }
    };

    class KeyReleasedEvent : public KeyEvent {
    public:
        KeyReleasedEvent(Key key) {
            m_KeyCode = key;
        }

        EventType GetType() const override {
            return EventType::KeyReleased;
        }
    };
}