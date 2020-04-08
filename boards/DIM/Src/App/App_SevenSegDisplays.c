#include <stdlib.h>

#include "App_SevenSegDisplays.h"
#include "App_SharedAssert.h"

struct SevenSegDisplays
{
    struct SevenSegDisplay *right_display;
    struct SevenSegDisplay *middle_display;
    struct SevenSegDisplay *left_display;
};

// Do NOT take ownership of individual displays
struct SevenSegDisplays *App_SevenSegDisplays_Create(
    struct SevenSegDisplay *right_display,
    struct SevenSegDisplay *middle_display,
    struct SevenSegDisplay *left_display)
{
    struct SevenSegDisplays *seven_seg_displays =
        malloc(sizeof(struct SevenSegDisplays));

    seven_seg_displays->right_display  = right_display;
    seven_seg_displays->middle_display = middle_display;
    seven_seg_displays->left_display   = left_display;

    return seven_seg_displays;
}

void App_SevenSegDisplays_Destroy(struct SevenSegDisplays *seven_seg_displays)
{
    free(seven_seg_displays);
}

void App_SevenSegDisplays_WriteStateOfCharge(
    struct SevenSegDisplays *seven_seg_displays,
    float                    state_of_charge)
{
    // Truncate the fractional bits of floating point
    uint32_t state_of_charge_u32 = (uint32_t)state_of_charge;

    shared_assert(state_of_charge_u32 <= 100U);

    uint8_t right_digit  = state_of_charge_u32 % 10U;
    uint8_t middle_digit = (state_of_charge_u32 / 10U) % 10U;
    uint8_t left_digit   = (state_of_charge_u32 / 100U) % 10U;

    App_SevenSegDisplay_SetDigit(
        seven_seg_displays->right_display, right_digit);
    App_SevenSegDisplay_SetDigit(
        seven_seg_displays->middle_display, middle_digit);
    App_SevenSegDisplay_SetDigit(seven_seg_displays->left_display, left_digit);
}
