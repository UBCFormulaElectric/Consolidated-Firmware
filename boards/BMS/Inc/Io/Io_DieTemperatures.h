#pragma once

bool Io_DieTemperatures_StartDieTempConversion(void);

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
float Io_DieTemperatures_GetSegmentDieTemp(AccumulatorSegments_E segment);

/**
 * Get the current maximum internal die temperature out of all the cell monitors
 * @return The current maximum internal die temperature (°C)
 */
float Io_DieTemperatures_GetMaxDieTemp(void);

void Io_DieTemperatures_GetMaxDieTempLocation(uint8_t *segment, uint8_t *ic);
