#include "Io_SevenSegDisplay.h"
#include "App_SharedAssert.h"

void Io_SevenSegDisplay_SetDigit(enum SevenSegDisplay_Digit digit)
{
    shared_assert(digit < NUM_SEVEN_SEG_DISPLAY_DIGITS);

    // TODO: Fill in each case with a static function definition that
    // turns on the appropriate segments in the 7-segment display
    switch (digit)
    {
        case SEVEN_SEG_DISPLAY_0:
        {
        }
        break;
        case SEVEN_SEG_DISPLAY_1:
        {
        }
        break;
        case SEVEN_SEG_DISPLAY_2:
        {
        }
        break;
        case SEVEN_SEG_DISPLAY_3:
        {
        }
        break;
        case SEVEN_SEG_DISPLAY_4:
        {
        }
        break;
        case SEVEN_SEG_DISPLAY_5:
        {
        }
        break;
        case SEVEN_SEG_DISPLAY_6:
        {
        }
        break;
        case SEVEN_SEG_DISPLAY_7:
        {
        }
        break;
        case SEVEN_SEG_DISPLAY_8:
        {
        }
        break;
        case SEVEN_SEG_DISPLAY_9:
        {
        }
        break;
        default:
        {
        }
        break;
    }
}

void Io_SevenSegDisplay_SetLetter(enum SevenSegDisplay_Letter letter)
{
    shared_assert(letter < NUM_SEVEN_SEG_DISPLAY_LETTERS);

    // TODO: Fill in each case with a static function definition that
    // turns on the appropriate segments in the 7-segment display
    switch (letter)
    {
        case SEVEN_SEG_DISPLAY_A:
        {
        }
        break;
        case SEVEN_SEG_DISPLAY_B:
        {
        }
        break;
        case SEVEN_SEG_DISPLAY_C:
        {
        }
        break;
        case SEVEN_SEG_DISPLAY_D:
        {
        }
        break;
        case SEVEN_SEG_DISPLAY_E:
        {
        }
        break;
        case SEVEN_SEG_DISPLAY_F:
        {
        }
        break;
        default:
        {
        }
        break;
    }
}
