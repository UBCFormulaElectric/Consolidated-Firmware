#include <assert.h>
#include <stdlib.h>
#include <stdint.h>
#include "App_AcceleratorPedal.h"

struct AcceleratorPedal
{
    bool (*is_encoder_alarm_active)(void);
    uint32_t (*get_encoder_counter_value)(void);
    void (*reset_encoder_counter)(void);

    uint32_t encoder_max_pressed_value;
};

struct AcceleratorPedal *App_AcceleratorPedal_Create(
    bool (*is_encoder_alarm_active)(void),
    void (*reset_encoder_counter)(void),
    uint32_t (*get_encoder_counter_value)(void),
    uint32_t encoder_max_pressed_value)
{
    struct AcceleratorPedal *accelerator_pedal =
        malloc(sizeof(struct AcceleratorPedal));
    assert(accelerator_pedal != NULL);

    accelerator_pedal->is_encoder_alarm_active   = is_encoder_alarm_active;
    accelerator_pedal->get_encoder_counter_value = get_encoder_counter_value;
    accelerator_pedal->reset_encoder_counter     = reset_encoder_counter;

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
    const uint32_t encoder_counter_value =
        accelerator_pedal->get_encoder_counter_value();
    const uint32_t encoder_max_pressed_value =
        accelerator_pedal->encoder_max_pressed_value;
    const float percent_deflection = 0.03f;

    const float low_end_deadzone_threshold =
        (float)encoder_max_pressed_value * percent_deflection;

    // TODO: Determine the high end deadzone threshold
    // Currently setting the high end deadzone threshold to be 3% less than
    // the maximum encoder value when the pedal is completely pressed
    const float high_end_deadzone_threshold =
        (float)encoder_max_pressed_value * (1.0f - percent_deflection);

    if (encoder_counter_value > encoder_max_pressed_value)
    {
        // If the accelerator pedal underflows, reset the corresponding
        // encoder's counter register and set the pedal percentage to 0.0%
        accelerator_pedal->reset_encoder_counter();
        return 0.0f;
    }
    else if (encoder_counter_value <= low_end_deadzone_threshold)
    {
        // Set the pedal percentage to 0.0% when the pedal is within the
        // low end deadzone
        return 0.0f;
    }
    else if (
        encoder_counter_value <= encoder_max_pressed_value &&
        encoder_counter_value >= high_end_deadzone_threshold)
    {
        // Set the mapped pedal percentage to 100% when the pedal is within the
        // high end deadzone
        return 100.0f;
    }

    // Pedal % = 100% * (encoder value - low end deadzone threshold) /
    // (high end deadzone threshold - low end deadzone threshold)
    return 100.0f *
           ((float)encoder_counter_value - low_end_deadzone_threshold) /
           (high_end_deadzone_threshold - low_end_deadzone_threshold);
}
