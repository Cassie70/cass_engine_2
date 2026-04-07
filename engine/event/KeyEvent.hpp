#pragma once
class KeyEvent : public Event {
protected:
    int m_KeyCode;
public:
    int GetKeyCode() const { return m_KeyCode; }
};

class KeyPressedEvent : public KeyEvent {
public:
    KeyPressedEvent(int key) { m_KeyCode = key; }

    EventType GetType() const override {
        return EventType::KeyPressed;
    }
};

class KeyReleasedEvent : public KeyEvent {
public:
    KeyReleasedEvent(int key) {
        m_KeyCode = key;
    }

    EventType GetType() const override {
        return EventType::KeyReleased;
    }
};