#pragma once

/**
 * Read raw thermistor voltages from the cell monitoring chips
 * @return EXIT_CODE_OK if raw thermistor voltages are read successfully from
 * all cell monitoring chips. Else, EXIT_CODE_ERROR
 */
bool Io_NewCellTemperatures_GetCellTemperatureDegC(void);
