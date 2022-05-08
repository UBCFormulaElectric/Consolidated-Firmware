#pragma once

#include <stdbool.h>

// TODO: Experimentally determine the encoder values for the primary and
// secondary APPS when the accelerator is fully pressed #666
#define PAPPS_ENCODER_FULLY_PRESSED_VALUE (2500U)
#define SAPPS_ENCODER_FULLY_PRESSED_VALUE (1800U)

// If encoder exceeds the following values, we reset it back to zero (usually
// due to underflowing)
#define PAPPS_ENCODER_RESET_VALUE (3500U)
#define SAPPS_ENCODER_RESET_VALUE (2400U)

#define MAX_ACCELERATOR_PEDAL_PRESS (100.0f)
#define MIN_ACCELERATOR_PEDAL_PRESS (0.0f)
#define PERCENT_DEFLECTION (0.03f)

struct AcceleratorPedals;

/**
 * Allocate and initialize a pair of accelerator pedals
 * @param is_primary_encoder_alarm_active A function that can be called to check
 * whether the primary encoder alarm is active (Modeled after MAX3097E)
 * @param is_secondary_encoder_alarm_active A function that can be called to
 * check whether the secondary encoder alarm is active (Modeled after MAX3097E)
 * @param get_primary_encoder_counter_value A function that can be called to get
 * the counter value of the primary encoder
 * @param get_secondary_encoder_counter_value A function that can be called to
 * get the counter value of the secondary encoder
 * @param reset_primary_encoder_counter A function that can be called to reset
 * the counter value of the primary encoder to 0
 * @param reset_secondary_encoder_counter A function that can be called to reset
 * the counter value of the secondary encoder to 0
 * @param primary_encoder_fully_pressed_value The value of the primary encoder
 * counter calibrated to the pedal box corresponding to when the pedal is fully
 * pressed
 * @param secondary_encoder_fully_pressed_value The value of the secondary
 * encoder counter calibrated to the pedal box corresponding to when the pedal
 * is fully pressed
 * @return The created pair of accelerator pedals, whose ownership is given to
 * the caller
 */
struct AcceleratorPedals *App_AcceleratorPedals_Create(
    bool (*is_primary_encoder_alarm_active)(void),
    bool (*is_secondary_encoder_alarm_active)(void),
    uint32_t (*get_primary_encoder_counter_value)(void),
    uint32_t (*get_secondary_encoder_counter_value)(void),
    void (*reset_primary_encoder_counter)(void),
    void (*reset_secondary_encoder_counter)(void));

/**
 * Deallocate the memory used by the given pair of accelerator pedals
 * @param accelerator_pedals The pair of accelerator pedals to deallocate
 */
void App_AcceleratorPedals_Destroy(struct AcceleratorPedals *accelerator_pedals);

/**
 * Check if the encoder alarm for the primary accelerator pedal is active
 * @param accelerator_pedals The pair of accelerator pedals containing the
 * primary accelerator pedal to check
 * @return true if the encoder alarm for the given accelerator pedal is active,
 *         else false
 */
bool App_AcceleratorPedals_IsPrimaryEncoderAlarmActive(const struct AcceleratorPedals *accelerator_pedals);

/**
 * Check if the encoder alarm for the secondary accelerator pedal is active
 * @param accelerator_pedals The pair of accelerator pedals containing the
 * secondary accelerator pedal to check
 * @return true if the encoder alarm for the given accelerator pedal is active,
 *         else false
 */
bool App_AcceleratorPedals_IsSecondaryEncoderAlarmActive(const struct AcceleratorPedals *accelerator_pedals);

/**
 * Get the pedal percentage of the primary accelerator pedal, a value in [0,
 * 100]
 * @param accelerator_pedals The pair of accelerator pedals to get the primary
 * pedal percentage from
 * @return The pedal percentage of the primary accelerator pedal
 */
float App_AcceleratorPedals_GetPrimaryPedalPercentage(const struct AcceleratorPedals *accelerator_pedals);

/**
 * Get the pedal percentage of the secondary accelerator pedal, a value in [0,
 * 100]
 * @param accelerator_pedals The pair of accelerator pedals to get the secondary
 * pedal percentage from
 * @return The pedal percentage of the secondary accelerator pedal
 */
float App_AcceleratorPedals_GetSecondaryPedalPercentage(const struct AcceleratorPedals *accelerator_pedals);
