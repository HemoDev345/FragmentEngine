#pragma once

#include <math.h>
#include <cstdint>

namespace fm
{
    template<typename Type>
    struct Vec2
    {
        Type x, y;

        Vec2()
        {
        }

        Vec2(Type _x, Type _y)
            : x(_x), y(_y)
        {

        }

        template<typename T>
        constexpr void operator+=(Vec2<T> vec)
        {
            x += vec.x; y += vec.y;
        }

        template<typename T>
        constexpr void operator-=(Vec2<T> vec)
        {
            x -= vec.x; y -= vec.y;
        }

        template<typename T>
        constexpr void operator*=(Vec2<T> vec)
        {
            x *= vec.x; y *= vec.y;
        }

        template<typename T>
        constexpr void operator/=(Vec2<T> vec)
        {
            x /= vec.x; y /= vec.y;
        }

        template<typename T>
        constexpr auto operator+(Vec2<T> vec)
        {
            return Vec2{ x + vec.x, y + vec.y };
        }
        
        template<typename T>
        constexpr auto operator-(Vec2<T> vec)
        {
            return Vec2{ x - vec.x, y - vec.y };
        }

        template<typename T>
        constexpr auto operator*(Vec2<T> vec)
        {
            return Vec2{ x * vec.x, y * vec.y };
        }
        
        template<typename T>
        constexpr auto operator/(Vec2<T> vec)
        {
            return Vec2{ x / vec.x, y / vec.y };
        }
        
        template<typename T>
        constexpr void operator=(Vec2<T> vec)
        {
            x = vec.x; y = vec.y;
        }
        
        template<typename T>
        constexpr bool operator==(Vec2<T> vec)
        {
            return (x == vec.x && y == vec.y);
        }

        template<typename T>
        constexpr void operator!=(Vec2<T> vec)
        {
            return !(x == vec.x && y == vec.y);
        }

        template<typename T>
        constexpr auto dot(Vec2<T> vec)
        {
            return x * vec.x + y * vec.y;
        }

        constexpr void norm()
        {
            float m = 1 / std::sqrt( x * x + y * y);
            x *= m;
            y *= m;
        }

        constexpr auto angle(float angle)
        {
            return Vec2{ std::sin(angle) * x, std::cos(angle) * y  };
        }

        template<typename T>
        constexpr float distance(Vec2<T> vec)
        {
            return std::sqrt((vec.x - x) * (vec.x - x) + (vec.y - y) * (vec.y - y)); 
        }
    };

    typedef Vec2<double> Vec2d;
    typedef Vec2<float> Vec2f;
    typedef Vec2<int32_t> Vec2i;
    typedef Vec2<int16_t> Vec2_i16;
    typedef Vec2<int8_t> Vec2_i8;
    typedef Vec2<uint32_t> Vec2u;
    typedef Vec2<uint16_t> Vec2_u16;
    typedef Vec2<uint8_t> Vec2_u8;
}

