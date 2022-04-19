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
    void (*set_primary_encoder_counter)(uint32_t val);
    void (*set_secondary_encoder_counter)(uint32_t val);
};

/**
 * Get the pedal percentage, a value in [0, 100], for an encoder configured to
 * count up
 * @param encoder_fully_pressed_value The value of the encoder counter when the
 * given accelerator pedal is fully pressed
 * @param encoder_counter_value The current counter value of the encoder for the
 * given accelerator pedal
 * @param set_encoder_counter A function that can be called to set the
 * counter value of the encoder for the given accelerator pedal
 * @return The pedal percentage of the given accelerator pedal
 */
static float App_GetPedalPercentage_CountUp(
    uint32_t encoder_fully_pressed_value,
    uint32_t encoder_unpressed_value,
    uint32_t encoder_reset_value,
    uint32_t encoder_counter_value,
    void (*set_encoder_counter)(uint32_t));

/**
 * Get the pedal percentage, a value in [0, 100], for an encoder configured to
 * count down
 * @param encoder_fully_pressed_value The value of the encoder counter when the
 * given accelerator pedal is fully pressed
 * @param encoder_counter_value The current counter value of the encoder for the
 * given accelerator pedal
 * @param set_encoder_counter A function that can be called to set the
 * counter value of the encoder for the given accelerator pedal
 * @return The pedal percentage of the given accelerator pedal
 */
static float App_GetPedalPercentage_CountDown(
    uint32_t encoder_fully_pressed_value,
    uint32_t encoder_unpressed_value,
    uint32_t encoder_counter_value,
    void (*set_encoder_counter)(uint32_t));

static float App_GetPedalPercentage_CountUp(
    uint32_t encoder_fully_pressed_value,
    uint32_t encoder_unpressed_value,
    uint32_t encoder_reset_value,
    uint32_t encoder_counter_value,
    void (*set_encoder_counter)(uint32_t))
{
    float low_count_deadzone_threshold =
        (float)(encoder_fully_pressed_value - encoder_unpressed_value) *
            PERCENT_DEFLECTION +
        (float)encoder_unpressed_value;

    float high_count_deadzone_threshold =
        (float)(encoder_fully_pressed_value - encoder_unpressed_value) *
            (1.0f - PERCENT_DEFLECTION) +
        (float)encoder_unpressed_value;

    if (encoder_counter_value > encoder_reset_value)
    {
        // If the accelerator pedal underflows, reset the corresponding
        // encoder's counter register and set the pedal percentage to
        set_encoder_counter(encoder_unpressed_value);
        return MIN_ACCELERATOR_PEDAL_PRESS;
    }
    else if (encoder_counter_value <= low_count_deadzone_threshold)
    {
        // Set the pedal percentage to 0.0% when the pedal is within the
        // low end deadzone
        return MIN_ACCELERATOR_PEDAL_PRESS;
    }
    else if (encoder_counter_value >= high_count_deadzone_threshold)
    {
        // Set the mapped pedal percentage to 100% when the pedal is within the
        // high end deadzone
        return MAX_ACCELERATOR_PEDAL_PRESS;
    }

    // Pedal % = 100% * (encoder value - low end deadzone threshold) /
    // (high end deadzone threshold - low end deadzone threshold)
    return MAX_ACCELERATOR_PEDAL_PRESS *
           ((float)encoder_counter_value - low_count_deadzone_threshold) /
           (high_count_deadzone_threshold - low_count_deadzone_threshold);
}

