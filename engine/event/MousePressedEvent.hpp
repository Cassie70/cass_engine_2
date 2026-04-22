class MousePressedEvent : public Event
{
private:
    int m_Button;

public:
    MousePressedEvent(int button): m_Button(button) {}

    int GetButton() const { return m_Button; }

    EventType GetType() const override {
        return EventType::MousePressed;
    }
};