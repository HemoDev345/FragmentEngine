#pragma once

#ifndef FM_DIST
    #define TRACY_ENABLE
    #include <tracy/Tracy.hpp>
    #define FM_PROFILE_FRAME FrameMark
    #define FM_PROFILE_FRAME_NAMED(x) FrameMarkNamed(x)
    #define FM_PROFILE_FUNC ZoneScoped
    #define FM_PROFILE_SCOPE(x) ZoneScopedN(x)
    #define FM_PROFILE_MESSAGE(x, size) TracyMessage(x, size)

    void* operator new(std :: size_t count )
    {
        auto ptr = malloc ( count );
        TracyAlloc (ptr , count );
        return ptr;
    }

    void operator delete (void* ptr) noexcept
    {
        TracyFree (ptr);
        free(ptr);
    }

#else
    #define FM_PROFILE_FRAME
    #define FM_PROFILE_FRAME_NAMED(x)
    #define FM_PROFILE_FUNC
    #define FM_PROFILE_SCOPE(x)
    #define FM_PROFILE_MESSAGE(x, size)
#endif


