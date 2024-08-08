#pragma once

#include <FM/Core/Util/Vec2.hpp>
#include <FM/Core/Event.hpp>
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

        bool PollEvent(Event& event);

        void InitRenderContext();

        void Display();

    private:
        Context m_context;
        
        int KeySymToKeyboradKey(int key);
    };

}


