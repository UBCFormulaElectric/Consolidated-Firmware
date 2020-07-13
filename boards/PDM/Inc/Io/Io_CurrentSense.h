#pragma once

/**
 * Get the auxiliary 1 current, in amps
 * @return The auxiliary 1 current, in amps
 */
float Io_CurrentSense_GetAux1Current(void);

/**
 * Get the auxiliary 2 current, in amps
 * @return The auxiliary 2 current, in amps
 */
float Io_CurrentSense_GetAux2Current(void);

/**
 * Get the AIR shutdown current, in amps
 * @return The AIR shutdown current, in amps
 */
float Io_CurrentSense_GetAirShutdownCurrent(void);

/**
 * Get the CAN current, in amps
 * @return The CAN current, in amps
 */
float Io_CurrentSense_GetCanCurrent(void);

/**
 * Get the energy meter current, in amps
 * @return The energy meter current, in amps
 */
float Io_CurrentSense_GetEnergyMeterCurrent(void);

/**
 * Get the back left inverter current, in amps
 * @return The back left inverter current, in amps
 */
float Io_CurrentSense_GetBackLeftInverterCurrent(void);

/**
 * Get the back right inverter current, in amps
 * @return The back right inverter current, in amps
 */
float Io_CurrentSense_GetBackRightInverterCurrent(void);

/**
 * Get the front left inverter current, in amps
 * @return The front left inverter current, in amps
 */
float Io_CurrentSense_GetFrontLeftInverterCurrent(void);

/**
 * Get the front right inverter current, in amps
 * @return The front right inverter current, in amps
 */
float Io_CurrentSense_GetFrontRightInverterCurrent(void);
