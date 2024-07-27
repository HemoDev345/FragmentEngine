#pragma once

#include <string>
#include <iostream>


#if !defined(FM_DIST)
    #define FM_LOG_ENABLE
    #define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
    #include "spdlog/spdlog.h"
    
    #define FM_LOG_INIT() spdlog::set_pattern("[%H:%M:%S.%e] %^[%@: %l]%$ %v");

    #define FM_LOG_TRACE(...) SPDLOG_TRACE(__VA_ARGS__)
    #define FM_LOG_INFO(...) SPDLOG_INFO(__VA_ARGS__)
    #define FM_LOG_WARNING(...) SPDLOG_WARN(__VA_ARGS__) 
    #define FM_LOG_ERROR(...) SPDLOG_ERROR(__VA_ARGS__)

#else

    #define FM_LOG_INIT() 
    #define FM_LOG_TRACE(...)
    #define FM_LOG_INFO(...)
    #define FM_LOG_WARNING(...)
    #define FM_LOG_ERROR(...)

#endif

