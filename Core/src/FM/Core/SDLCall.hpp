#pragma once

#include <SDL2/SDL.h>
#include "FM/Core/Log.hpp"

#if !defined(FM_DIST)
    #define FM_SDL_CALL(x) if (x < 0) FM_SDL_LOG_ERROR

    #define FM_SDL_LOG_ERROR FM_LOG_ERROR("sdl2: {}", SDL_GetError());
#else
    #define FM_SDL_CALL(x)

    #define FM_SDL_LOG_ERROR
#endif
