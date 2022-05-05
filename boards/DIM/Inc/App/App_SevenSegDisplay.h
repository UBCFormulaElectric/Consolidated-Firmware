#pragma once

#include <stdbool.h>
#include "App_SharedConstants.h"

struct SevenSegHexDigit
{
    // If "enabled" is false, then the 7-segment display is turned off instead
    // of displaying "value".
    bool          enabled;
    enum HexDigit value;
};

/**
 * Allocate and initialize a 7-segment display
 * @param set_hex_digit A function that can be called to set the underlying
 *                      hardware to display a given hexadecimal digit
 * @return A pointer to the created 7-segment display, whose ownership is given
 *         to the caller
 */
struct SevenSegDisplay *App_SevenSegDisplay_Create(void (*set_hex_digit)(struct SevenSegHexDigit));

/**
 * Deallocate the memory used by the given 7-segment display
 * @param seven_seg_display The 7-segment display to deallocate
 */
void App_SevenSegDisplay_Destroy(struct SevenSegDisplay *seven_seg_display);

/**
 * Set the hexadecimal digit for the given 7-segment display
 * @param seven_seg_display The 7-segment display to set hexadecimal digit for
 * @param hex_digit The hexadecimal digit to set
 */
void App_SevenSegDisplay_SetHexDigit(
    const struct SevenSegDisplay *seven_seg_display,
    struct SevenSegHexDigit       hex_digit);
