#pragma once

#include "FM/Core/InputTypes.hpp"
#include "FM/Core/Util/Vec2.hpp"

namespace fm
{
    class Input
    {
    public:
        Input();
        Input(const Input&) = delete;
        void operator=(const Input&) = delete;
        
        ~Input();
        
        static void Update();
        
        static bool IsKeyPressed(KeyboardKey key);

        static bool IsKeyReleased(KeyboardKey key);

        static bool IsButtonPressed(MouseButton button);

        static bool IsButtonReleased(MouseButton button);
        
        static fm::Vec2i GetMousePosition();

        static int GetMouseScroll();

    private:
        void UpdateImpl();
        
        bool IsKeyPressedImpl(KeyboardKey key);

        bool IsKeyReleasedImpl(KeyboardKey key);

        bool IsButtonPressedImpl(MouseButton button);

        bool IsButtonReleasedImpl(MouseButton button);
        
        fm::Vec2i GetMousePositionImpl();

        int GetMouseScrollImpl();

        void SetKey(KeyboardKey key, bool value);
        void SetButton(MouseButton button, bool value);
        
        void SetMousePosition(fm::Vec2i position);

        void SetMouseScroll(int scroll);

        bool m_keys_buffer[255 /* KEYBOARD_KEY_MAX */];
        bool m_buttons_buffer[3 /* MOUSE_BUTTON_MAX */];
        fm::Vec2i m_mouse_position;
        int m_mouse_scroll;

        static Input& GetSelf();
        
        friend class Window;
    };

}
