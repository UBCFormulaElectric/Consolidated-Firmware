#include <stdlib.h>
#include <assert.h>

#include "App_SevenSegDisplay.h"

struct SevenSegDisplay
{
    void (*set_hex_digit)(struct SevenSegHexDigit);
};

struct SevenSegDisplay *App_SevenSegDisplay_Create(void (*const set_hex_digit)(struct SevenSegHexDigit))
{
    struct SevenSegDisplay *seven_seg_display = malloc(sizeof(struct SevenSegDisplay));

    assert(seven_seg_display != NULL);

    seven_seg_display->set_hex_digit = set_hex_digit;

    return seven_seg_display;
}

void App_SevenSegDisplay_Destroy(struct SevenSegDisplay *const seven_seg_display)
{
    free(seven_seg_display);
}

void App_SevenSegDisplay_SetHexDigit(
    const struct SevenSegDisplay *const seven_seg_display,
    const struct SevenSegHexDigit       hex_digit)
{
    seven_seg_display->set_hex_digit(hex_digit);
}
