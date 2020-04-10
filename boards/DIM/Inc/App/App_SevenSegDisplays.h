#pragma once

#include <stdint.h>

struct SevenSegDisplay;

enum SEVEN_SEG_DISPLAYS_DIGIT
{
    SEVEN_SEG_DISPLAYS_LEFT_HEX_DIGIT,
    SEVEN_SEG_DISPLAYS_MIDDLE_HEX_DIGIT,
    SEVEN_SEG_DISPLAYS_RIGHT_HEX_DIGIT,
    NUM_SEVEN_SEG_DISPLAYS_DIGITS,
};

// Do NOT take ownership of individual displays
struct SevenSegDisplays *App_SevenSegDisplays_Create(
    void (*set_left_hex_digit)(uint8_t),
    void (*set_middle_hex_digit)(uint8_t),
    void (*set_right_hex_digit)(uint8_t));

void App_SevenSegDisplays_Destroy(struct SevenSegDisplays *seven_seg_displays);

void App_SevenSegDisplays_SetValue(
    const struct SevenSegDisplays *seven_seg_displays,
    const uint8_t                  chars[],
    size_t                         num_chars);
