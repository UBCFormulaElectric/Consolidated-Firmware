#pragma once

#include <stdint.h>

struct SevenSegDisplay *
    App_SevenSegDisplay_Create(void (*set_hex_digit)(uint8_t hex_digit));

void App_SevenSegDisplay_Destroy(struct SevenSegDisplay *seven_seg_display);

void App_SevenSegDisplay_SetHexDigit(
    const struct SevenSegDisplay *seven_seg_display,
    uint8_t                       hex_digit);
