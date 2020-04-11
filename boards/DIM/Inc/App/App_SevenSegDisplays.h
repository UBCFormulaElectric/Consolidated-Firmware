#pragma once

#include <stdint.h>

struct SevenSegDisplay;

enum SevenSegDisplays_Digit
{
    SEVEN_SEG_DISPLAYS_LEFT_HEX_DIGIT,
    SEVEN_SEG_DISPLAYS_MIDDLE_HEX_DIGIT,
    SEVEN_SEG_DISPLAYS_RIGHT_HEX_DIGIT,
    NUM_SEVEN_SEG_DISPLAYS_DIGITS,
};

// Do NOT take ownership of individual displays
struct SevenSegDisplays *App_SevenSegDisplays_Create(
    struct SevenSegDisplay *left_seven_seg_display,
    struct SevenSegDisplay *middle_seven_seg_display,
    struct SevenSegDisplay *right_seven_seg_display);

void App_SevenSegDisplays_Destroy(struct SevenSegDisplays *seven_seg_displays);

void App_SevenSegDisplays_SetHexDigits(
    const struct SevenSegDisplays *seven_seg_displays,
    const uint8_t                  hex_digits[],
    size_t                         num_hex_digits);
