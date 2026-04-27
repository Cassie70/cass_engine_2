#pragma once
#include "EventType.hpp"

namespace cass {

    class Event {
    public:
        virtual EventType GetType() const = 0;
        bool Handled = false;
    };
}
