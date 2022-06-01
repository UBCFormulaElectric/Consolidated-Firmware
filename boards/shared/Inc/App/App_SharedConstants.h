#pragma once

// clang-format off

#define MAX_4_BITS_VALUE  (uint32_t)(15)
#define MAX_6_BITS_VALUE  (uint32_t)(63)
#define MAX_8_BITS_VALUE  (uint32_t)(255)
#define MAX_10_BITS_VALUE (uint32_t)(1023)
#define MAX_12_BITS_VALUE (uint32_t)(4095)
#define MAX_16_BITS_VALUE (uint32_t)(65535)
#define MAX_32_BITS_VALUE (uint32_t)(4294967295)

// Generic enum for hexadecimal digits. The order of the enums is important and
// shouldn't be changed.
enum HexDigit
{
    HEX_DIGIT_0,
    HEX_DIGIT_1,
    HEX_DIGIT_2,
    HEX_DIGIT_3,
    HEX_DIGIT_4,
    HEX_DIGIT_5,
    HEX_DIGIT_6,
    HEX_DIGIT_7,
    HEX_DIGIT_8,
    HEX_DIGIT_9,
    HEX_DIGIT_A,
    HEX_DIGIT_B,
    HEX_DIGIT_C,
    HEX_DIGIT_D,
    HEX_DIGIT_E,
    HEX_DIGIT_F,
    NUM_HEX_DIGITS,
};

typedef enum
{
    ACCUMULATOR_SEGMENT_0 = 0U,
    ACCUMULATOR_SEGMENT_1,
    ACCUMULATOR_SEGMENT_2,
    ACCUMULATOR_SEGMENT_3,
    ACCUMULATOR_SEGMENT_4,
    ACCUMULATOR_SEGMENT_5,
    NUM_OF_ACCUMULATOR_SEGMENTS,
} AccumulatorSegments_E;
