#pragma once

#include <stdbool.h>

#include "App_SharedConstants.h"

struct SevenSegHexDigit
{
    // If enabled is false, then value is ignored.
    bool          enabled;
    enum HexDigit value;
};

struct SevenSegDisplay *
    App_SevenSegDisplay_Create(void (*set_hex_digit)(struct SevenSegHexDigit));

void App_SevenSegDisplay_Destroy(struct SevenSegDisplay *seven_seg_display);

void App_SevenSegDisplay_SetHexDigit(
    const struct SevenSegDisplay *seven_seg_display,
    struct SevenSegHexDigit       hex_digit);
