#include <stdlib.h>

#include "App_SevenSegDisplays.h"
#include "App_SevenSegDisplay.h"
#include "App_SharedConstants.h"
#include "App_SharedAssert.h"

struct SevenSegDisplays
{
    struct SevenSegDisplay *displays[NUM_SEVEN_SEG_DISPLAYS_DIGITS];
};

struct SevenSegDisplays *App_SevenSegDisplays_Create(
    struct SevenSegDisplay *const left_seven_seg_display,
    struct SevenSegDisplay *const middle_seven_seg_display,
    struct SevenSegDisplay *const right_seven_seg_display)
{
    struct SevenSegDisplays *seven_seg_displays =
        malloc(sizeof(struct SevenSegDisplays));

    shared_assert(seven_seg_displays != NULL);

    seven_seg_displays->displays[SEVEN_SEG_DISPLAYS_LEFT_DIGIT] =
        left_seven_seg_display;
    seven_seg_displays->displays[SEVEN_SEG_DISPLAYS_MIDDLE_DIGIT] =
        middle_seven_seg_display;
    seven_seg_displays->displays[SEVEN_SEG_DISPLAYS_RIGHT_DIGIT] =
        right_seven_seg_display;

    return seven_seg_displays;
}

void App_SevenSegDisplays_Destroy(
    struct SevenSegDisplays *const seven_seg_displays)
{
    free(seven_seg_displays);
}

void App_SevenSegDisplays_SetHexDigits(
    const struct SevenSegDisplays *const seven_seg_displays,
    const uint8_t                        hex_digits[],
    size_t                               num_hex_digits)
{
    shared_assert(num_hex_digits <= NUM_SEVEN_SEG_DISPLAYS_DIGITS);

    for (size_t i = 0; i < NUM_SEVEN_SEG_DISPLAYS_DIGITS; i++)
    {
        struct SevenSegHexDigit hex_digit;

        if (num_hex_digits == 0)
        {
            hex_digit.enabled = false;
        }
        else
        {
            // TODO: return error code instead of assert failure

            shared_assert(hex_digits[i] < NUM_HEX_DIGITS);
            hex_digit.enabled = true;
            hex_digit.value   = hex_digits[i];
        }

        App_SevenSegDisplay_SetDigit(
            seven_seg_displays->displays[i], hex_digit);
    }
}
