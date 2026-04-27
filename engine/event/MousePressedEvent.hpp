#pragma once
#include "Event.hpp"
#include <Mouse.hpp>

namespace cass {

    class MousePressedEvent : public Event {

    private:
        Mouse m_Button;

    public:
        MousePressedEvent(Mouse button)
            : m_Button(button) {
        }

        Mouse GetButton() const { return m_Button; }

        EventType GetType() const override {
            return EventType::MousePressed;
        }
    };
}