#include <assert.h>
#include <stdlib.h>
#include <stdint.h>
#include "App_AcceleratorPedal.h"

struct AcceleratorPedal
{
    bool (*is_encoder_alarm_active)(void);
    uint32_t (*get_encoder_count_value)(void);
    void (*reset_encoder_counter)(void);

    uint32_t encoder_resolution;
    uint32_t encoder_max_pressed_value;
};

struct AcceleratorPedal *App_AcceleratorPedal_Create(
    bool (*is_encoder_alarm_active)(void),
    void (*reset_encoder_counter)(void),
    uint32_t (*get_encoder_count_value)(void),
    uint32_t encoder_resolution,
    uint32_t encoder_max_pressed_value)
{
    struct AcceleratorPedal *accelerator_pedal =
        malloc(sizeof(struct AcceleratorPedal));
    assert(accelerator_pedal != NULL);

    accelerator_pedal->is_encoder_alarm_active = is_encoder_alarm_active;
    accelerator_pedal->get_encoder_count_value = get_encoder_count_value;
    accelerator_pedal->reset_encoder_counter   = reset_encoder_counter;

    accelerator_pedal->encoder_resolution        = encoder_resolution;
    accelerator_pedal->encoder_max_pressed_value = encoder_max_pressed_value;

    return accelerator_pedal;
}

void App_AcceleratorPedal_Destroy(struct AcceleratorPedal *accelerator_pedal)
{
    free(accelerator_pedal);
}

bool App_AcceleratorPedal_IsEncoderAlarmActive(
    const struct AcceleratorPedal *accelerator_pedal)
{
    return accelerator_pedal->is_encoder_alarm_active();
}

float App_AcceleratorPedal_GetPedalPercentage(
    const struct AcceleratorPedal *const accelerator_pedal)
{
    const uint32_t encoder_count_value =
        accelerator_pedal->get_encoder_count_value();
    const uint32_t encoder_max_pressed_value =
        accelerator_pedal->encoder_max_pressed_value;
    const float percent_deflection = 0.03f;

    // Calculate the upper threshold for the low end dead-zone and the lower
    // threshold for the high end dead-zone
    const float low_end_deadzone_upper_threshold =
        (float)encoder_max_pressed_value * percent_deflection;
    const float high_end_deadzone_lower_threshold =
        (float)encoder_max_pressed_value * (1.0f - percent_deflection);

    if (encoder_count_value <= accelerator_pedal->encoder_resolution &&
        encoder_count_value > encoder_max_pressed_value)
    {
        // If the accelerator pedal underflows reset the counter register and
        // set the pedal percentage to 0.0%
        accelerator_pedal->reset_encoder_counter();
        return 0.0f;
    }
    else if (
        (encoder_count_value <= low_end_deadzone_upper_threshold) ||
        (encoder_count_value > encoder_max_pressed_value))
    {
        // Set the pedal percentage to 0.0% when
        // (1) The pedal is within the depressed dead-zone
        // (2) The pedal exceeds the encoder's maximum pressed value
        return 0.0f;
    }
    else if (
        encoder_count_value <= encoder_max_pressed_value &&
        encoder_count_value >= high_end_deadzone_lower_threshold)
    {
        // Set the pedal percentage to 100% when the pedal is within the pressed
        // dead-zone
        return 100.0f;
    }

    return 100.0f *
           ((float)encoder_count_value - low_end_deadzone_upper_threshold) /
           (high_end_deadzone_lower_threshold -
            low_end_deadzone_upper_threshold);
}
