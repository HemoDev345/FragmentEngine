#pragma once

#include <cstdint>

namespace fm
{
    enum USAGE_TYPE
    {
        STREAM_DRAW,
        STREAM_READ,
        STREAM_COPY,
        STATIC_DRAW,
        STATIC_READ,
        STATIC_COPY,
        DYNAMIC_DRAW,
        DYNAMIC_READ,
        DYNAMIC_COPY
    };

    uint32_t ConvertUsage(USAGE_TYPE type);
}

