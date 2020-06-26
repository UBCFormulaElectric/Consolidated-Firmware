#pragma once

#include <stdbool.h>

struct AcceleratorPedal;

/**
 * Allocate and initialize an accelerator pedal
 * @param is_encoder_alarm_active A function that can be called to check whether
 * the encoder alarm is active (Modeled after MAX3097E)
 * @param reset_encoder_count A function that can be called to reset the counter
 * value of the encoder corresponding to the given accelerator pedal
 * @param get_encoder_counter_value A function that can be called to get the
 * counter value of the encoder corresponding to the given accelerator pedal
 * @param encoder_max_pressed_value The maximum encoder counter value
 * corresponding to the given accelerator pedal calibrated to the pedal box
 * @return The created accelerator pedal, whose ownership is given to the caller
 */
struct AcceleratorPedal *App_AcceleratorPedal_Create(
    bool (*is_encoder_alarm_active)(void),
    void (*reset_encoder_count)(void),
    uint32_t (*get_encoder_counter_value)(void),
    uint32_t encoder_max_pressed_value);

/**
 * Deallocate the memory used by the given accelerator pedal
 * @param accelerator_pedal The accelerator pedal to deallocate
 */
void App_AcceleratorPedal_Destroy(struct AcceleratorPedal *accelerator_pedal);

/**
 * Check if the encoder alarm for the given accelerator pedal is active
 * @param accelerator_pedal The accelerator pedal to check
 * @return true if the encoder alarm for the given accelerator pedal is active,
 *         else false
 */
bool App_AcceleratorPedal_IsEncoderAlarmActive(
    const struct AcceleratorPedal *accelerator_pedal);

/**
 * Get the pedal percentage for the given accelerator pedal
 * @param accelerator_pedal The accelerator pedal to get the pedal percentage
 * from
 * @return The pedal percentage of the given accelerator pedal
 */
float App_AcceleratorPedal_GetPedalPercentage(
    const struct AcceleratorPedal *accelerator_pedal);
