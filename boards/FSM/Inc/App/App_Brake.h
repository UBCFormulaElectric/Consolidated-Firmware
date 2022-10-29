#pragma once

#include <stdbool.h>
#include "App_FsmWorld.h"

struct Brake;

/**
 * Allocate and initialize a brake
 * @param get_primary_pressure_psi A function that returns the brake pressure in psi from the primary sensor
 * @param get_secondary_pressure_psi A function that returns the brake pressure in psi from the secondary sensor
 * @param is_primary_pressure_sensor_open_or_short_circuit A function that checks whether the primary pressure sensor is
 * in an open or short circuit state
 * @param is_secondary_pressure_sensor_open_or_short_circuit A function that checks whether the secondary pressure
 * sensor is in an open or short circuit state
 * @param pedal_angle A function that returns the brake pedal angle in radians
 * @param is_pedal_angle_sensor_open_or_short_circuit A function that checks whether the pedal angle sensor is in an
 * open or short circuit state
 * @param is_brake_actuated A function that checks whether the brake is actuated
 * @return The created brake whose ownership is given to the caller
 */
struct Brake *App_Brake_Create(
    float (*get_primary_pressure_psi)(void),
    float (*get_secondary_pressure_psi)(void),
    bool (*is_primary_pressure_sensor_open_or_short_circuit)(void),
    bool (*is_secondary_pressure_sensor_open_or_short_circuit)(void),
    float (*pedal_angle)(void),
    bool (*is_pedal_angle_sensor_open_or_short_circuit)(void),
    bool (*is_brake_actuated)(void));

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

/**
 * Check if the pressure sensor is OC
 * @param brake The brake to check pressure sensor OC for
 * @return True if the pressure sensor is open circuit, otherwise false
 */
bool App_Brake_IsPressureSensorOpenCircuit(const struct Brake *brake);

/**
 * Check if the pressure sensor is SC
 * @param brake The brake to check pressure sensor SC for
 * @return True if the pressure sensor is SC, otherwise false
 */
bool App_Brake_IsPressureSensorShortCircuited(const struct Brake *brake);

bool App_Brake_IsPedalSensorOpenOrShortCircuit(const struct Brake *brake);

/**
 * CAN BUS Broadcast Function
 * @param world World Context from which to pull sensors
 */
void App_Brake_Broadcast(const struct FsmWorld * world);
