#pragma once
#include <math.h>
#include <assert.h>
#include "io_log.h"

#define NUM_ELEMENTS_IN_ARRAY(array_pointer) sizeof(array_pointer) / sizeof(array_pointer[0])

/* @brief Extract the basename from a file path */
#define __BASENAME__(path) (__builtin_strrchr(path, '/') ? __builtin_strrchr(path, '/') + 1 : path)

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

// Uses Cortex M7 FPU, got this from FMAC docs
#ifdef STM32H733xx
#define FLOAT_TO_Q1_15(x) (int16_t)(x*0x8000U)
#define Q1_15_TO_FLOAT(x) (float)(x / ((float)(0x8000U)))
#endif

typedef enum
{
    EXIT_CODE_OK = 0,
    EXIT_CODE_INVALID_ARGS,
    EXIT_CODE_OUT_OF_RANGE,
    EXIT_CODE_TIMEOUT,
    EXIT_CODE_ERROR,
    EXIT_CODE_BUSY,
    EXIT_CODE_UNIMPLEMENTED,
    EXIT_CODE_RETRY_FAILED,
    EXIT_CODE_CHECKSUM_FAIL,
    EXIT_INDETERMINATE, // use this for when you don't know what the exit code is YET
    NUM_EXIT_CODES,
} ExitCode;

#define IS_EXIT_OK(code) ((code) == EXIT_CODE_OK)
#define IS_EXIT_ERR(code) ((code) != EXIT_CODE_OK)
#define ASSERT_EXIT_OK(code) (assert(code == EXIT_CODE_OK))

#define RETURN_IF_ERR(err_expr)                                                \
    {                                                                          \
        const ExitCode exit = err_expr;                                        \
        if (IS_EXIT_ERR(exit))                                                 \
        {                                                                      \
            LOG_ERROR(#err_expr " exited with an error, returning: %d", exit); \
            return exit;                                                       \
        }                                                                      \
    }

#define RETURN_IF_ERR_SILENT(err_expr)  \
    {                                   \
        const ExitCode exit = err_expr; \
        if (IS_EXIT_ERR(exit))          \
        {                               \
            return exit;                \
        }                               \
    }

#define LOG_IF_ERR(err_expr)                                        \
    {                                                               \
        const ExitCode exit = err_expr;                             \
        if (IS_EXIT_ERR(exit))                                      \
        {                                                           \
            LOG_ERROR(#err_expr " exited with an error: %d", exit); \
        }                                                           \
    }
