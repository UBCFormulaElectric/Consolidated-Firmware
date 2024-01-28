#pragma once

#include <stdio.h>
#include "app_utils.h"

/*
This logging module users SEGGER Real-Time Transfer (RTT) for printf-style debugging.
The SEGGER_RTT_printf function has the following spec:
(taken from SEGGER_RTT_printf.c, for reference)

Notes
*    (1) Conversion specifications have following syntax:
*          %[flags][FieldWidth][.Precision]ConversionSpecifier
*    (2) Supported flags:
*          -: Left justify within the field width
*          +: Always print sign extension for signed conversions
*          0: Pad with 0 instead of spaces. Ignored when using '-'-flag or precision
*        Supported conversion specifiers:
*          c: Print the argument as one char
*          d: Print the argument as a signed integer
*          u: Print the argument as an unsigned integer
*          x: Print the argument as an hexadecimal integer
*          s: Print the string pointed to by the argument
*          p: Print the argument as an 8-digit hexadecimal integer. (Argument shall be a pointer to void.)

*/

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
#define _LOG_PRINTF(format, ...) SEGGER_RTT_printf(0, format, ##__VA_ARGS__)
#else
#include <stdio.h>
#define _LOG_PRINTF(format, ...) printf(format, ##__VA_ARGS__)
#endif

#define _LOG(level, format, ...) \
    _LOG_PRINTF("[" level ANSI_RESET "] (%s:%u) " format "\n", __BASENAME__(__FILE__), __LINE__, ##__VA_ARGS__)
