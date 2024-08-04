#pragma once

#include <stdint.h>

namespace fm
{
    struct Color
    {
        Color(uint8_t red, uint8_t green, uint8_t blue)
            : r(red), g(green), b(blue)
        {
        }

        Color(uint32_t RGB)
            : r(RGB & 0x000000FF), g(RGB & 0x0000FF00), b(RGB & 0x00FF0000)
        {
        }

        uint32_t getRGB()
        {
            return (r) | (g << 8) | (b << 16)
        }
        
        ~Color()
        {
        }

        uint8_t r, g, b;
    }
}
