#pragma once
#include <stdbool.h>

/**
 * getter for mechincal load on load cell 1
 * @returns mechanical load from load cell 1 (after op-amp gain) (TODO: add units)
 */
float io_loadCell_getMechanicalLoad1(void);

/**
 * getter for mechincal load on load cell 2
 * @returns mechanical load from load cell 2 (after op-amp gain) (TODO: add units)
 */
float io_loadCell_getMechanicalLoad2(void);

/**
 * ocsc load cell 1 check
 * @returns checks open and short circuit for load cell 1
 */
bool io_loadCell_sensor1OCSC(void);

/**
 * ocsc load cell 2 check
 * @returns checks open and short circuit for load cell 2
 */
bool io_loadCell_sensor2OCSC(void);