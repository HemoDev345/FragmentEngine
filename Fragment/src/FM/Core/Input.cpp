#include "Input.hpp"
#include "FM/Core/InputTypes.hpp"

#include <cstring>

namespace fm
{
    Input::Input()
    {

    }

    Input::~Input()
    {

    }

    void Input::Update()
    {
    }
    
    bool Input::IsKeyPressed(KeyboardKey key)
    {
        return GetSelf().IsKeyPressedImpl(key);
    }

    bool Input::IsKeyReleased(KeyboardKey key)
    {
        return GetSelf().IsKeyPressedImpl(key);
    }

    bool Input::IsButtonPressed(MouseButton button)
    {
        return GetSelf().IsButtonPressedImpl(button);
    }

    bool Input::IsButtonReleased(MouseButton button)
    {
        return GetSelf().IsButtonReleasedImpl(button);
    }

    fm::Vec2i Input::GetMousePosition()
    {
        return GetSelf().GetMousePositionImpl();
    }

    int Input::GetMouseScroll()
    {
        return GetSelf().GetMouseScrollImpl();
    }

    bool Input::IsKeyPressedImpl(KeyboardKey key)
    {
        return m_keys_buffer[key];
    }

    bool Input::IsKeyReleasedImpl(KeyboardKey key)
    {
        return !m_keys_buffer[key];
    }

    bool Input::IsButtonPressedImpl(MouseButton button)
    {
        return m_buttons_buffer[button];
    }

    bool Input::IsButtonReleasedImpl(MouseButton button)
    {
        return !m_buttons_buffer[button];
    }

    fm::Vec2i Input::GetMousePositionImpl()
    {
        return m_mouse_position;
    }

    int Input::GetMouseScrollImpl()
    {
        return m_mouse_scroll;
    }

    void Input::HandleInput(Event& event)
    {
        switch (event.type) 
        {
            case EVENT_KEYBOARD_KEY_PRESS:
            case EVENT_KEYBOARD_KEY_RELEASE:
                m_keys_buffer[event.keyboard.key] = event.keyboard.press;
                break;

            case EVENT_MOUSE_BUTTON_PRESS:
            case EVENT_MOUSE_BUTTON_RELEASE:
                m_buttons_buffer[event.mouse.button] = event.mouse.press;
                break;

            case EVENT_MOUSE_MOTION:
                m_mouse_position = event.mouse.position;
                break;

            case EVENT_MOUSE_SCROLL:
                m_mouse_scroll = event.mouse.delta_scroll;

            default:
                break;
        }
    }

    void Input::SetKey(KeyboardKey key, bool value)
    {
        m_keys_buffer[key] = value;
    }
    
    void Input::SetButton(MouseButton key, bool value)
    {
        m_buttons_buffer[key] = value;
    }

    void Input::SetMousePosition(fm::Vec2i position)
    {
        m_mouse_position = position;
    }

    void Input::SetMouseScroll(int scroll)
    {
        m_mouse_scroll = scroll;
    }

    Input& Input::GetSelf()
    {
        static Input self;
        return self;
    }
}

