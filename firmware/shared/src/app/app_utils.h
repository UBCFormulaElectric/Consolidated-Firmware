#pragma once
#include "math.h"

#define NUM_ELEMENTS_IN_ARRAY(array_pointer) sizeof(array_pointer) / sizeof(array_pointer[0])

/* @brief Extract the basename from a file path */
#define __BASENAME__(path) (__builtin_strrchr(path, '/') ? __builtin_strrchr(path, '/') + 1 : path)

#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN3(x, y, z) (MIN(MIN((x), (y)), (z)))
#define CLAMP(x, min, max) (MAX(MIN(x, max), min))

#define MAX_4_BITS_VALUE (uint32_t)(15)
#define MAX_6_BITS_VALUE (uint32_t)(63)
#define MAX_8_BITS_VALUE (uint32_t)(255)
#define MAX_10_BITS_VALUE (uint32_t)(1023)
#define MAX_12_BITS_VALUE (uint32_t)(4095)
#define MAX_16_BITS_VALUE (uint32_t)(65535)
#define MAX_32_BITS_VALUE (uint32_t)(4294967295)

#define IS_BIT_SET(input, bit) ((bool)((uint8_t)(input >> bit) & 1U))
#define SET_BIT_HIGH(input, bit) (input | (1U << bit))
#define SET_BIT_LOW(input, bit) (input & ~(1U << bit))

#define APPROX_EQUAL(a, b, threshold) ((bool)(fabs((a) - (b)) < threshold))

// Extra guard because HAL defines the same macro
#ifndef UNUSED
#define UNUSED(x) (void)(x)
#endif // UNUSED

// Change endianness of a 16-bit number
#define CHANGE_WORD_ENDIANNESS(word) ((uint16_t)(((word) >> 8U) | ((word) << 8U)))

#define IS_IN_RANGE(min, max, val) (((val) > (min)) && ((val) < (max)))

#define EMPTY_STRUCT(name)     \
    typedef struct             \
    {                          \
        unsigned char _unused; \
    } name;

typedef enum
{
    EXIT_CODE_OK = 0,
    EXIT_CODE_INVALID_ARGS,
    EXIT_CODE_OUT_OF_RANGE,
    EXIT_CODE_TIMEOUT,
    EXIT_CODE_ERROR,
    EXIT_CODE_UNIMPLEMENTED,
    NUM_EXIT_CODES,
} ExitCode;

#define EXIT_OK(code) ((code) == EXIT_CODE_OK)

#define RETURN_CODE_IF_EXIT_NOT_OK(code) \
    if ((code) != EXIT_CODE_OK)          \
    return (code)
