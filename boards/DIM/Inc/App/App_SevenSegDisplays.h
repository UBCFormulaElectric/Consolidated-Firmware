#pragma once

struct SevenSegDisplay;

// Do NOT take ownership of individual displays
struct SevenSegDisplays *App_SevenSegDisplays_Create(
    float (*get_state_of_charge)(void),
    struct SevenSegDisplay *left_display,
    struct SevenSegDisplay *middle_display,
    struct SevenSegDisplay *right_display);

void App_SevenSegDisplays_Destroy(struct SevenSegDisplays *seven_seg_displays);

void App_SevenSegDisplays_WriteStateOfCharge(
    const struct SevenSegDisplays *seven_seg_displays);
