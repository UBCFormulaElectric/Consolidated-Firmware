#pragma once

#include <stdbool.h>

/**
 * Check if the brake is actuated, by getting the digital signal from the comparator on the FSM
 * @return True if the brake is actuated, false if the brake is not actuated
 */
bool io_brake_isActuated(void);

/**
 * Get the pressure in psi from the MSP-300-2K5-P-3-N-1 brake pressure sensor
 * @return The pressure of the primary brake pressure sensor in psi
 */
float io_brake_getFrontPressurePsi(void);

/**
 * @return whether there is a open or short circuit in the front pressure sensor
 */
bool io_brake_frontPressureSensorOCSC(void);

/**
 * Get the pressure in psi from the [REAR BRAKE PRESSURE SENSOR]
 * @return The pressure of the secondary brake pressure sensor in psi
 */
float io_brake_getRearPressurePsi(void);

/**
 * @return whether or not the rear pressure sensor is open or short circuit
 */
bool io_brake_rearPressureSensorOCSC(void);

/**
 * @return the percent travel of the brake pedal
 */
float io_brake_getPedalPercentTravel(void);

/**
 *
 * @return whether or not the brake pedal travel sensor is open or short circuit
 */
bool io_brake_pedalSensorOCSC(void);