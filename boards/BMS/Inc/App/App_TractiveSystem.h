#pragma once

#include "App_SharedBinaryStatus.h"

struct TractiveSystem;

/**
 * Allocate and initialize a tractive system
 * @param get_system_voltage A function that returns the voltage on the tractive
 * system
 * @return A tractive system whose ownership is given to the caller
 */
struct TractiveSystem *App_TractiveSystem_Create(
    float (*get_system_voltage)(float),
    float (*get_adc_voltage)(void));

/**
 * Deallocate the memory used by the given tractive system
 * @param tractive_system The given tractive system to deallocate
 */
void App_TractiveSystem_Destroy(struct TractiveSystem *tractive_system);

/**
 * Get the voltage from the Tractive System
 * @param tractive_system The Tractive System to get the voltage from
 * @return The voltage of the given Tractive System
 */
float App_TractiveSystem_GetSystemVoltage(
    const struct TractiveSystem *const tractive_system);

/**
 * Get the capacitance on the Tractive System
 * @param tractive_system The Tractive System to get the voltage from
 * @return The capacitance of the given Tractive System
 */
float App_TractiveSystem_GetSystemCapacitance(void);
