#pragma once
#include <stdbool.h>

/**
 * @returns mechanical load on load cell based on the following data sheet:
 */
float io_loadCell_voltageToMechancialLoad();

/**
 * @returns helper function to check OCSC for both load cells
 */
bool io_loadCell_OCSC(int);

/**
 * @returns voltage from load cell 3 (after op-amp gain)
 */
float io_loadCell_getVoltage3();

/**
 * @returns voltage from load cell 4 (after op-amp gain)
 */
float io_loadCell_getVoltage4();

/**
 * @returns checks open and short circuit for load cell 3
 */
bool io_loadCell3_OCSC(void);

/**
 * @returns checks open and short circuit for load cell 3
 */
bool io_loadCell3_OCSC(void);
