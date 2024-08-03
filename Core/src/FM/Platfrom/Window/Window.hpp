#pragma once

#include <FM/Utility/Vec2.hpp>

#include <FM/Core/Input.hpp>

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

        bool PollEvent();

        void InitRenderContext();

        void Display();

    private:
        Context m_context;
        
        int KeySymToKeyboradKey(int key);
    };

}


