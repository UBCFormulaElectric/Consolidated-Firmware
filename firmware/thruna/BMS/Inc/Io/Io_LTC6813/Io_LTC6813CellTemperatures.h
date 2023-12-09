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

/**
 * Get the min cell temperature measured in the daisy chain
 * @param segment The segment where the min cell temperature is located
 * @param therm The thermistor measuring the min cell temperature
 * @return The min cell temperature in degC
 */
float Io_LTC6813CellTemperatures_GetMinTempDegC(uint8_t *segment, uint8_t *therm);

/**
 * Get the max cell temperature measured in the daisy chain
 * @param segment The segment where the max cell temperature is located
 * @param therm The thermistor measuring the max cell temperature
 * @return The max cell temperature in degC
 */
float Io_LTC6813CellTemperatures_GetMaxTempDegC(uint8_t *segment, uint8_t *therm);

/**
 * Get the average cell temperature
 * @return The average cell temperature in degC
 */
float Io_LTC6813CellTemperatures_GetAverageTempDegC(void);

/**
 * Read all cell temperatures from the LTC6813
 * @param celltemp2 pointer to float value to write temperature in for segment 4, cell 2
 * @param celltemp8 pointer to float value to write temperature in for segment 4, cell 8
 */
void Io_LTC6813CellTemperatures_GetSpecifiedTempDegC(float *celltemp2, float *celltemp8);
