#pragma once
#include <EventType.hpp>
class Event {
public:
    virtual EventType GetType() const = 0;
    bool Handled = false;
};
