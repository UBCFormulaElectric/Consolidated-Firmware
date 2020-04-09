#include <stdlib.h>

#include "App_SevenSegDisplay.h"
#include "App_SharedConstants.h"
#include "App_SharedAssert.h"

struct SevenSegDisplay
{
    void (*set_digit)(uint8_t hex_digit);
};

struct SevenSegDisplay *
    App_SevenSegDisplay_Create(void (*const set_digit)(uint8_t hex_digit))
{
    struct SevenSegDisplay *seven_seg_display =
        malloc(sizeof(struct SevenSegDisplay));

    shared_assert(seven_seg_display != NULL);

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
