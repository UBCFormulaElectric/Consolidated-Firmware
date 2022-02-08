#pragma once

/**
 * Read internal die temperatures for all cell monitoring chips
 * @return EXIT_CODE_OK if internal die temperatures (°C) were acquired
 * successfully from all cell monitoring chips. Else, EXIT_CODE_ERROR
 */
bool Io_DieTemperatures_ReadTemp(void);

/**
 * Get the internal die temperature of the cell monitoring chip for the 0th
 * accumulator segment
 * @return The internal die temperature (°C) of the cell monitoring chip for the
 * 0th accumulator segment
 */
float Io_DieTemperatures_GetSegment0DieTemp(void);

/**
 * Get the internal die temperature of the cell monitoring chip for the 1st
 * accumulator segment
 * @return The internal die temperature (°C) of the cell monitoring chip for the
 * 1st accumulator segment
 */
float Io_DieTemperatures_GetSegment1DieTemp(void);

/**
 * Get the internal die temperature of the cell monitoring chip for the 2nd
 * accumulator segment
 * @return The internal die temperature (°C) of the cell monitoring chip for the
 * 2nd accumulator segment
 */
float Io_DieTemperatures_GetSegment2DieTemp(void);

/**
 * Get the internal die temperature of the cell monitoring chip for the 3rd
 * accumulator segment
 * @return The internal die temperature (°C) of the cell monitoring chip for the
 * 3rd accumulator segment
 */
float Io_DieTemperatures_GetSegment3DieTemp(void);

/**
 * Get the internal die temperature of the cell monitoring chip for the 4th
 * accumulator segment
 * @return The internal die temperature (°C) of the cell monitoring chip for the
 * 4th accumulator segment
 */
float Io_DieTemperatures_GetSegment4DieTemp(void);

/**
 * Get the internal die temperature of the cell monitoring chip for the 5th
 * accumulator segment
 * @return The internal die temperature (°C) of the cell monitoring chip for the
 * 5th accumulator segment
 */
float Io_DieTemperatures_GetSegment5DieTemp(void);

/**
 * Get the current maximum internal die temperature out of all the cell monitors
 * @return The current maximum internal die temperature (°C)
 */
float Io_DieTemperatures_GetMaxDieTemp(void);
