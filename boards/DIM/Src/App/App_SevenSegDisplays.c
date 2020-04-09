#include <stdlib.h>

#include "App_SevenSegDisplays.h"
#include "App_SevenSegDisplay.h"
#include "App_SharedAssert.h"

struct SevenSegDisplays
{
    float (*get_state_of_charge)(void);
    struct SevenSegDisplay *left_display;
    struct SevenSegDisplay *middle_display;
    struct SevenSegDisplay *right_display;
};

static void WriteStateOfCharge(
    const struct SevenSegDisplays *seven_seg_displays,
    float                          state_of_charge);

static void WriteStateOfCharge(
    const struct SevenSegDisplays *const seven_seg_displays,
    float                                state_of_charge)
{
    // Truncate the fractional bits of floating point
    uint32_t state_of_charge_u32 = (uint32_t)state_of_charge;

    shared_assert(state_of_charge_u32 <= 100U);

    uint8_t left_digit   = (state_of_charge_u32 / 100U) % 10U;
    uint8_t middle_digit = (state_of_charge_u32 / 10U) % 10U;
    uint8_t right_digit  = state_of_charge_u32 % 10U;

    App_SevenSegDisplay_SetHexDigit(
        seven_seg_displays->left_display, left_digit);
    App_SevenSegDisplay_SetHexDigit(
        seven_seg_displays->middle_display, middle_digit);
    App_SevenSegDisplay_SetHexDigit(
        seven_seg_displays->right_display, right_digit);
}

struct SevenSegDisplays *App_SevenSegDisplays_Create(
    float (*const get_state_of_charge)(void),
    struct SevenSegDisplay *const left_display,
    struct SevenSegDisplay *const middle_display,
    struct SevenSegDisplay *const right_display)
{
    struct SevenSegDisplays *seven_seg_displays =
        malloc(sizeof(struct SevenSegDisplays));

    seven_seg_displays->get_state_of_charge = get_state_of_charge;
    seven_seg_displays->left_display        = left_display;
    seven_seg_displays->middle_display      = middle_display;
    seven_seg_displays->right_display       = right_display;

    return seven_seg_displays;
}

void App_SevenSegDisplays_Destroy(
    struct SevenSegDisplays *const seven_seg_displays)
{
    free(seven_seg_displays);
}

void App_SevenSegDisplays_WriteStateOfCharge(
    const struct SevenSegDisplays *const seven_seg_displays)
{
    WriteStateOfCharge(
        seven_seg_displays, seven_seg_displays->get_state_of_charge());
}
