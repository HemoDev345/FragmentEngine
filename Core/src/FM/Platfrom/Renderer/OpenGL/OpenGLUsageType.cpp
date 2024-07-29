#if defined(FM_RENDER_API_OPENGL)

#include "FM/Platfrom/Renderer/UsageType.hpp"
#include "FM/Core/Log.hpp"
#include "glad/glad.h"

namespace fm
{
    uint32_t ConvertUsage(USAGE_TYPE type)
    {
        switch (type)
        {
        case USAGE_TYPE::STREAM_DRAW:
            return GL_STREAM_DRAW;
        
        case USAGE_TYPE::STREAM_COPY:
            return GL_STREAM_COPY;
        
        case USAGE_TYPE::STREAM_READ:
            return GL_STREAM_READ;
        
        case USAGE_TYPE::STATIC_DRAW:
            return GL_STATIC_DRAW;
        
        case USAGE_TYPE::STATIC_COPY:
            return GL_STATIC_COPY;
        
        case USAGE_TYPE::STATIC_READ:
            return GL_STATIC_READ;
        
        case USAGE_TYPE::DYNAMIC_DRAW:
            return GL_DYNAMIC_DRAW;
        
        case USAGE_TYPE::DYNAMIC_COPY:
            return GL_DYNAMIC_DRAW;
        
        case USAGE_TYPE::DYNAMIC_READ:
            return GL_DYNAMIC_DRAW;
        }

        FM_LOG_ERROR("unkown usage {}", (int)type);
        return 0;
    }
}

#endif
