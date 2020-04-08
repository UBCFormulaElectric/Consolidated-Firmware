#include <stdlib.h>

#include "App_SevenSegDisplay.h"
#include "App_SharedAssert.h"

struct SevenSegDisplay
{
    void (*set_digit)(enum SevenSegDisplay_Digit digit);
    void (*set_letter)(enum SevenSegDisplay_Letter letter);
};

struct SevenSegDisplay *App_SevenSegDisplay_Create(
    void (*const set_digit)(enum SevenSegDisplay_Digit digit),
    void (*const set_letter)(enum SevenSegDisplay_Letter letter))
{
    struct SevenSegDisplay *seven_seg_display =
        malloc(sizeof(struct SevenSegDisplay));

    seven_seg_display->set_digit  = set_digit;
    seven_seg_display->set_letter = set_letter;

    return seven_seg_display;
}

void App_SevenSegDisplay_Destroy(
    struct SevenSegDisplay *const seven_seg_display)
{
    free(seven_seg_display);
}

void App_SevenSegDisplay_SetDigit(
    const struct SevenSegDisplay *const seven_seg_display,
    enum SevenSegDisplay_Digit          digit)
{
    shared_assert(digit < NUM_SEVEN_SEG_DISPLAY_DIGITS);
    seven_seg_display->set_digit(digit);
}

void App_SevenSegDisplay_SetLetter(
    const struct SevenSegDisplay *const seven_seg_display,
    enum SevenSegDisplay_Letter         letter)
{
    shared_assert(letter < NUM_SEVEN_SEG_DISPLAY_LETTERS);
    seven_seg_display->set_letter(letter);
}
