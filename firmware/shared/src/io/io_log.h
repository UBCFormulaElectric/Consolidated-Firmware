#pragma once

#include <stdio.h>
#include "App_SharedMacros.h"

#define ANSI_BOLD_GREEN "\x1B[1;32m"
#define ANSI_BOLD_YELLOW "\x1B[1;33m"
#define ANSI_BOLD_RED "\x1B[1;31m"
#define ANSI_RESET "\x1B[1;0m"

#define LOG_INFO(format, ...) _LOG(ANSI_BOLD_GREEN "INFO ", format, ##__VA_ARGS__)
#define LOG_WARN(format, ...) _LOG(ANSI_BOLD_YELLOW "WARN ", format, ##__VA_ARGS__)
#define LOG_ERROR(format, ...) _LOG(ANSI_BOLD_RED "ERROR", format, ##__VA_ARGS__)

// Do not use these macros directly!
#ifdef TARGET_EMBEDDED
#include "SEGGER_RTT.h"
#define _LOG_PRINTF(...) SEGGER_RTT_printf(0, ##__VA_ARGS__)
#elif
#define _LOG_PRINTF(...) printf(##__VA_ARGS__)
#endif

#define _LOG(level, format, ...) \
    _LOG_PRINTF("[" level ANSI_RESET "] (%s:%u) " format "\n", __BASENAME__(__FILE__), __LINE__, ##__VA_ARGS__)
