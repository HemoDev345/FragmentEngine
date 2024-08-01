#pragma once

#include "FM/Platfrom/Window/Window.hpp"

namespace fm
{
    class Renderer
    {
    public:
        typedef void* Context;

        Renderer();
        Renderer(const Renderer&) = delete;
        Renderer& operator=(const Renderer&) = delete;
        
        ~Renderer();

        void static Init(Window& window) { getSelf().InitImpl(window); };

    private:
        Context m_context;

        void InitImpl(Window& window);
        static Renderer& getSelf();

    };
}
