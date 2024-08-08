#if FM_RENDER_API_OPENGL

#include "FM/Platfrom/Renderer/Renderer.hpp"

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
        window.InitRenderContext();
    }

    Renderer &Renderer::getSelf() 
    {
        static Renderer renderer;
        return renderer;
    }
}

#endif
