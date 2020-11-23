#pragma once

#include <stdint.h>
#include <stdlib.h>
#include "App_SharedExitCode.h"

/**
 * Read raw cell voltages measured from the accumulator
 * @return EXIT_CODE_OK if the raw cell voltages (100µV) were acquired
 * successfully. Else, EXIT_CODE_ERROR.
 */
ExitCode Io_AccumulatorVoltages_ReadRawCellVoltages(void);

/**
 * Get the minimum cell voltage out of all cell voltages in the accumulator
 * @return The minimum cell voltage (V) out of all cell voltages in the
 * accumulator
 */
float Io_AccumulatorVoltages_GetMinCellVoltage(void);

/**
 * Get the maximum cell voltage out of all cell voltages in the accumulator
 * @return The maximum cell voltage (V) out of all cell voltages in the
 * accumulator
 */
float Io_AccumulatorVoltages_GetMaxCellVoltage(void);

/**
 * Get the average cell voltage for the accumulator
 * @return The average cell voltage (V) for the accumulator
 */
float Io_AccumulatorVoltages_GetAverageCellVoltage(void);

/**
 * Get the pack voltage for the accumulator
 * @return The pack voltage (V) for the accumulator
 */
float Io_AccumulatorVoltages_GetPackVoltage(void);

/**
 * Get the voltage of the 0th accumulator segment
 * @return The voltage (V) of the 0th accumulator segment
 */
float Io_AccumulatorVoltages_GetSegment0Voltage(void);

/**
 * Get the voltage of the 1st accumulator segment
 * @return The voltage (V) of the 1st accumulator segment
 */
float Io_AccumulatorVoltages_GetSegment1Voltage(void);

/**
 * Get the voltage of the 2nd accumulator segment
 * @return The voltage (V) of the 2nd accumulator segment
 */
float Io_AccumulatorVoltages_GetSegment2Voltage(void);

/**
 * Get the voltage of the 3rd accumulator segment
 * @return The voltage (V) of the 3rd accumulator segment
 */
float Io_AccumulatorVoltages_GetSegment3Voltage(void);

/**
 * Get the voltage of the 4th accumulator segment
 * @return The voltage (V) of the 4th accumulator segment
 */
float Io_AccumulatorVoltages_GetSegment4Voltage(void);

/**
 * Get the voltage of the 5th accumulator segment
 * @return The voltage (V) of the 5th accumulator segment
 */
float Io_AccumulatorVoltages_GetSegment5Voltage(void);
