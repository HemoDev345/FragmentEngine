#pragma once

#include <memory>
#include <SDL2/SDL.h>

#include "FM/Utility/Vec2.hpp"

namespace fm
{


    class Mouse
    {
    public:
        enum Button {
            Left,
            Wheel,
            Right,
        };

        Mouse()
        {

        }
        
        static bool isKeyPressed(Button button)
        {
            return getSelf().m_buttons[button].clicked;
        }
        
        static bool isKeyReleased(Button button)
        {
            return (getSelf().m_buttons[button].clicked == false) && getSelf().m_buttons[button].last_clicked;
        }

        static bool isKeyHolded(Button button)
        {
            return getSelf().m_buttons[button].clicked && (getSelf().m_buttons[button].last_clicked == false);
        }

        ~Mouse()
        {

        }


    private:
        struct State 
        {
            bool clicked;
            bool last_clicked;
            bool clicks;
        } m_buttons[3], m_last_buttons[3];
        fm::Vec2i m_position;
        fm::Vec2i m_wheel;

        static Mouse& getSelf()
        {
            static Mouse mouse;
            return mouse;
        }
        
        void reset()
        {
            memcpy(m_last_buttons, m_buttons, sizeof(m_buttons));
        }

        void handleEvent(SDL_Event& event)
        {
            switch (event.type)
            {
            case SDL_MOUSEBUTTONDOWN:
                m_buttons[event.button.button].clicked = true;
                m_buttons[event.button.button].clicks = event.button.clicks;
                break;

            case SDL_MOUSEBUTTONUP:
                m_buttons[event.button.button].clicked = false;
                break;
            
            case SDL_MOUSEMOTION:
                m_position = { event.motion.x, event.button.y };

            case SDL_MOUSEWHEEL:
                m_wheel = { event.wheel.x, event.wheel.y };
                
                break;
            
            default:
                break;
            };
        }

        friend class Game;
    };
}
