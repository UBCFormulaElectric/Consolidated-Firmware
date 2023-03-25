#include <stdlib.h>
#include <assert.h>
#include <math.h>

#include "App_SharedExitCode.h"
#include "App_SevenSegDisplays.h"
#include "App_SevenSegDisplay.h"

static uint8_t digits[NUM_SEVEN_SEG_DISPLAYS];

struct SevenSegDisplays
{
    struct SevenSegDisplay *displays[NUM_SEVEN_SEG_DISPLAYS];
    void (*display_value_callback)(void);
};

struct SevenSegDisplays *App_SevenSegDisplays_Create(
    struct SevenSegDisplay *left_l_seven_seg_display,
    struct SevenSegDisplay *left_m_seven_seg_display,
    struct SevenSegDisplay *left_r_seven_seg_display,
    struct SevenSegDisplay *middle_l_seven_seg_display,
    struct SevenSegDisplay *middle_m_seven_seg_display,
    struct SevenSegDisplay *middle_r_seven_seg_display,
    struct SevenSegDisplay *right_l_seven_seg_display,
    struct SevenSegDisplay *right_m_seven_seg_display,
    struct SevenSegDisplay *right_r_seven_seg_display,
    void (*const display_value_callback)(void))
{
    assert(display_value_callback != NULL);

    struct SevenSegDisplays *seven_seg_displays = malloc(sizeof(struct SevenSegDisplays));

    assert(seven_seg_displays != NULL);

    seven_seg_displays->displays[LEFT_L_SEVEN_SEG_DISPLAY]   = left_l_seven_seg_display;
    seven_seg_displays->displays[LEFT_M_SEVEN_SEG_DISPLAY]   = left_m_seven_seg_display;
    seven_seg_displays->displays[LEFT_R_SEVEN_SEG_DISPLAY]   = left_r_seven_seg_display;
    seven_seg_displays->displays[MIDDLE_L_SEVEN_SEG_DISPLAY] = middle_l_seven_seg_display;
    seven_seg_displays->displays[MIDDLE_M_SEVEN_SEG_DISPLAY] = middle_m_seven_seg_display;
    seven_seg_displays->displays[MIDDLE_R_SEVEN_SEG_DISPLAY] = middle_r_seven_seg_display;
    seven_seg_displays->displays[RIGHT_L_SEVEN_SEG_DISPLAY]  = right_l_seven_seg_display;
    seven_seg_displays->displays[RIGHT_M_SEVEN_SEG_DISPLAY]  = right_m_seven_seg_display;
    seven_seg_displays->displays[RIGHT_R_SEVEN_SEG_DISPLAY]  = right_r_seven_seg_display;
    seven_seg_displays->display_value_callback               = display_value_callback;

    return seven_seg_displays;
}

void App_SevenSegDisplays_Destroy(struct SevenSegDisplays *const seven_seg_displays)
{
    free(seven_seg_displays);
}

ExitCode
    App_SevenSegDisplays_SetHexDigits(const struct SevenSegDisplays *seven_seg_displays, const uint8_t hex_digits[])
{
    size_t len_hex_digits = 9; // TODO: This is bad, should get from somewhere else

    for (size_t i = 0; i < len_hex_digits; i++)
    {
        if (hex_digits[i] >= NUM_HEX_DIGITS)
        {
            return EXIT_CODE_INVALID_ARGS;
        }
    }

    return EXIT_CODE_OK;
}

void App_Set_Digits(const struct SevenSegDisplays *seven_seg_displays, uint8_t index)
{
    for (int i = index; i < NUM_IN_GROUP + index; i++)
    {
        struct SevenSegHexDigit hex_digit;
        hex_digit.enabled = true;
        hex_digit.value   = digits[i];

        App_SevenSegDisplay_SetHexDigit(seven_seg_displays->displays[i], hex_digit);
    }

    seven_seg_displays->display_value_callback();
}

ExitCode App_SevenSegDisplays_SetGroupL(const struct SevenSegDisplays *const seven_seg_displays, uint32_t value)
{
    if (value > pow(10, NUM_IN_GROUP) - 1)
    {
        return EXIT_CODE_INVALID_ARGS;
    }
    digits[0] = 0;
    digits[1] = 0;
    digits[2] = 0;

    // Turn the base-10 value into individual digits. Have to write backwards with how the
    // displays are initialized and how they are passed to the IO function.
    int shift = 3;
    for (int digits_index = shift; digits_index + shift > NUM_IN_GROUP; digits_index--)
    {
        digits[digits_index - 1] = (uint8_t)(value % 10);
        value /= 10;

        if (value == 0)
        {
            App_Set_Digits(seven_seg_displays, 0);
            break;
        }
    }

    return EXIT_CODE_OK;
}

ExitCode App_SevenSegDisplays_SetGroupM(const struct SevenSegDisplays *const seven_seg_displays, uint32_t value)
{
    if (value > pow(10, NUM_IN_GROUP) - 1)
    {
        return EXIT_CODE_INVALID_ARGS;
    }
    digits[3] = 0;
    digits[4] = 0;
    digits[5] = 0;

    // Turn the base-10 value into individual digits. Have to write backwards with how the
    // displays are initialized and how they are passed to the IO function
    int shift = 6;
    for (int digits_index = shift; digits_index + shift> NUM_IN_GROUP ; digits_index--)
    {
        digits[digits_index] = (uint8_t)(value % 10);
        value /= 10;

        if (value == 0)
        {
            App_Set_Digits(seven_seg_displays, 3);
            break;
        }
    }

    return EXIT_CODE_OK;
}

ExitCode App_SevenSegDisplays_SetGroupR(const struct SevenSegDisplays *const seven_seg_displays, uint32_t value)
{
    if (value > pow(10, NUM_IN_GROUP) - 1)
    {
        return EXIT_CODE_INVALID_ARGS;
    }

    // 'wiping' this section of the array

    digits[6] = 0;
    digits[7] = 0;
    digits[8] = 0;

    // Turn the base-10 value into individual digits. Have to write backwards with how the
    // displays are initialized and how they are passed to the IO function
    int shift = 8;
    for (int digits_index = shift; digits_index + shift > NUM_IN_GROUP ; digits_index--)
    {
        digits[digits_index] = (uint8_t)(value % 10);
        value /= 10;

        if (value == 0)
        {
            App_Set_Digits(seven_seg_displays, 6);
            break;
        }
    }

    return EXIT_CODE_OK;
}