static float App_GetPedalPercentage_CountDown(
    uint32_t encoder_fully_pressed_value,
    uint32_t encoder_unpressed_value,
    uint32_t encoder_counter_value,
    void (*set_encoder_counter)(uint32_t))
{
    float low_count_deadzone_threshold =
        (float)(encoder_unpressed_value - encoder_fully_pressed_value) *
            PERCENT_DEFLECTION +
        (float)encoder_fully_pressed_value;

    float high_count_deadzone_threshold =
        (float)(encoder_unpressed_value - encoder_fully_pressed_value) *
            (1.0f - PERCENT_DEFLECTION) +
        (float)encoder_fully_pressed_value;

    if (encoder_counter_value > encoder_unpressed_value)
    {
        // For down counting, when the encoder unpressed value is exceeded,
        // reset the encoder to the unpressed value and set 0% pedal press
        set_encoder_counter(encoder_unpressed_value);
        return MIN_ACCELERATOR_PEDAL_PRESS;
    }
    else if (encoder_counter_value >= high_count_deadzone_threshold)
    {
        // For down count, set mapped pedal percentage to 0% when within
        // high end deadzone
        return MIN_ACCELERATOR_PEDAL_PRESS;
    }
    else if (encoder_counter_value <= low_count_deadzone_threshold)
    {
        // For down count, set mapped pedal percentage to 100% when within the
        // low end deadzone
        return MAX_ACCELERATOR_PEDAL_PRESS;
    }

    // Pedal % = 100% * (high end deadzone threshold - encoder value) /
    // (high end deadzone threshold - low end deadzone threshold)
    return MAX_ACCELERATOR_PEDAL_PRESS *
           (high_count_deadzone_threshold - (float)encoder_counter_value) /
           (high_count_deadzone_threshold - low_count_deadzone_threshold);
}

struct AcceleratorPedals *App_AcceleratorPedals_Create(
    bool (*is_primary_encoder_alarm_active)(void),
    bool (*is_secondary_encoder_alarm_active)(void),
    uint32_t (*get_primary_encoder_counter_value)(void),
    uint32_t (*get_secondary_encoder_counter_value)(void),
    void (*set_primary_encoder_counter)(uint32_t),
    void (*set_secondary_encoder_counter)(uint32_t))
{
    struct AcceleratorPedals *accelerator_pedals =
        malloc(sizeof(struct AcceleratorPedals));
    assert(accelerator_pedals != NULL);

    accelerator_pedals->is_primary_encoder_alarm_active =
        is_primary_encoder_alarm_active;
    accelerator_pedals->is_secondary_encoder_alarm_active =
        is_secondary_encoder_alarm_active;
    accelerator_pedals->get_primary_encoder_counter_value =
        get_primary_encoder_counter_value;
    accelerator_pedals->get_secondary_encoder_counter_value =
        get_secondary_encoder_counter_value;
    accelerator_pedals->set_primary_encoder_counter =
        set_primary_encoder_counter;
    accelerator_pedals->set_secondary_encoder_counter =
        set_secondary_encoder_counter;

    return accelerator_pedals;
}

void App_AcceleratorPedals_Destroy(struct AcceleratorPedals *accelerator_pedals)
{
    free(accelerator_pedals);
}

bool App_AcceleratorPedals_IsPrimaryEncoderAlarmActive(
    const struct AcceleratorPedals *const accelerator_pedals)
{
    return accelerator_pedals->is_primary_encoder_alarm_active();
}

bool App_AcceleratorPedals_IsSecondaryEncoderAlarmActive(
    const struct AcceleratorPedals *const accelerator_pedals)
{
    return accelerator_pedals->is_secondary_encoder_alarm_active();
}

void App_AcceleratorPedals_ResetAcceleratorPedalToUnpressed(
    const struct AcceleratorPedals *const accelerator_pedals)
{
    accelerator_pedals->set_primary_encoder_counter(
        PAPPS_ENCODER_UNPRESSED_VALUE);
    accelerator_pedals->set_secondary_encoder_counter(
        SAPPS_ENCODER_UNPRESSED_VALUE);
}

float App_AcceleratorPedals_GetPrimaryPedalPercentage(
    const struct AcceleratorPedals *const accelerator_pedals)
{
    return App_GetPedalPercentage_CountDown(
        PAPPS_ENCODER_FULLY_PRESSED_VALUE, PAPPS_ENCODER_UNPRESSED_VALUE,
        accelerator_pedals->get_primary_encoder_counter_value(),
        accelerator_pedals->set_primary_encoder_counter);
}

float App_AcceleratorPedals_GetSecondaryPedalPercentage(
    const struct AcceleratorPedals *const accelerator_pedals)
{
    return App_GetPedalPercentage_CountUp(
        SAPPS_ENCODER_FULLY_PRESSED_VALUE, SAPPS_ENCODER_UNPRESSED_VALUE,
        SAPPS_ENCODER_RESET_VALUE,
        accelerator_pedals->get_secondary_encoder_counter_value(),
        accelerator_pedals->set_secondary_encoder_counter);
}
