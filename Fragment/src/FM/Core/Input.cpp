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

