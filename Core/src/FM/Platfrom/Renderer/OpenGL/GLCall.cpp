
#if defined(FM_RENDER_API_OPENGL) & !defined(FM_DIST)

#include "GLCall.hpp"
#include "glad/glad.h"
#include "FM/Core/Log.hpp"

namespace fm
{
    void LogError()
    { 
        uint32_t error = glGetError();
        if (error != 0)
            FM_LOG_ERROR("opengl: {}", ConvertError(error));
    }

    const char* ConvertError(uint32_t error)
    {
        switch (error)
        {
            case GL_INVALID_ENUM:                  return("invalid eunm");
            case GL_INVALID_VALUE:                 return("invalid value");
            case GL_INVALID_OPERATION:             return("invalid operation");
            case GL_STACK_OVERFLOW:                return("stack overflow");
            case GL_STACK_UNDERFLOW:               return("stack underflow");
            case GL_OUT_OF_MEMORY:                 return("out of memory");
            case GL_INVALID_FRAMEBUFFER_OPERATION: return("invalid framebuffer operation");
        }
        return "INVALID_ERROR";
    }
}

#endif