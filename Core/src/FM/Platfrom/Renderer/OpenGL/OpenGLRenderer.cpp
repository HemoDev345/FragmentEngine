#if 1

#include "FM/Platfrom/Renderer/Renderer.hpp"
#include "FM/Core/Log.hpp"

#include <GL/glx.h>

namespace fm
{
    Renderer::Renderer()
    {

    }

    Renderer::~Renderer()
    {

    }

    void Renderer::InitImpl(Window& window)
    {
        FM_LOG_INFO("GL Vendor: {}", (const char*)glGetString(GL_VENDOR));
        FM_LOG_INFO("GL Renderer: {}", (const char*)glGetString(GL_RENDERER));
        FM_LOG_INFO("GL Version: {}", (const char*)glGetString(GL_VERSION));
        FM_LOG_INFO("GL Shading Language: {}", (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION));
        window.InitRenderContext();
    }

    Renderer &Renderer::getSelf() 
    {
        static Renderer renderer;
        return renderer;
    }
}

#endif
