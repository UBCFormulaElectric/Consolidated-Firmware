#pragma once

#include <stdbool.h>

/**
 * Check if the brake is actuated
 * @return True if the brake is actuated, false if the brake is not actuated
 */
bool Io_Brake_IsActuated(void);

/**
 * Check if the MSP-300-2K5-P-3-N-1 is in an open circuit or short circuit
 * state.
 * @return True if the sensor is open or short circuited, false if the sensor is
 * not
 */
bool Io_MSP3002K5P3N1_IsOpenOrShortCircuit(void);

/**
 * Get the pressure in psi from the MSP-300-2K5-P-3-N-1 brake pressure sensor
 * @return The pressure of the primary brake pressure sensor in psi
 */
float Io_MSP3002K5P3N1_GetPressurePsi(void);

//TODO Update descriptions
/**
 * Check if the [REAR BRAKE PRESSURE SENSOR] is in an open circuit or short circuit
 * state.
 * @return True if the sensor is open or short circuited, false if the sensor is
 * not
 */
bool Io_RearBrake_IsOpenOrShortCircuit(void);
/**
 * Get the pressure in psi from the [REAR BRAKE PRESSURE SENSOR]
 * @return The pressure of the secondary brake pressure sensor in psi
 */
float Io_RearBrake_GetPressurePsi(void);


/**
 * Check if the [BRAKE PEDAL ANGLE SENSOR] is in an open circuit or short circuit
 * state.
 * @return True if the sensor is open or short circuited, false if the sensor is
 * not
 */
bool IO_BrakePedal_IsOpenOrShortCircuit(void);
/**
 * @return the angle of the brake pedal in radians.
*/
float Io_BrakePedal_GetAngle(void);
