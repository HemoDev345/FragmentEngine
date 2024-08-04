
#pragma once

#include "Log.hpp"

#if !defined(FM_RELEASE) && !defined(FM_DIST)
    #define FM_ASSERT(b, ...) if (!(b)) FM_LOG_ERROR(__VA_ARGS__)
#else
    #define FM_ASSERT(b, ...)
#endif
