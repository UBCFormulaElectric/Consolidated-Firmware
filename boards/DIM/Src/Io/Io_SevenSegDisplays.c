#include "App_SharedAssert.h"
#include "App_SharedConstants.h"

#include "Io_SevenSegDisplays.h"

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

float Io_SevenSegDisplay_GetStateOfCharge(void)
{
    // TODO: Read the state of charge from CAN
    return 0.0f;
}

void Io_SevenSegDisplay_SetLeftHexDigit(uint8_t hex_digit)
{
    shared_assert(hex_digit < NUM_HEX_DIGITS);

    // TODO: Fill in each case with a static function definition that
    // turns on the appropriate segments in the 7-segment display
    switch (hex_digit)
    {
        case HEX_DIGIT_0:
        {
        }
        break;
        case HEX_DIGIT_1:
        {
        }
        break;
        case HEX_DIGIT_2:
        {
        }
        break;
        case HEX_DIGIT_3:
        {
        }
        break;
        case HEX_DIGIT_4:
        {
        }
        break;
        case HEX_DIGIT_5:
        {
        }
        break;
        case HEX_DIGIT_6:
        {
        }
        break;
        case HEX_DIGIT_7:
        {
        }
        break;
        case HEX_DIGIT_8:
        {
        }
        break;
        case HEX_DIGIT_9:
        {
        }
        break;
        case HEX_DIGIT_A:
        {
        }
        break;
        case HEX_DIGIT_B:
        {
        }
        break;
        case HEX_DIGIT_C:
        {
        }
        break;
        case HEX_DIGIT_D:
        {
        }
        break;
        case HEX_DIGIT_E:
        {
        }
        break;
        case HEX_DIGIT_F:
        {
        }
        break;
        default:
        {
        }
        break;
    }
}

void Io_SevenSegDisplay_SetMiddleHexDigit(uint8_t hex_digit)
{
    shared_assert(hex_digit < NUM_HEX_DIGITS);

    // TODO: Fill in each case with a static function definition that
    // turns on the appropriate segments in the 7-segment display
    switch (hex_digit)
    {
        case HEX_DIGIT_0:
        {
        }
        break;
        case HEX_DIGIT_1:
        {
        }
        break;
        case HEX_DIGIT_2:
        {
        }
        break;
        case HEX_DIGIT_3:
        {
        }
        break;
        case HEX_DIGIT_4:
        {
        }
        break;
        case HEX_DIGIT_5:
        {
        }
        break;
        case HEX_DIGIT_6:
        {
        }
        break;
        case HEX_DIGIT_7:
        {
        }
        break;
        case HEX_DIGIT_8:
        {
        }
        break;
        case HEX_DIGIT_9:
        {
        }
        break;
        case HEX_DIGIT_A:
        {
        }
        break;
        case HEX_DIGIT_B:
        {
        }
        break;
        case HEX_DIGIT_C:
        {
        }
        break;
        case HEX_DIGIT_D:
        {
        }
        break;
        case HEX_DIGIT_E:
        {
        }
        break;
        case HEX_DIGIT_F:
        {
        }
        break;
        default:
        {
        }
        break;
    }
}

void Io_SevenSegDisplay_SetRightHexDigit(uint8_t hex_digit)
{
    shared_assert(hex_digit < NUM_HEX_DIGITS);

    // TODO: Fill in each case with a static function definition that
    // turns on the appropriate segments in the 7-segment display
    switch (hex_digit)
    {
        case HEX_DIGIT_0:
        {
        }
        break;
        case HEX_DIGIT_1:
        {
        }
        break;
        case HEX_DIGIT_2:
        {
        }
        break;
        case HEX_DIGIT_3:
        {
        }
        break;
        case HEX_DIGIT_4:
        {
        }
        break;
        case HEX_DIGIT_5:
        {
        }
        break;
        case HEX_DIGIT_6:
        {
        }
        break;
        case HEX_DIGIT_7:
        {
        }
        break;
        case HEX_DIGIT_8:
        {
        }
        break;
        case HEX_DIGIT_9:
        {
        }
        break;
        case HEX_DIGIT_A:
        {
        }
        break;
        case HEX_DIGIT_B:
        {
        }
        break;
        case HEX_DIGIT_C:
        {
        }
        break;
        case HEX_DIGIT_D:
        {
        }
        break;
        case HEX_DIGIT_E:
        {
        }
        break;
        case HEX_DIGIT_F:
        {
        }
        break;
        default:
        {
        }
        break;
    }
}
