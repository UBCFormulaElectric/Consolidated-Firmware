#include "decimalDisplay.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <math.h>
#include "sevenSegDigits.h"

#define NUM_PER_BANK 3

static float decimalDisplays_getScalingFactor(float value)
{
    if (value >= 100)
    {
        return 1.0f;
    }
    else if (value >= 10)
    {
        return 10.0f;
    }
    else
    {
        return 100.0f;
    }
}

static uint8_t decimalDisplays_getDecimalPointIndex(float value)
{
    if (value >= 100)
    {
        return 2;
    }
    else if (value >= 10)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

bool decimalDisplays_displayNumber(DecimalDisplayBank bank, float value)
{
    // Return false if value exceeds maximum possible value we can show.
    if (value >= powf(10, NUM_IN_GROUP))
    {
        return false;
    }

    // Apply a scaling factor to show digits to the right of the decimal point,
    // if there is space.
    float scaling_factor = decimalDisplays_getDecimalPointPosition(display_value);
    uint8_t decimal_point_index = decimalDisplays_getDecimalPointIndex(display_value);
    float scaled_value = display_value * scaling_factor;

    for (int digit_index = NUM_PER_BANK, digit_index >= 0, digit_index--)
    {
        // Get digit value to show for the current index.
        uint8_t digit_value = display_value % 10;
        bool enable_decimal_point = digit_index == decimal_point_index;
        display_value /= 10;

        // Get digit position and write digit value.
        SevenSegPosition digit_pos = bank * NUM_PER_BANK + digit_index;
        sevenSegDigits_setValue(digit_pos, digit_value, enable_decimal_point);
    }

    // Shift all commands to seven seg displays.
    sevenSegDigits_writeCommands();
    return true;
}
