class MouseMovedEvent : public Event {
private:
    float m_X, m_Y;

public:
    MouseMovedEvent(float x, float y)
        : m_X(x), m_Y(y) {
    }

    float GetX() const { return m_X; }
    float GetY() const { return m_Y; }

    EventType GetType() const override {
        return EventType::MouseMoved;
    }
};