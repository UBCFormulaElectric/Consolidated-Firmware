#pragma once

#include <stdbool.h>

struct Brake;

/**
 * Allocate and initialize a brake
 * @param get_pressure_psi A function that returns the brake pressure in psi
 * @param is_pressure_sensor_open_or_short_circuit A function that checks
 * whether the pressure sensor is in an open or short circuit state
 * @param is_brake_actuated A function that checks whether the brake is actuated
 * @param min_pressure_psi The minimum brake pressure in psi that can be read
 * from the brake pressure sensor
 * @param max_pressure_psi The maximum brake pressure in psi that can be read
 * from the brake pressure sensor
 * @return The created brake whose ownership is given to the caller
 */
struct Brake *App_Brake_Create(
    float (*get_pressure_psi)(void),
    bool (*is_pressure_sensor_open_or_short_circuit)(void),
    bool (*is_brake_actuated)(void),
    float min_pressure_psi,
    float max_pressure_psi);

/**
 * Deallocate the memory used by the given brake
 * @param brake The brake to deallocate
 */
void App_Brake_Destroy(struct Brake *brake);

/**
 * Get the pressure in-range check for the given brake
 * @param brake The brake to get the pressure in-range check for
 * @return The pressure in-range check for the given brake
 */
struct InRangeCheck *App_Brake_GetPressureInRangeCheck(const struct Brake *brake);

/**
 * Check if the given brake is actuated
 * @param brake The brake to check if it is actuated
 * @return True if the brake is actuated, false if it is not
 */
bool App_Brake_IsBrakeActuated(const struct Brake *brake);

/**
 * Check if the brake pressure sensor is in an open or short circuit state
 * @param brake The brake to check if the corresponding pressure sensor is
 * in an open or short circuit state
 * @return True if the brake pressure sensor is in an open or short circuit
 * state, false if it is not
 */
bool App_Brake_IsPressureSensorOpenOrShortCircuit(const struct Brake *brake);
