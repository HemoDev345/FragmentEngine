#pragma once

#include <SDL2/SDL.h>

namespace fm 
{
    class Keyboard
    {
    public:
        Keyboard()
        {
    
        }

        ~Keyboard()
        {

        }

        static bool isKeyPressed(SDL_Scancode scancode)
        {
            return m_codes[scancode];
        }
        
        static bool isKeyReleased(SDL_Scancode scancode)
        {
            return (m_codes[scancode] == false && m_last_codes[scancode] == true);
        }

        static bool isKeyHolded(SDL_Scancode scancode)
        {
            return (m_codes[scancode] == true && m_last_codes[scancode] == true);
        }

    private:
        friend class Game;

        inline static bool m_last_codes[512 /*SDL_NUM_SCANCODES*/]{};
        inline static bool m_codes[512 /*SDL_NUM_SCANCODES*/]{};

        static void reset()
        {
            memcpy(m_last_codes, m_codes, SDL_NUM_SCANCODES);
        }
        
        static void handleEvent(SDL_Event event)
        {        
            switch (event.type)
            {
                case SDL_KEYDOWN:
                    m_last_codes[event.key.keysym.scancode] = m_codes[event.key.keysym.scancode];
                    m_codes[event.key.keysym.scancode] = true;
                    
                    break;
                case SDL_KEYUP:
                    m_last_codes[event.key.keysym.scancode] = m_codes[event.key.keysym.scancode];
                    m_codes[event.key.keysym.scancode] = false;
                    
                    break;
            }
        }
    };

}



