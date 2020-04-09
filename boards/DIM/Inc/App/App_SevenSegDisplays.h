#pragma once

#include <stdint.h>

struct SevenSegDisplay;

// Do NOT take ownership of individual displays
struct SevenSegDisplays *App_SevenSegDisplays_Create(
    float (*const get_state_of_charge)(void),
    void (*set_left_hex_digit)(uint8_t),
    void (*set_middle_hex_digit)(uint8_t),
    void (*set_right_hex_digit)(uint8_t));

void App_SevenSegDisplays_Destroy(struct SevenSegDisplays *seven_seg_displays);

void App_SevenSegDisplays_WriteStateOfCharge(
    const struct SevenSegDisplays *seven_seg_displays);
