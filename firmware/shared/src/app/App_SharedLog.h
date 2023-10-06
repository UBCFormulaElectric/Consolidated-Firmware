#pragma once

#include <stdio.h>
#include "App_SharedMacros.h"

// clang-format off
#define ANSI_BOLD_GREEN  "\x1B[1;32m"
#define ANSI_BOLD_YELLOW "\x1B[1;33m"
#define ANSI_BOLD_RED    "\x1B[1;31m"
#define ANSI_RESET       "\x1B[1;0m"

#define LOG_DEBUG(format, ...) _LOG(ANSI_BOLD_GREEN "[DEBUG]", format, ##__VA_ARGS__)
#define LOG_WARN(format, ...)  _LOG(ANSI_BOLD_YELLOW "[WARN ]", format, ##__VA_ARGS__)
#define LOG_ERROR(format, ...) _LOG(ANSI_BOLD_RED "[ERROR]", format, ##__VA_ARGS__)

// Do NOT use this macro directly
#define _LOG(level, format, ...)                                        \
    printf(                                                             \
        level " %s:%u " format ANSI_RESET, __BASENAME__(__FILE__), __LINE__, \
        ##__VA_ARGS__)
