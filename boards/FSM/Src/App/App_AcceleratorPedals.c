#include <assert.h>
#include <stdlib.h>
#include <stdint.h>
#include "App_AcceleratorPedals.h"
#include "Io_PrimaryScancon2RMHF.h"
#include "Io_AcceleratorPedals.h"

struct AcceleratorPedals
{
    bool (*is_primary_encoder_alarm_active)(void);
    bool (*is_secondary_encoder_alarm_active)(void);
    float (*get_primary_pedal_percent)(void);
    float (*get_secondary_pedal_percent)(void);
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

/*
static float App_GetPedalPercentage_CountUp(
    uint32_t encoder_fully_pressed_value,
    uint32_t encoder_unpressed_value,
    uint32_t encoder_underflow_threshold,
    uint32_t encoder_counter_value,
    void (*set_encoder_counter)(uint32_t))
{
    float low_count_deadzone_threshold =
        (float)(encoder_fully_pressed_value - encoder_unpressed_value) * PERCENT_DEFLECTION +
        (float)encoder_unpressed_value;

    float high_count_deadzone_threshold =
        (float)(encoder_fully_pressed_value - encoder_unpressed_value) * (1.0f - PERCENT_DEFLECTION) +
        (float)encoder_unpressed_value;

    if (encoder_counter_value > encoder_underflow_threshold)
    {
        // If the accelerator pedal underflows (if counter exceeds a threshold
        // only possible to be reached via wrapping on underflow), reset the
        // corresponding encoder's counter register and set the pedal percentage
        // to
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
    return MAX_ACCELERATOR_PEDAL_PRESS * ((float)encoder_counter_value - low_count_deadzone_threshold) /
           (high_count_deadzone_threshold - low_count_deadzone_threshold);
}
 */

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
/*
static float App_GetPedalPercentage_CountDown(
<<<<<<< HEAD
    uint32_t encoder_fully_pressed_value,
    uint32_t encoder_unpressed_value,
    uint32_t encoder_counter_value,
    void (*set_encoder_counter)(uint32_t))
{
    // float low_count_deadzone_threshold =
    //    (float)(encoder_unpressed_value - encoder_fully_pressed_value) * PERCENT_DEFLECTION +
    //    (float)encoder_fully_pressed_value;

    // float high_count_deadzone_threshold =
    //    (float)(encoder_unpressed_value - encoder_fully_pressed_value) * (1.0f - PERCENT_DEFLECTION) +
    //    (float)encoder_fully_pressed_value;

    // if (encoder_counter_value > encoder_unpressed_value)
    //{
    //    // For down counting, when the encoder unpressed value is exceeded,
    //    // reset the encoder to the unpressed value and set 0% pedal press
    //    set_encoder_counter(encoder_unpressed_value);
    //    return MIN_ACCELERATOR_PEDAL_PRESS;
    //}
    // else if (encoder_counter_value >= high_count_deadzone_threshold)
    //{
    //    // For down count, set mapped pedal percentage to 0% when within
    //    // high end deadzone
    //    return MIN_ACCELERATOR_PEDAL_PRESS;
    //}
    // else if (encoder_counter_value <= low_count_deadzone_threshold)
    //{
    //    // For down count, set mapped pedal percentage to 100% when within the
    //    // low end deadzone
    //    return MAX_ACCELERATOR_PEDAL_PRESS;
    //}

    //// Pedal % = 100% * (high end deadzone threshold - encoder value) /
    //// (high end deadzone threshold - low end deadzone threshold)
    // return MAX_ACCELERATOR_PEDAL_PRESS * (high_count_deadzone_threshold - (float)encoder_counter_value) /
    //       (high_count_deadzone_threshold - low_count_deadzone_threshold);
    return 0.0f;
=======
   uint32_t encoder_fully_pressed_value,
   uint32_t encoder_unpressed_value,
   uint32_t encoder_counter_value,
   void (*set_encoder_counter)(uint32_t))
{
   // float low_count_deadzone_threshold =
   //    (float)(encoder_unpressed_value - encoder_fully_pressed_value) * PERCENT_DEFLECTION +
   //    (float)encoder_fully_pressed_value;

   // float high_count_deadzone_threshold =
   //    (float)(encoder_unpressed_value - encoder_fully_pressed_value) * (1.0f - PERCENT_DEFLECTION) +
   //    (float)encoder_fully_pressed_value;

   // if (encoder_counter_value > encoder_unpressed_value)
   //{
   //    // For down counting, when the encoder unpressed value is exceeded,
   //    // reset the encoder to the unpressed value and set 0% pedal press
   //    set_encoder_counter(encoder_unpressed_value);
   //    return MIN_ACCELERATOR_PEDAL_PRESS;
   //}
   // else if (encoder_counter_value >= high_count_deadzone_threshold)
   //{
   //    // For down count, set mapped pedal percentage to 0% when within
   //    // high end deadzone
   //    return MIN_ACCELERATOR_PEDAL_PRESS;
   //}
   // else if (encoder_counter_value <= low_count_deadzone_threshold)
   //{
   //    // For down count, set mapped pedal percentage to 100% when within the
   //    // low end deadzone
   //    return MAX_ACCELERATOR_PEDAL_PRESS;
   //}

   //// Pedal % = 100% * (high end deadzone threshold - encoder value) /
   //// (high end deadzone threshold - low end deadzone threshold)
   // return MAX_ACCELERATOR_PEDAL_PRESS * (high_count_deadzone_threshold - (float)encoder_counter_value) /
   //       (high_count_deadzone_threshold - low_count_deadzone_threshold);
   return 0.0f;
>>>>>>> 9f4e826a1653caf1378f99c986767d1a000e3140
}
*/

struct AcceleratorPedals *App_AcceleratorPedals_Create(
    bool (*is_primary_encoder_alarm_active)(void),
    bool (*is_secondary_encoder_alarm_active)(void),
    float (*get_primary_pedal_percent)(void),
    float (*get_secondary_pedal_percent)(void))
{
    struct AcceleratorPedals *accelerator_pedals = malloc(sizeof(struct AcceleratorPedals));
    assert(accelerator_pedals != NULL);

    accelerator_pedals->is_primary_encoder_alarm_active   = is_primary_encoder_alarm_active;
    accelerator_pedals->is_secondary_encoder_alarm_active = is_secondary_encoder_alarm_active;
    accelerator_pedals->get_primary_pedal_percent         = get_primary_pedal_percent;
    accelerator_pedals->get_secondary_pedal_percent       = get_secondary_pedal_percent;

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

float App_AcceleratorPedals_GetPrimaryPedalPercentage(const struct AcceleratorPedals *accelerator_pedals)
{
<<<<<<< HEAD
    return Io_AcceleratorPedals_GetPapps();
    return App_GetPedalPercentage_CountDown(
        PAPPS_ENCODER_FULLY_PRESSED_VALUE, PAPPS_ENCODER_UNPRESSED_VALUE,
        accelerator_pedals->get_primary_encoder_counter_value(), accelerator_pedals->set_primary_encoder_counter);
}

float App_AcceleratorPedals_GetSecondaryPedalPercentage(const struct AcceleratorPedals *const accelerator_pedals)
{
    return Io_AcceleratorPedals_GetPapps();
    return App_GetPedalPercentage_CountDown(
        PAPPS_ENCODER_FULLY_PRESSED_VALUE, PAPPS_ENCODER_UNPRESSED_VALUE,
        accelerator_pedals->get_primary_encoder_counter_value(), accelerator_pedals->set_primary_encoder_counter);
=======
    return accelerator_pedals->get_primary_pedal_percent();
    // return App_GetPedalPercentage_CountDown(
    //    PAPPS_ENCODER_FULLY_PRESSED_VALUE, PAPPS_ENCODER_UNPRESSED_VALUE,
    //    accelerator_pedals->get_primary_encoder_counter_value(), accelerator_pedals->set_primary_encoder_counter);
}

float App_AcceleratorPedals_GetSecondaryPedalPercentage(const struct AcceleratorPedals *accelerator_pedals)
{
    return accelerator_pedals->get_primary_pedal_percent();
    /*
    return App_GetPedalPercentage_CountDown(
        PAPPS_ENCODER_FULLY_PRESSED_VALUE, PAPPS_ENCODER_UNPRESSED_VALUE,
        accelerator_pedals->get_primary_encoder_counter_value(), accelerator_pedals->set_primary_encoder_counter);
        */
>>>>>>> 9f4e826a1653caf1378f99c986767d1a000e3140
}
