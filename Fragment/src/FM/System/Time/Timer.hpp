#pragma once

#include <chrono>

namespace fm
{
    class Timer
    {
    public:
        Timer()
        {

        }

        ~Timer()
        {

        }

        void init()
        {
            m_start = std::chrono::system_clock::now();
            m_last = m_start;
        }

        void restart()
        {
            m_last = std::chrono::system_clock::now();
        }

        int64_t getElapsedTimeTick()
        {
            std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
            return std::chrono::duration_cast<std::chrono::milliseconds>(now - m_last).count();
        }

        float getElapsedTime()
        {
            std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
            uint64_t elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(now - m_last).count();
            return (elapsed_time / 1000.f);
        }

    private:
        std::chrono::time_point<std::chrono::system_clock> m_start;
        std::chrono::time_point<std::chrono::system_clock> m_last;

    };
}


