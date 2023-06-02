#include <stdlib.h>
#include <stdio.h>
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

uint8_t App_SevenSegDisplays_AddDecimalIndicator(uint8_t value)
{
    uint8_t temp = (uint8_t)(value + (uint8_t)10); //help
    return temp;
}


void App_SevenSegDisplays_Destroy(struct SevenSegDisplays *const seven_seg_displays)
{
    free(seven_seg_displays);
}

void App_SevenSegDisplays_SetDigits(const struct SevenSegDisplays *seven_seg_displays, uint8_t group_index)
{
    for (int i = group_index; i < NUM_IN_GROUP + group_index; i++)
    {
        struct SevenSegHexDigit hex_digit;
        hex_digit.enabled = true;
        hex_digit.value   = digits[i];

        App_SevenSegDisplay_SetHexDigit(seven_seg_displays->displays[i], hex_digit, i);
    }

    seven_seg_displays->display_value_callback();
}

ExitCode App_SevenSegDisplays_SetGroupL(const struct SevenSegDisplays *const seven_seg_displays, double value)
{
    if (value > pow(10, NUM_IN_GROUP) - 1)
    {
        return EXIT_CODE_INVALID_ARGS;
    }
    digits[LEFT_L_SEVEN_SEG_DISPLAY] = 0;
    digits[LEFT_M_SEVEN_SEG_DISPLAY] = 0;
    digits[LEFT_R_SEVEN_SEG_DISPLAY] = 0;

    char str[20];
    sprintf(str, "%f", value);
    int i = 0;

    // Turn the base-10 value into individual digits. Have to write backwards with how the
    // displays are initialized and how they are passed to the IO function.
    for (int digits_index = LEFT_L_SEVEN_SEG_DISPLAY; digits_index < NUM_IN_GROUP; digits_index ++)
    {
        if (str[i] == '.'){
            digits[digits_index-1] = App_SevenSegDisplays_AddDecimalIndicator((uint8_t)digits[digits_index-1]);
            i++;
        }
        digits[digits_index] = (uint8_t)(str[i] - '0');
        i++;
    }
    App_SevenSegDisplays_SetDigits(seven_seg_displays, LEFT_L_SEVEN_SEG_DISPLAY);
    return EXIT_CODE_OK;
}

ExitCode App_SevenSegDisplays_SetGroupM(const struct SevenSegDisplays *const seven_seg_displays, double value)
{
    if (value > pow(10, NUM_IN_GROUP) - 1)
    {
        return EXIT_CODE_INVALID_ARGS;
    }
    digits[MIDDLE_L_SEVEN_SEG_DISPLAY] = 0;
    digits[MIDDLE_M_SEVEN_SEG_DISPLAY] = 0;
    digits[MIDDLE_R_SEVEN_SEG_DISPLAY] = 0;

    char str[20];
    sprintf(str, "%f", value);
    int i = 0;

    // Turn the base-10 value into individual digits. Have to write backwards with how the
    // displays are initialized and how they are passed to the IO function
    for (int digits_index = MIDDLE_L_SEVEN_SEG_DISPLAY; digits_index < MIDDLE_L_SEVEN_SEG_DISPLAY+ NUM_IN_GROUP; digits_index++)
    {
        if (str[i] == '.'){ //with how the seven segs work, the the previous digit has to hold the decimal value
            digits[digits_index-1] = App_SevenSegDisplays_AddDecimalIndicator(digits[digits_index-1]);
            i++;
        }
        digits[digits_index] = (uint8_t)(str[i] - '0');
        i++;
    }
    App_SevenSegDisplays_SetDigits(seven_seg_displays, MIDDLE_L_SEVEN_SEG_DISPLAY);

    return EXIT_CODE_OK;
}

ExitCode App_SevenSegDisplays_SetGroupR(const struct SevenSegDisplays *const seven_seg_displays, double value)
{
    if (value > pow(10, NUM_IN_GROUP) - 1)
    {
        return EXIT_CODE_INVALID_ARGS;
    }

    // 'wiping' this section of the array

    digits[RIGHT_L_SEVEN_SEG_DISPLAY] = 0;
    digits[RIGHT_M_SEVEN_SEG_DISPLAY] = 0;
    digits[RIGHT_R_SEVEN_SEG_DISPLAY] = 0;

    char str[20];
    sprintf(str, "%f", value);
    int i = 0;

    // Turn the base-10 value into individual digits. Have to write backwards with how the
    // displays are initialized and how they are passed to the IO function
    for (int digits_index = RIGHT_L_SEVEN_SEG_DISPLAY; digits_index < RIGHT_L_SEVEN_SEG_DISPLAY + NUM_IN_GROUP; digits_index++)
    {
        if (str[i] == '.'){
            digits[digits_index-1] = App_SevenSegDisplays_AddDecimalIndicator((uint8_t)digits[digits_index]);
            i++;
        }
        digits[digits_index] = (uint8_t)(str[i] - '0');
        i++;
    }
    App_SevenSegDisplays_SetDigits(seven_seg_displays, RIGHT_L_SEVEN_SEG_DISPLAY);
    return EXIT_CODE_OK;
}