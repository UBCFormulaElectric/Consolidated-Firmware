#include <stdlib.h>

#include "App_SevenSegDisplay.h"
#include "App_SharedAssert.h"

enum HexDigits
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

struct SevenSegDisplay
{
    void (*set_digit)(uint8_t hex_digit);
};

struct SevenSegDisplay *
    App_SevenSegDisplay_Create(void (*const set_digit)(uint8_t hex_digit))
{
    struct SevenSegDisplay *seven_seg_display =
        malloc(sizeof(struct SevenSegDisplay));

    seven_seg_display->set_digit = set_digit;

    return seven_seg_display;
}

void App_SevenSegDisplay_Destroy(
    struct SevenSegDisplay *const seven_seg_display)
{
    free(seven_seg_display);
}

void App_SevenSegDisplay_SetHexDigit(
    const struct SevenSegDisplay *const seven_seg_display,
    uint8_t                             hex_digit)
{
    shared_assert(hex_digit < NUM_HEX_DIGITS);
    seven_seg_display->set_digit(hex_digit);
}
