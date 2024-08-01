#if 1 

#include "FM/Platfrom/Renderer/Renderer.hpp"

#include "FM/Core/Log.hpp"
#include <glad/glad.h> 
#include <X11/Xlib.h>

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

    }

    Renderer &Renderer::getSelf() 
    {
        static Renderer renderer;
        return renderer;
    }
}

#endif
