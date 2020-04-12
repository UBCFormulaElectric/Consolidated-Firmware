#include <stdlib.h>

#include "App_ErrorCode.h"
#include "App_SevenSegDisplays.h"
#include "App_SevenSegDisplay.h"
#include "App_SharedConstants.h"
#include "App_SharedAssert.h"

struct SevenSegDisplays
{
    struct SevenSegDisplay *displays[NUM_SEVEN_SEG_DISPLAYS];
};

struct SevenSegDisplays *App_SevenSegDisplays_Create(
    struct SevenSegDisplay *const left_seven_seg_display,
    struct SevenSegDisplay *const middle_seven_seg_display,
    struct SevenSegDisplay *const right_seven_seg_display)
{
    struct SevenSegDisplays *seven_seg_displays =
        malloc(sizeof(struct SevenSegDisplays));

    shared_assert(seven_seg_displays != NULL);

    seven_seg_displays->displays[LEFT_SEVEN_SEG_DISPLAY] =
        left_seven_seg_display;
    seven_seg_displays->displays[MIDDLE_SEVEN_SEG_DISPLAY] =
        middle_seven_seg_display;
    seven_seg_displays->displays[RIGHT_SEVEN_SEG_DISPLAY] =
        right_seven_seg_display;

    return seven_seg_displays;
}

void App_SevenSegDisplays_Destroy(
    struct SevenSegDisplays *const seven_seg_displays)
{
    free(seven_seg_displays);
}

ErrorCode App_SevenSegDisplays_SetHexDigits(
    const struct SevenSegDisplays *const seven_seg_displays,
    const uint8_t                        hex_digits[],
    size_t                               num_hex_digits)
{
    if (num_hex_digits > NUM_SEVEN_SEG_DISPLAYS)
    {
        return ERROR_CODE_INVALID_ARGS;
    }

    // If any of the input digits is invalid, we don't write anything to the
    // 7-segment displays at all.
    for (size_t i = 0; i < num_hex_digits; i++)
    {
        if (hex_digits[i] >= NUM_HEX_DIGITS)
        {
            return ERROR_CODE_INVALID_ARGS;
        }
    }

    for (size_t i = 0; i < NUM_SEVEN_SEG_DISPLAYS; i++)
    {
        struct SevenSegHexDigit hex_digit;

        if (i < num_hex_digits)
        {
            hex_digit.enabled = true;
            hex_digit.value   = hex_digits[i];
        }
        else
        {
            // We turn off the 7-segment displays with unspecified values. For
            // example, if the callers wants to write 0xF, we would turn off the
            // right and middle 7-segment displays.
            hex_digit.enabled = false;
        }

        App_SevenSegDisplay_SetDigit(
            seven_seg_displays->displays[i], hex_digit);
    }

    return ERROR_CODE_OK;
}
