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
        std::cout << "GL Vendor: " << glGetString(GL_VENDOR) << "\n";
        std::cout << "GL Renderer: " << glGetString(GL_RENDERER) << "\n";
        std::cout << "GL Version: " << glGetString(GL_VERSION) << "\n";
        std::cout << "GL Shading Language: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << "\n";
   }

    Renderer &Renderer::getSelf() 
    {
        static Renderer renderer;
        return renderer;
    }
}

#endif
