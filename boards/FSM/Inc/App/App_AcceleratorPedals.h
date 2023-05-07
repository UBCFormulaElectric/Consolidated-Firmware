#pragma once

#include <stdbool.h>

#include "App_FsmWorld.h"

#define PAPPS_ENCODER_FULLY_PRESSED_VALUE (937U)
#define PAPPS_ENCODER_UNPRESSED_VALUE (1353U)
#define SAPPS_ENCODER_FULLY_PRESSED_VALUE (310U)
#define SAPPS_ENCODER_UNPRESSED_VALUE (0U)
#define SAPPS_ENCODER_RESET_VALUE (2400U)

#define MAX_ACCELERATOR_PEDAL_PRESS (100.0f)
#define MIN_ACCELERATOR_PEDAL_PRESS (0.0f)
#define PERCENT_DEFLECTION (0.03f)

struct AcceleratorPedals;

/**
 * Allocate and initialize a pair of accelerator pedals
 * @param get_primary_pedal_percent
 * @param primary_pedal_OCSC
 * @param get_secondary_pedal_percent
 * @param secondary_pedal_OCSC
 * @return The created pair of accelerator pedals, whose ownership is given to
 * the caller
 */
struct AcceleratorPedals *App_AcceleratorPedals_Create(
    float (*get_primary_pedal_percent)(void), bool (*primary_pedal_OCSC)(void),
    float (*get_secondary_pedal_percent)(void),
    bool (*secondary_pedal_OCSC)(void));

/**
 * Deallocate the memory used by the given pair of accelerator pedals
 * @param accelerator_pedals The pair of accelerator pedals to deallocate
 */
void App_AcceleratorPedals_Destroy(
    struct AcceleratorPedals *accelerator_pedals);

/**
 * Get the pedal percentage of the primary accelerator pedal, a value in [0,100]
 * @param accelerator_pedals The pair of accelerator pedals to get the primary
 * pedal percentage from
 * @return The pedal percentage of the primary accelerator pedal
 */
float App_AcceleratorPedals_GetPrimaryPedalPercentage(
    const struct AcceleratorPedals *accelerator_pedals);
/**
 * Get the pedal percentage of the secondary accelerator pedal, a value in
 * [0,100]
 * @param accelerator_pedals The pair of accelerator pedals to get the secondary
 * pedal percentage from
 * @return The pedal percentage of the secondary accelerator pedal
 */
float App_AcceleratorPedals_GetSecondaryPedalPercentage(
    const struct AcceleratorPedals *accelerator_pedals);

/**
 * Primary Broadcast Function. Very important.
 * @param world World Context from which to pull sensors
 */
void App_AcceleratorPedals_Broadcast(const struct FsmWorld *world);
