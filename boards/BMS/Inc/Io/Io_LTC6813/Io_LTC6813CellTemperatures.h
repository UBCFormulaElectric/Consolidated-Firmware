#pragma once

/**
 * Start ADC conversion for cell temperature measurements
 * @return True if the ADC conversions have been sent out successfully. Else
 * false
 */
bool Io_LTC6813CellTemperatures_StartAdcConversion(void);

/**
 * Read all cell temperatures from the LTC6813
 * Note: Io_LTC6813CellTemperatures_StartAdcConversion needs to be called to
 * trigger cell temperature conversions before valid cell temperatures can be
 * read back
 * @return True if cell temperatures are read back successfully. Else, false
 */
bool Io_LTC6813CellTemperatures_ReadTemperatures(void);
