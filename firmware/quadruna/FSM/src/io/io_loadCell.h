#pragma once
#include <stdbool.h>
#include "app_utils.h"

#ifdef TARGET_EMBEDDED
#include "hw_adc.h"

typedef struct
{
    AdcChannel cell_1;
    AdcChannel cell_2;
} LoadCellConfig;

#else
EMPTY_STRUCT(LoadCellConfig)
#endif

/**
 * Initialize adc pins for load cell
 * @param load_cell_config wrapper around adc pins for load cells
 */
void io_loadCell_init(const LoadCellConfig *load_cell_config);

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