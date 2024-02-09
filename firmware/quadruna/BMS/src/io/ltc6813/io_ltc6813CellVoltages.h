#pragma once

/**
 * Read all cell voltages from the LTC6813
 * Note: Io_LTC6813CellVoltages_StartAdcConversion needs to be called to trigger
 * cell voltages conversions before valid cell voltages can be read back
 * @return True if cell voltages are read back successfully. Else, false
 */
bool io_ltc6813CellVoltages_readVoltages(void);

/**
 * Start an ADC conversion on the LTC6813 for measured cell voltages
 * @return True if the ADC conversions have been sent out successfully. Else,
 * false
 */
bool io_ltc6813CellVoltages_startAdcConversion(void);

/**
 * @brief Read cell voltages from the previously stored values
 * @return Cell Voltage Value
 */
float io_ltc6813CellVoltages_getCellVoltage(uint8_t segement, uint8_t cell);

/**
 * Start an open wire check on the LTC6813, used to detect if any connections from the ADCs to the cells have gone open
 * circuit. The command performs ADC conversions identically to the ADCV command, except with either a pull up or pull
 * down current.
 * @param pull_up If True, run command with pull-up current. Otherwise run with pull-down.
 * @return True if the ADOW commands have been sent out succefully. Else, false
 */
bool io_ltc6813CellVoltages_owcStart(bool pull_up);

/**
 * Read all cell voltages from the LTC6813 for Open Wire Check
 * Note: Io_LTC6813CellVoltages_StartOpenWireCheck needs to be called to trigger
 * Open Wire Check voltage conversions before voltages can be read back
 * @param pull_up If True, store values into pull-up voltage buffer. Otherwire,
 * store values into pull-down voltage buffer.
 * @return True if cell voltages are read back succefully. Else, false
 */
bool io_ltc6813CellVoltages_owcReadVoltages(bool pull_up);

/**
 * @brief Check to see if any of the cell segments might have open wires
 * Note: Open Wire Check Read and parse voltage functions need to be
 * called before calculating the status.
 */
void io_ltc6813CellVoltages_checkOpenWireStatus(void);

/**
 * @brief Get the global Open Wire Fault before looking for specific cell
 * with OWC faults.
 */
bool io_ltc6813CellVoltages_getGlobalOpenWireFault(void);

/**
 * @brief Get the Open Wire Faults from the the last check.
 */
bool io_ltc6813CellVoltages_getOpenWireFault(uint8_t segment, uint8_t cell);
