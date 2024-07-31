#if defined(FM_RENDER_API_OPENGL)

#include "FM/Platfrom/Renderer/Renderer.hpp"

#include <glad/glad.h> 
#include <SDL2/SDL.h>

#include "FM/Core/SDLCall.hpp"

namespace fm
{
    Renderer::Renderer()
    {

    }

    Renderer::~Renderer()
    {

    }

    void Renderer::InitImpl(SDL_Window *window)
    {
        FM_SDL_CALL(SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3));
        FM_SDL_CALL(SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3));
        FM_SDL_CALL(SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE));

        FM_SDL_CALL(SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1));
        FM_SDL_CALL(SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24));

        SDL_GLContext context;
        m_context = SDL_GL_CreateContext(window);
        if (m_context == NULL)
        {
            FM_SDL_LOG_ERROR;
            exit(EXIT_FAILURE);
        }
        
        gladLoadGLLoader(SDL_GL_GetProcAddress);
        FM_LOG_INFO("vendor:   {}", (const char*)glGetString(GL_VENDOR));
        FM_LOG_INFO("renderer: {}", (const char*)glGetString(GL_RENDERER));
        FM_LOG_INFO("verion:  {}", (const char*)glGetString(GL_VERSION));
    }

    Renderer &Renderer::getSelf() 
    {
        static Renderer renderer;
        return renderer;
    }
}

#endif
