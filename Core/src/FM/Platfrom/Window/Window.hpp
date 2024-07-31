#pragma once

#include <FM/Utility/Vec2.hpp>

namespace fm
{
    class Window
    {
    public:
        typedef void* Context;

        Window();
        Window(fm::Vec2f position, fm::Vec2u size, const char* name);

        ~Window();

        void Init(fm::Vec2f position, fm::Vec2u size, const char* name);
        void Shutdown();

        void PollEvent();

    private:
        Context m_context;

    };

}

