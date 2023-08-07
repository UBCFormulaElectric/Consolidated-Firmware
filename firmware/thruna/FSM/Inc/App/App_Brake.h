#pragma once

#include <stdbool.h>
#include "App_FsmWorld.h"

struct Brake;

/**
 *
 * @param get_front_pressure_psi A function that returns the brake pressure in psi from the primary sensor
 * @param front_pressure_sensor_ocsc A function that returns whether or not the front pressure sensor is open or short
 * circuit
 * @param get_rear_pressure_psi A function that returns the brake pressure in psi from the secondary sensor
 * @param rear_pressure_sensor_ocsc A function that returns whether or not the rear pressure sensor is open or short
 * circuit
 * @param get_pedal_travel A function that returns the brake pedal angle in radians
 * @param pedal_travel_sensor_ocsc A function that returns whether or not the pedal travel sensor is open or short
 * circuit
 * @param is_brake_actuated A function that checks whether the brake is actuated
 * @return The created brake whose ownership is given to the caller
 */
struct Brake *App_Brake_Create(
    float (*get_front_pressure_psi)(void),
    bool (*front_pressure_sensor_ocsc)(void),
    float (*get_rear_pressure_psi)(void),
    bool (*rear_pressure_sensor_ocsc)(void),
    float (*get_pedal_travel)(void),
    bool (*pedal_travel_sensor_ocsc)(void),
    bool (*is_brake_actuated)(void));
/**
 * Deallocate the memory used by the given brake
 * @param brake The brake to deallocate
 */
void App_Brake_Destroy(struct Brake *brake);

/**
 * Getter for get_front_sensor_psi function
 * @param brake
 * @return
 */
float App_Brake_GetFrontPSI(const struct Brake *brake);
/**
 * Getter for get_rear_sensor_psi function
 * @param brake
 * @return
 */
float App_Brake_GetRearPSI(const struct Brake *brake);
/**
 * Returns whether there is an open or short circuit in front or rear brake pressure sensor
 * @param brake
 * @return
 */
bool App_Brake_PressureElectricalFault(const struct Brake *brake);

/**
 * Check if the given brake is actuated
 * @param brake The brake to check if it is actuated
 * @return True if the brake is actuated, false if it is not
 */
bool App_Brake_IsBrakeActuated(const struct Brake *brake);

/**
 * Get the pressure in-range check for the given brake
 * @param brake The brake to get the pressure in-range check for
 * @return The pressure in-range check for the given brake
 */
struct InRangeCheck *App_Brake_GetPressureInRangeCheck(const struct Brake *brake);

/**
 * Gets the pedal angle of a given brake
 * @param brake
 * @return The pedal angle (percent)
 */
float App_Brake_GetPedalAngle(const struct Brake *brake);
/**
 * Checks if a given pedal sensor is open or short circuit
 * @param brake The brake which contains the pedal sensor
 * @return Whether or not the pedal sensor is open or short circuit
 */
bool App_Brake_PedalSensorAlarm(const struct Brake *brake);

/**
 * CAN BUS Broadcast Function
 * @param world World Context from which to pull sensors
 */
void App_Brake_Broadcast(const struct FsmWorld *world);
