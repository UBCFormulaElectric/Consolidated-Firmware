#pragma once

#include "App_SharedExitCode.h"

/**
 * Read cell temperatures from all thermistors connected to the accumulator
 * @return EXIT_CODE_OK if cell temperatures (°C) were acquired successfully
 * from all thermistors connected to the accumulator. Else, EXIT_CODE_ERROR
 */
ExitCode Io_AccumulatorTemperatures_ReadTemperatures(void);

/**
 * Get the current minimum cell temperature out of all cell temperatures from
 * the accumulator
 * @return The current minimum cell temperature (°C)
 */
float Io_AccumulatorTemperatures_GetMinCellTemperature(void);

/**
 * Get the current maximum cell temperature out of all cell temperatures from
 * the accumulator
 * @return The current maximum cell temperature (°C)
 */
float Io_AccumulatorTemperatures_GetMaxCellTemperature(void);

/**
 * Get the average cell temperature for the accumulator
 * @return The average accumulator cell temperature (°C)
 */
float Io_AccumulatorTemperatures_GetAverageCellTemperature(void);
