#include <assert.h>
#include <stdlib.h>
#include <stdint.h>
#include "App_AcceleratorPedals.h"

struct AcceleratorPedals
{
    bool (*is_primary_encoder_alarm_active)(void);
    bool (*is_secondary_encoder_alarm_active)(void);
    uint32_t (*get_primary_encoder_counter_value)(void);
    uint32_t (*get_secondary_encoder_counter_value)(void);
    void (*reset_primary_encoder_counter)(void);
    void (*reset_secondary_encoder_counter)(void);
};

/**
 * Get the pedal percentage, a value in [0, 100]
 * @param encoder_fully_pressed_value The value of the encoder counter when the
 * given accelerator pedal is fully pressed
 * @param encoder_counter_value The current counter value of the encoder for the
 * given accelerator pedal
 * @param reset_encoder_counter A function that can be called to reset the
 * counter value of the encoder for the given accelerator pedal to 0
 * @return The pedal percentage of the given accelerator pedal
 */
static float App_GetPedalPercentage(
    uint32_t encoder_fully_pressed_value,
    uint32_t encoder_reset_value,
    uint32_t encoder_counter_value,
    void (*reset_encoder_counter)(void));

static float App_GetPedalPercentage(
    uint32_t encoder_fully_pressed_value,
    uint32_t encoder_reset_value,
    uint32_t encoder_counter_value,
    void (*reset_encoder_counter)(void))
{
    const float low_end_deadzone_threshold = (float)encoder_fully_pressed_value * PERCENT_DEFLECTION;

    const float high_end_deadzone_threshold = (float)encoder_fully_pressed_value * (1.0f - PERCENT_DEFLECTION);

    if (encoder_counter_value > encoder_reset_value)
    {
        // If the accelerator pedal underflows, reset the corresponding
        // encoder's counter register and set the pedal percentage to 0.0%
        reset_encoder_counter();
        return MIN_ACCELERATOR_PEDAL_PRESS;
    }
    else if (encoder_counter_value <= low_end_deadzone_threshold)
    {
        // Set the pedal percentage to 0.0% when the pedal is within the
        // low end deadzone
        return MIN_ACCELERATOR_PEDAL_PRESS;
    }
    else if (encoder_counter_value >= high_end_deadzone_threshold)
    {
        // Set the mapped pedal percentage to 100% when the pedal is within the
        // high end deadzone
        return MAX_ACCELERATOR_PEDAL_PRESS;
    }
    // Pedal % = 100% * (encoder value - low end deadzone threshold) /
    // (high end deadzone threshold - low end deadzone threshold)
    return MAX_ACCELERATOR_PEDAL_PRESS * ((float)encoder_counter_value - low_end_deadzone_threshold) /
           (high_end_deadzone_threshold - low_end_deadzone_threshold);
}

struct AcceleratorPedals *App_AcceleratorPedals_Create(
    bool (*is_primary_encoder_alarm_active)(void),
    bool (*is_secondary_encoder_alarm_active)(void),
    uint32_t (*get_primary_encoder_counter_value)(void),
    uint32_t (*get_secondary_encoder_counter_value)(void),
    void (*reset_primary_encoder_counter)(void),
    void (*reset_secondary_encoder_counter)(void))
{
    struct AcceleratorPedals *accelerator_pedals = malloc(sizeof(struct AcceleratorPedals));
    assert(accelerator_pedals != NULL);

    accelerator_pedals->is_primary_encoder_alarm_active     = is_primary_encoder_alarm_active;
    accelerator_pedals->is_secondary_encoder_alarm_active   = is_secondary_encoder_alarm_active;
    accelerator_pedals->get_primary_encoder_counter_value   = get_primary_encoder_counter_value;
    accelerator_pedals->get_secondary_encoder_counter_value = get_secondary_encoder_counter_value;
    accelerator_pedals->reset_primary_encoder_counter       = reset_primary_encoder_counter;
    accelerator_pedals->reset_secondary_encoder_counter     = reset_secondary_encoder_counter;

    return accelerator_pedals;
}

void App_AcceleratorPedals_Destroy(struct AcceleratorPedals *accelerator_pedals)
{
    free(accelerator_pedals);
}

bool App_AcceleratorPedals_IsPrimaryEncoderAlarmActive(const struct AcceleratorPedals *const accelerator_pedals)
{
    return accelerator_pedals->is_primary_encoder_alarm_active();
}

bool App_AcceleratorPedals_IsSecondaryEncoderAlarmActive(const struct AcceleratorPedals *const accelerator_pedals)
{
    return accelerator_pedals->is_secondary_encoder_alarm_active();
}

float App_AcceleratorPedals_GetPrimaryPedalPercentage(const struct AcceleratorPedals *const accelerator_pedals)
{
    return App_GetPedalPercentage(
        PAPPS_ENCODER_FULLY_PRESSED_VALUE, PAPPS_ENCODER_RESET_VALUE,
        accelerator_pedals->get_primary_encoder_counter_value(), accelerator_pedals->reset_primary_encoder_counter);
}

float App_AcceleratorPedals_GetSecondaryPedalPercentage(const struct AcceleratorPedals *const accelerator_pedals)
{
    return App_GetPedalPercentage(
        SAPPS_ENCODER_FULLY_PRESSED_VALUE, SAPPS_ENCODER_RESET_VALUE,
        accelerator_pedals->get_secondary_encoder_counter_value(), accelerator_pedals->reset_secondary_encoder_counter);
}
