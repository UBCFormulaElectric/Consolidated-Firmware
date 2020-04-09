#include <stdlib.h>

#include "App_SevenSegDisplays.h"
#include "App_SharedAssert.h"

struct SevenSegDisplays
{
    float (*get_state_of_charge)(void);
    void (*set_left_hex_digit)(uint8_t);
    void (*set_middle_hex_digit)(uint8_t);
    void (*set_right_hex_digit)(uint8_t);
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

    seven_seg_displays->set_left_hex_digit(left_digit);
    seven_seg_displays->set_middle_hex_digit(middle_digit);
    seven_seg_displays->set_right_hex_digit(right_digit);
}

struct SevenSegDisplays *App_SevenSegDisplays_Create(
    float (*const get_state_of_charge)(void),
    void (*set_left_hex_digit)(uint8_t),
    void (*set_middle_hex_digit)(uint8_t),
    void (*set_right_hex_digit)(uint8_t))

{
    struct SevenSegDisplays *seven_seg_displays =
        malloc(sizeof(struct SevenSegDisplays));

    shared_assert(seven_seg_displays != NULL);

    seven_seg_displays->get_state_of_charge = get_state_of_charge;
    seven_seg_displays->set_left_hex_digit = set_left_hex_digit;
    seven_seg_displays->set_middle_hex_digit = set_middle_hex_digit;
    seven_seg_displays->set_right_hex_digit = set_right_hex_digit;

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
