#include "app_sevenSegDisplays.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include "io_sevenSegDisplays.h"

static float getScalingFactor(float value)
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

static SevenSegSubposition getDecimalPointSubposition(float value)
{
    if (value >= 100)
    {
        return SEVEN_SEG_SUBPOSITION_R;
    }
    else if (value >= 10)
    {
        return SEVEN_SEG_SUBPOSITION_M;
    }
    else
    {
        return SEVEN_SEG_SUBPOSITION_L;
    }
}

void app_sevenSegDisplays_init(void)
{
    // Set all displays to show zero.
    app_sevenSegDisplays_setGroup(SEVEN_SEG_GROUP_L, 0.0f);
    app_sevenSegDisplays_setGroup(SEVEN_SEG_GROUP_M, 0.0f);
    app_sevenSegDisplays_setGroup(SEVEN_SEG_GROUP_R, 0.0f);
}

bool app_sevenSegDisplays_setGroup(SevenSegGroup group, float value)
{
    // Return false if value exceeds maximum possible value we can show.
    if (value >= powf(10, NUM_SEVEN_SEG_SUBPOSITIONS))
    {
        return false;
    }

    // Apply a scaling factor to show digits to the right of the decimal point,
    // if there is space.
    float               scaling_factor      = getScalingFactor(value);
    SevenSegSubposition decimal_subposition = getDecimalPointSubposition(value);
    int                 scaled_value        = (int)(value * scaling_factor);

    for (int digit_subposition = NUM_SEVEN_SEG_SUBPOSITIONS; digit_subposition >= 0; digit_subposition--)
    {
        // Get digit value to show for the current index.
        int  digit_value          = scaled_value % 10;
        bool enable_decimal_point = digit_subposition == (int)decimal_subposition;
        value /= 10;

        // Write digit value.
        io_sevenSegDisplays_setValue(
            group, (SevenSegSubposition)digit_subposition, (HexDigit)digit_value, enable_decimal_point);
    }

    // Shift out all commands to 7-segs.
    io_sevenSegDisplays_writeCommands();
    return true;
}
