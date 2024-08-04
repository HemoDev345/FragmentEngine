#pragma once

#include <chrono>

namespace fm
{
    class Time
    {
    public:
        Time()
        {

        }

        ~Time()
        {

        }

        static void init()
        {
            m_speed = 1.f;
            m_start = std::chrono::system_clock::now();
            m_last = m_start;
        }
        
        static void restart()
        {
            std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
            m_elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(now - m_last).count();
            m_last = now;
        }

        static void setSpeedTime(float speed)
        {
            m_speed = speed;
        }

        static int64_t getFrameTimeTick()
        {
            std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
            return std::chrono::duration_cast<std::chrono::milliseconds>(now - m_last).count();
        }

        static float getElapsedTime()
        {
            return (m_elapsed_time / 1000.f) * m_speed;
        }

    private:
        inline static float m_speed;
        inline static int64_t m_elapsed_time;
        inline static std::chrono::time_point<std::chrono::system_clock> m_start;
        inline static std::chrono::time_point<std::chrono::system_clock> m_last;

    };
}


