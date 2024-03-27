#pragma once
#include <stdbool.h>

/**
 * @returns mechanical loadd based off adc pin voltage (after op-amp gain)
 */
float io_loadCell_getMechanicalLoad3();

/**
 * @returns mechanical loadd based off adc pin voltage (after op-amp gain)
 */
float io_loadCell_getMechanicalLoad4();

/**
 * @returns checks open and short circuit for load cell 3
 */
bool io_loadCell3_OCSC(void);

/**
 * @returns checks open and short circuit for load cell 3
 */
bool io_loadCell4_OCSC(void);
