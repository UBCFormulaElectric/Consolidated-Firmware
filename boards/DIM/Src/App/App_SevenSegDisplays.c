#include <stdlib.h>

#include "App_SevenSegDisplays.h"
#include "App_SharedConstants.h"
#include "App_SharedAssert.h"

struct SevenSegDisplays
{
    float (*get_state_of_charge)(void);
    void (*set_hex_digit[NUM_SEVEN_SEG_DISPLAYS_DIGITS])(uint8_t);
};

struct SevenSegDisplays *App_SevenSegDisplays_Create(
    void (*const set_left_hex_digit)(uint8_t),
    void (*const set_middle_hex_digit)(uint8_t),
    void (*const set_right_hex_digit)(uint8_t))
{
    struct SevenSegDisplays *seven_seg_displays =
        malloc(sizeof(struct SevenSegDisplays));

    shared_assert(seven_seg_displays != NULL);

    seven_seg_displays->set_hex_digit[SEVEN_SEG_DISPLAYS_LEFT_HEX_DIGIT] =
        set_left_hex_digit;
    seven_seg_displays->set_hex_digit[SEVEN_SEG_DISPLAYS_MIDDLE_HEX_DIGIT] =
        set_middle_hex_digit;
    seven_seg_displays->set_hex_digit[SEVEN_SEG_DISPLAYS_RIGHT_HEX_DIGIT] =
        set_right_hex_digit;

    return seven_seg_displays;
}

void App_SevenSegDisplays_Destroy(
    struct SevenSegDisplays *const seven_seg_displays)
{
    free(seven_seg_displays);
}

void App_SevenSegDisplays_SetValue(
    const struct SevenSegDisplays *const seven_seg_displays,
    const uint8_t                        chars[],
    size_t                               num_chars)
{
    shared_assert(num_chars <= NUM_SEVEN_SEG_DISPLAYS_DIGITS);

    for (size_t i = 0; i < num_chars; i++)
    {
        shared_assert(chars[i] < NUM_HEX_DIGITS);
        seven_seg_displays->set_hex_digit[i](chars[i]);
    }
}
