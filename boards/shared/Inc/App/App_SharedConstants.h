#pragma once

#include <assert.h>

#define MAX_4_BITS_VALUE (uint32_t)(15)
#define MAX_6_BITS_VALUE (uint32_t)(63)
#define MAX_8_BITS_VALUE (uint32_t)(255)
#define MAX_10_BITS_VALUE (uint32_t)(1023)
#define MAX_12_BITS_VALUE (uint32_t)(4095)
#define MAX_16_BITS_VALUE (uint32_t)(65535)
#define MAX_32_BITS_VALUE (uint32_t)(4294967295)

#define MAX_TORQUE_REQUEST_NM (90.0f) // Max torque of EMRAX 188 HV CC motor

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

static_assert(HEX_DIGIT_0 == 0, "Hex enum must match its numeric value.");
static_assert(HEX_DIGIT_1 == 1, "Hex enum must match its numeric value.");
static_assert(HEX_DIGIT_2 == 2, "Hex enum must match its numeric value.");
static_assert(HEX_DIGIT_3 == 3, "Hex enum must match its numeric value.");
static_assert(HEX_DIGIT_4 == 4, "Hex enum must match its numeric value.");
static_assert(HEX_DIGIT_5 == 5, "Hex enum must match its numeric value.");
static_assert(HEX_DIGIT_6 == 6, "Hex enum must match its numeric value.");
static_assert(HEX_DIGIT_7 == 7, "Hex enum must match its numeric value.");
static_assert(HEX_DIGIT_8 == 8, "Hex enum must match its numeric value.");
static_assert(HEX_DIGIT_9 == 9, "Hex enum must match its numeric value.");
static_assert(HEX_DIGIT_A == 10, "Hex enum must match its numeric value.");
static_assert(HEX_DIGIT_B == 11, "Hex enum must match its numeric value.");
static_assert(HEX_DIGIT_C == 12, "Hex enum must match its numeric value.");
static_assert(HEX_DIGIT_D == 13, "Hex enum must match its numeric value.");
static_assert(HEX_DIGIT_E == 14, "Hex enum must match its numeric value.");
static_assert(HEX_DIGIT_F == 15, "Hex enum must match its numeric value.");
