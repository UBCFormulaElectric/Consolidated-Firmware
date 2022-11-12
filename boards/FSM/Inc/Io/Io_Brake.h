#pragma once

#include <stdbool.h>

/**
 * Check if the brake is actuated
 * @return True if the brake is actuated, false if the brake is not actuated
 */
bool Io_Brake_IsActuated(void);

/**
 * Get the pressure in psi from the MSP-300-2K5-P-3-N-1 brake pressure sensor
 * @return The pressure of the primary brake pressure sensor in psi
 */
float Io_Brake_GetFrontBrakePressurePsi(void);

/**
 * Get the pressure in psi from the [REAR BRAKE PRESSURE SENSOR]
 * @return The pressure of the secondary brake pressure sensor in psi
 */
float Io_Brake_GetRearBrakePressurePsi(void);

/**
 * Checks if a voltage means that there exist an open or short circuit
 * @param pressure_voltage Voltage in question
 * @return Whether there is a open or short circuit
 */
bool Io_Brake_PressureVoltageAlarm(float pressure_voltage);

/**
 * @return the percent travel of the brake pedal
 */
float Io_Brake_GetPedalPercentTravel(void);
