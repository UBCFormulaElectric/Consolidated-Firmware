#pragma once

#include <stdint.h>

#include "App_ErrorCode.h"

struct SevenSegDisplay;

enum
{
    LEFT_SEVEN_SEG_DISPLAY,
    MIDDLE_SEVEN_SEG_DISPLAY,
    RIGHT_SEVEN_SEG_DISPLAY,
    NUM_SEVEN_SEG_DISPLAYS,
};

// Do NOT take ownership of individual displays
struct SevenSegDisplays *App_SevenSegDisplays_Create(
    struct SevenSegDisplay *left_seven_seg_display,
    struct SevenSegDisplay *middle_seven_seg_display,
    struct SevenSegDisplay *right_seven_seg_display);

void App_SevenSegDisplays_Destroy(struct SevenSegDisplays *seven_seg_displays);

ErrorCode App_SevenSegDisplays_SetHexDigits(
    const struct SevenSegDisplays *seven_seg_displays,
    const uint8_t                  hex_digits[],
    size_t                         num_hex_digits);
