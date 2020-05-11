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
 * Get the left inverter current, in amps
 * @return The left inverter current, in amps
 */
float Io_CurrentSense_GetLeftInverterCurrent(void);

/**
 * Get the right inverter current, in amps
 * @return The right inverter current, in amps
 */
float Io_CurrentSense_GetRightInverterCurrent(void);

/**
 * Get the energy meter current, in amps
 * @return The energy meter current, in amps
 */
float Io_CurrentSense_GetEnergyMeterCurrent(void);

/**
 * Get the CAN current, in amps
 * @return The CAN current, in amps
 */
float Io_CurrentSense_GetCanCurrent(void);

/**
 * Get the AIR shutdown current, in amps
 * @return The AIR shutdown current, in amps
 */
float Io_CurrentSense_GetAirShutdownCurrent(void);
