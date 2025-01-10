#pragma once

#include <stdint.h>

/**
 * Start ADC conversion for cell temperature measurements
 * @return True if the ADC conversions have been sent out successfully. Else
 * false
 */
bool io_ltc6813CellTemps_startAdcConversion(void);

/**
 * Read all cell temperatures from the LTC6813
 * Note: io_ltc6813CellTemps_StartAdcConversion needs to be called to
 * trigger cell temperature conversions before valid cell temperatures can be
 * read back
 * @return True if cell temperatures are read back successfully. Else, false
 */
bool io_ltc6813CellTemps_readTemperatures(void);

/**
 * Get the min cell temperature measured in the daisy chain
 * @param segment The segment where the min cell temperature is located
 * @param therm The thermistor measuring the min cell temperature
 * @return The min cell temperature in degC
 */
float io_ltc6813CellTemps_getMinTempDegC(uint8_t* segment, uint8_t* therm);

/**
 * Get the max cell temperature measured in the daisy chain
 * @param segment The segment where the max cell temperature is located
 * @param therm The thermistor measuring the max cell temperature
 * @return The max cell temperature in degC
 */
float io_ltc6813CellTemps_getMaxTempDegC(uint8_t* segment, uint8_t* therm);

/**
 * Get the average cell temperature
 * @return The average cell temperature in degC
 */
float io_ltc6813CellTemps_getAverageTempDegC(void);

/**
 * Read individual cell temperature from the LTC6813
 * @param segment segment to read cell temp from
 * @param reg_group register group to read cell temp from
 * @param thermistor thermistor to read cell temp from
 * @return Cell temp in degrees C
 */
float io_ltc6813CellTemperatures_getSpecificCellTempDegC(uint8_t segment, uint8_t reg_group, uint8_t thermistor);
