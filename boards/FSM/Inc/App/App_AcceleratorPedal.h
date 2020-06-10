#pragma once

#include <stdbool.h>

struct AcceleratorPedal;

/**
 * Allocate and initialize an accelerator pedal
 * @param is_encoder_alarm_active A function that can be called to check whether
 *                                the encoder alarm is active (Modeled after
 *                                MAX-3097E)
 * @return The created accelerator pedal, whose ownership is given to the caller
 */
struct AcceleratorPedal *
    App_AcceleratorPedal_Create(bool (*is_encoder_alarm_active)(void));

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
