#pragma once
#include <math.h>
#include <assert.h>
#include "io/log.h"

#define NUM_ELEMENTS_IN_ARRAY(array_pointer) sizeof(array_pointer) / sizeof(array_pointer[0])

#ifndef MIN
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#endif

#ifndef MAX
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#endif

#define MIN3(x, y, z) (MIN(MIN((x), (y)), (z)))
#define MIN4(w, x, y, z) (MIN(MIN(MIN((w), (x)), (y)), (z)))
#define CLAMP(x, min, max) (MAX(MIN(x, max), min))
#define CLAMP_TO_ONE(x) (((x) <= 0) ? 1 : ((x) > 1 ? 1 : (x))) // initialize to 1 if value is <=0
#define SQUARE(x) ((x) * (x))

#define MAX_4_BITS_VALUE (uint32_t)(15)
#define MAX_6_BITS_VALUE (uint32_t)(63)
#define MAX_8_BITS_VALUE (uint32_t)(255)
#define MAX_10_BITS_VALUE (uint32_t)(1023)
#define MAX_11_BITS_VALUE (uint32_t)(2047)
#define MAX_12_BITS_VALUE (uint32_t)(4095)
#define MAX_16_BITS_VALUE (uint32_t)(65535)
#define MAX_32_BITS_VALUE (uint32_t)(4294967295)

#define IS_BIT_SET(input, bit) ((bool)((uint8_t)(input >> bit) & 1U))
#define SET_BIT_HIGH(input, bit) (input | (1U << bit))
#define SET_BIT_LOW(input, bit) (input & ~(1U << bit))

#define APPROX_EQUAL(a, b, threshold) ((bool)(fabs((a) - (b)) < threshold))
#define APPROX_EQUAL_FLOAT(a, b, threshold) ((bool)(fabsf((a) - (b)) < threshold))

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

#ifdef __cplusplus
#define CFUNC extern "C"
#define NORET [[noreturn]]
#else
#define CFUNC
#define NORET _Noreturn
#endif