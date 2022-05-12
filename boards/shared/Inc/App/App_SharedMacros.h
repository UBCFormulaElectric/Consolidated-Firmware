#pragma once

// clang-format off

#ifdef __arm__
#include "cmsis_os.h"
#endif

#define NUM_ELEMENTS_IN_ARRAY(array_pointer) \
    sizeof(array_pointer) / sizeof(array_pointer[0])

/* @brief Extract the basename from a file path */
#define __BASENAME__(path) \
    (__builtin_strrchr(path, '/') ? __builtin_strrchr(path, '/') + 1 : path)

#define min(x, y) (((x) < (y)) ? (x) : (y))

// Extra guard because HAL defines the same macro
#ifndef UNUSED
#define UNUSED(x) (void)(x)
#endif // UNUSED

// Change endianness of a 16-bit number
#define CHANGE_WORD_ENDIANNESS(word) ((uint16_t)(((word) >> 8U) | ((word) << 8U)))

// Convert the tick rate to a time in ms
#define TICK_TO_MS(tick) ((tick) * portTICK_PERIOD_MS)

// Convert seconds to milliseconds
#define S_TO_MS(time_s) ((uint32_t)((time_s) * 1000U))

#define IS_IN_RANGE(min,max,val) (((val) > (min)) && ((val) < (max)))
