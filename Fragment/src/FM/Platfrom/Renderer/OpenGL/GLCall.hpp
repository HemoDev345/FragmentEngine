#pragma once

#include <cstdint>

#if defined(FM_RENDER_API_OPENGL) && !defined(FM_DIST)

#define FM_GL_CALL(x) x; fm::LogError()
#define FM_GL_LOG_ERROR fm::LogError()

namespace fm
{
    void LogError();
    const char* ConvertError(uint32_t error);
}

#else
    
    #define FM_GL_LOG_ERROR
    #define FM_GL_CALL(x) x

#endif
