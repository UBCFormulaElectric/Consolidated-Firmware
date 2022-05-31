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
 * Get the pedal percentage, a value in [0, 100], for an encoder configured to count up
 * @param fully_pressed_value The value of the encoder counter when the given accelerator pedal is fully pressed
 * @param encoder_value The current counter value of the encoder for the given accelerator pedal
 * @param set_encoder_counter A function that can be called to set the
 * counter value of the encoder for the given accelerator pedal
 * @return The pedal percentage of the given accelerator pedal
 */
static float App_GetPedalPercentage_CountUp(
    uint32_t fully_pressed_value,
    uint32_t unpressed_value,
    uint32_t underflow_threshold,
    uint32_t encoder_value,
    void (*set_encoder_counter)(uint32_t));

/**
 * Get the pedal percentage, a value in [0, 100], for an encoder configured to count down
 * @param fully_pressed_value The value of the encoder counter when the given accelerator pedal is fully pressed
 * @param encoder_value The current counter value of the encoder for the given accelerator pedal
 * @param set_encoder_counter A function that can be called to set the
 * counter value of the encoder for the given accelerator pedal
 * @return The pedal percentage of the given accelerator pedal
 */
static float App_GetPedalPercentage_CountDown(
    uint32_t fully_pressed_value,
    uint32_t unpressed_value,
    uint32_t encoder_value,
    void (*set_encoder_counter)(uint32_t));

static float App_GetPedalPercentage_CountUp(
    uint32_t fully_pressed_value,
    uint32_t unpressed_value,
    uint32_t underflow_threshold,
    uint32_t encoder_value,
    void (*set_encoder_counter)(uint32_t))
{
    const float encoder_relative_range = (float)(fully_pressed_value - unpressed_value);

    // Add a deadzone for both lower and upper encoder ranges
    const float encoder_range_deadzone = encoder_relative_range * PERCENT_DEFLECTION;
    float       lower_deadzone_thresh  = encoder_range_deadzone + (float)unpressed_value;
    float       upper_deadzone_thresh  = (encoder_relative_range - encoder_range_deadzone) + (float)unpressed_value;
    float       pedal_pct              = 0.0f;

    if (encoder_value > underflow_threshold)
    {
        // If the accelerator pedal underflows (if counter exceeds a threshold only possible to be reached via wrapping
        // on underflow), reset the encoder's counter register
        set_encoder_counter(unpressed_value);
    }
    else if (encoder_value <= lower_deadzone_thresh)
    {
        pedal_pct = MIN_ACCELERATOR_PEDAL_PRESS;
    }
    else if (encoder_value >= upper_deadzone_thresh)
    {
        // Set the mapped pedal percentage to 100% when the pedal is within the high end deadzone
        pedal_pct = MAX_ACCELERATOR_PEDAL_PRESS;
    }
    else
    {
        pedal_pct = MAX_ACCELERATOR_PEDAL_PRESS * ((float)encoder_value - lower_deadzone_thresh) /
                    (upper_deadzone_thresh - lower_deadzone_thresh);
    }

    return pedal_pct;
}

static float App_GetPedalPercentage_CountDown(
    uint32_t fully_pressed_value,
    uint32_t unpressed_value,
    uint32_t encoder_value,
    void (*set_encoder_counter)(uint32_t))
{
    const float encoder_relative_range = (float)(unpressed_value - fully_pressed_value);

    // Add a deadzone for both lower and upper encoder ranges
    const float encoder_range_deadzone = encoder_relative_range * PERCENT_DEFLECTION;
    float       lower_deadzone_thresh  = encoder_range_deadzone + (float)fully_pressed_value;
    float       upper_deadzone_thresh  = (encoder_relative_range - encoder_range_deadzone) + (float)fully_pressed_value;
    float       pedal_pct              = 0.0f;

    if (encoder_value > unpressed_value)
    {
        // For counting down, when the encoder unpressed value is exceeded, reset the encoder to the unpressed value and
        // set 0% pedal press
        set_encoder_counter(unpressed_value);
    }
    else if (encoder_value <= lower_deadzone_thresh)
    {
        // For counting down, set mapped pedal percentage to 100% when within the low end deadzone
        pedal_pct = MAX_ACCELERATOR_PEDAL_PRESS;
    }
    else if (encoder_value >= upper_deadzone_thresh)
    {
        pedal_pct = MIN_ACCELERATOR_PEDAL_PRESS;
    }
    else
    {
        pedal_pct = MAX_ACCELERATOR_PEDAL_PRESS * (upper_deadzone_thresh - (float)encoder_value) /
                    (upper_deadzone_thresh - lower_deadzone_thresh);
    }

    return pedal_pct;
}

struct AcceleratorPedals *App_AcceleratorPedals_Create(
    bool (*is_primary_encoder_alarm_active)(void),
    bool (*is_secondary_encoder_alarm_active)(void),
    uint32_t (*get_primary_encoder_counter_value)(void),
    uint32_t (*get_secondary_encoder_counter_value)(void),
    void (*set_primary_encoder_counter)(uint32_t),
    void (*set_secondary_encoder_counter)(uint32_t))
{
    struct AcceleratorPedals *accelerator_pedals = malloc(sizeof(struct AcceleratorPedals));
    assert(accelerator_pedals != NULL);

    accelerator_pedals->is_primary_encoder_alarm_active     = is_primary_encoder_alarm_active;
    accelerator_pedals->is_secondary_encoder_alarm_active   = is_secondary_encoder_alarm_active;
    accelerator_pedals->get_primary_encoder_counter_value   = get_primary_encoder_counter_value;
    accelerator_pedals->get_secondary_encoder_counter_value = get_secondary_encoder_counter_value;
    accelerator_pedals->set_primary_encoder_counter         = set_primary_encoder_counter;
    accelerator_pedals->set_secondary_encoder_counter       = set_secondary_encoder_counter;

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
    return App_GetPedalPercentage_CountDown(
        PAPPS_ENCODER_FULLY_PRESSED_VALUE, PAPPS_ENCODER_UNPRESSED_VALUE,
        accelerator_pedals->get_primary_encoder_counter_value(), accelerator_pedals->set_primary_encoder_counter);
}

float App_AcceleratorPedals_GetSecondaryPedalPercentage(const struct AcceleratorPedals *const accelerator_pedals)
{
    return App_GetPedalPercentage_CountUp(
        SAPPS_ENCODER_FULLY_PRESSED_VALUE, SAPPS_ENCODER_UNPRESSED_VALUE, SAPPS_ENCODER_RESET_VALUE,
        accelerator_pedals->get_secondary_encoder_counter_value(), accelerator_pedals->set_secondary_encoder_counter);
}
