#pragma once

#include <SDL2/SDL.h>

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

        void static Init(SDL_Window* window) { getSelf().InitImpl(window); };

    private:
        Context m_context;

        void InitImpl(SDL_Window* window);
        static Renderer& getSelf();

    };
}
