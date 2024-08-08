#pragma once

#include "FM/Core/InputTypes.hpp"
#include "FM/Core/Util/Vec2.hpp"
namespace fm
{
    enum EventType
    {
        EVENT_WINDOW_DISTROY, EVENT_WINDOW_RESIZE, EVENT_WINDOW_MOTION,
        EVENT_WINDOW_FOCUS, EVENT_WINDOW_UNFOCUS,
        EVENT_KEYBOARD_KEY_PRESS, EVENT_KEYBOARD_KEY_RELEASE, 
        EVENT_MOUSE_BUTTON_PRESS, EVENT_MOUSE_BUTTON_RELEASE, 
        EVENT_MOUSE_MOTION, EVENT_MOUSE_SCROLL,
        EVENT_MOUSE_ENTER, EVENT_MOUSE_LEAVE,
    };

    struct WindowEvent
    {
        EventType type;
        fm::Vec2i position;
        fm::Vec2i size;
        bool maximze;
        bool focus;
    };

    struct KeyboardEvent
    {
        EventType type;
        KeyboardKey key;
        bool press;
    };

    struct MouseEvent
    {
        EventType type;
        MouseButton button;
        Vec2i position;
        int delta_scroll;
        bool focus;
        bool press;
    };

    struct Event
    {
        EventType type;
        WindowEvent window;
        KeyboardEvent keyboard;
        MouseEvent mouse;
    };
}

