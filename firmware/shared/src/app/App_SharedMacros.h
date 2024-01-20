#pragma once
#include "math.h"

// clang-format off

#ifdef __arm__
#include "cmsis_os.h"
#endif

#define NUM_ELEMENTS_IN_ARRAY(array_pointer) \
    sizeof(array_pointer) / sizeof(array_pointer[0])

/* @brief Extract the basename from a file path */
#define __BASENAME__(path) \
    (__builtin_strrchr(path, '/') ? __builtin_strrchr(path, '/') + 1 : path)

#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN3(x, y, z) (MIN(MIN((x),(y)),(z)))
#define CLAMP(x, min, max) (MAX(MIN(x, max), min))
#define DEG_TO_RAD(degrees) ((degrees) * (float)M_PI / 180.0f)

#define IS_BIT_SET(input, bit) ((bool)((uint8_t)(input >> bit) & 1U))
#define SET_BIT_HIGH(input, bit) (input | (1U << bit))
#define SET_BIT_LOW(input, bit) (input & ~(1U << bit))

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

#define SECONDS_TO_HOUR(s) ((s) * (1/3600.0f))

// Grab current gear ratio
#define GEAR_RATIO (4.3f)

// Convert minutes to hours
#define MIN_TO_HOUR (60.0f)

// Grab wheel diameter
#define WHEEL_DIAMETER (16.0f)

// Convert inches to kilometers
#define INCH_TO_KM (0.0000254f)

#define RPM_TO_RADS(rpm) ((rpm) * (float)M_PI / 30.0f)

#define PI (3.1415926f)
#define RAD_TO_DEG(r) (r * 180.0f / PI)

#define MOTOR_RPM_TO_KMH(kmh) ((kmh) * (float)WHEEL_DIAMETER*PI*INCH_TO_KM*MIN_TO_HOUR/GEAR_RATIO) //take rpm of whell to kph

#define EMPTY_STRUCT(name)\
typedef struct \
{\
    unsigned char _unused;\
} name;
