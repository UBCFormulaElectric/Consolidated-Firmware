#include <stdlib.h>
#include <assert.h>
#include <math.h>

#include "App_SharedExitCode.h"
#include "App_SevenSegDisplays.h"
#include "App_SevenSegDisplay.h"

struct SevenSegDisplays
{
    struct SevenSegDisplay *displays[NUM_SEVEN_SEG_DISPLAYS];
    void (*display_value_callback)(void);
};

struct SevenSegDisplays *App_SevenSegDisplays_Create(
    struct SevenSegDisplay *const left_seven_seg_display,
    struct SevenSegDisplay *const middle_seven_seg_display,
    struct SevenSegDisplay *const right_seven_seg_display,
    void (*const display_value_callback)(void))
{
    assert(display_value_callback != NULL);

    struct SevenSegDisplays *seven_seg_displays = malloc(sizeof(struct SevenSegDisplays));

    assert(seven_seg_displays != NULL);

    seven_seg_displays->displays[LEFT_SEVEN_SEG_DISPLAY]   = left_seven_seg_display;
    seven_seg_displays->displays[MIDDLE_SEVEN_SEG_DISPLAY] = middle_seven_seg_display;
    seven_seg_displays->displays[RIGHT_SEVEN_SEG_DISPLAY]  = right_seven_seg_display;
    seven_seg_displays->display_value_callback             = display_value_callback;

    return seven_seg_displays;
}

void App_SevenSegDisplays_Destroy(struct SevenSegDisplays *const seven_seg_displays)
{
    free(seven_seg_displays);
}

ExitCode App_SevenSegDisplays_SetHexDigits(
    const struct SevenSegDisplays *const seven_seg_displays,
    const uint8_t                        hex_digits[],
    size_t                               num_hex_digits)
{
    if (num_hex_digits > NUM_SEVEN_SEG_DISPLAYS || num_hex_digits == 0)
    {
        return EXIT_CODE_INVALID_ARGS;
    }

    // If any of the input digits is invalid, we don't write anything to the
    // 7-segment displays at all.
    for (size_t i = 0; i < num_hex_digits; i++)
    {
        if (hex_digits[i] >= NUM_HEX_DIGITS)
        {
            return EXIT_CODE_INVALID_ARGS;
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

        App_SevenSegDisplay_SetHexDigit(seven_seg_displays->displays[i], hex_digit);
    }

    seven_seg_displays->display_value_callback();

    return EXIT_CODE_OK;
}

ExitCode
    App_SevenSegDisplays_SetUnsignedBase10Value(const struct SevenSegDisplays *const seven_seg_displays, uint32_t value)
{
    if (value > pow(10, NUM_SEVEN_SEG_DISPLAYS) - 1)
    {
        return EXIT_CODE_INVALID_ARGS;
    }

    uint8_t digits[NUM_SEVEN_SEG_DISPLAYS];

    // Turn the base-10 value into individual digits. We treat a value of 0
    // as having 1 digit, which is why num_digits starts counting from 1.
    for (uint8_t num_digits = 1; num_digits <= NUM_SEVEN_SEG_DISPLAYS; num_digits++)
    {
        digits[num_digits - 1] = (uint8_t)(value % 10);
        value /= 10;

        if (value == 0)
        {
            App_SevenSegDisplays_SetHexDigits(seven_seg_displays, digits, num_digits);
            break;
        }
    }

    return EXIT_CODE_OK;
}
