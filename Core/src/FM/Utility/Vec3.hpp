#pragma once

#include <math.h>

namespace fm
{
    template<typename T>
    struct Vec3
    {
        T x, y, z;

        template<typename T>
        inline void operator+=(Vec3<T> vec)
        {
            x += vec.x; y += vec.y; z += vec.z;
        }

 

        template<typename T>
        inline auto dot(Vec2<T> vec)
        {
            return x * vec.x + y * vec.y;
        }

        inline void norm()
        {
            float m = 1 / std::sqrt( x * x + y * y);
            x *= m;
            y *= m;
        }

        inline auto angle(float angle)
        {
            return { std::sin(angle) * x, std::cos(angle) * y  }
        }
    };

    typedef Vec2<float> Vec2f;
    typedef Vec2<int> Vec2i;
    typedef Vec2<unsigned> Vec2u;
}

