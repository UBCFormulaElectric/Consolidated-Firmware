#pragma once
#include <stdbool.h>
#include "app_utils.h"

#ifdef TARGET_EMBEDDED
#include "hw_adc.h"

typedef struct
{
    AdcChannel front_left_suspension;
    AdcChannel front_right_suspension;
} SuspensionConfig;

#else
EMPTY_STRUCT(SuspensionConfig)
#endif

/**
 * Set up adc pins for suspension sensors
 * @param suspension_config wrapper around ADC pins for suspension sensors
 */
void io_suspension_init(SuspensionConfig *suspension_config);

/**
 * Get the travel of the front left suspension
 * @return The travel of the suspension in (TODO: add units)
 */
float io_suspension_getLeftTravel(void);

/**
 * Get the travel of the front right suspension
 * @return The travel of the suspension in (TODO: add units)
 */
float io_suspension_getRightTravel(void);

/**
 * @return whether or not the left suspension sensor is open or short circuit
 */
bool io_suspension_leftSensorOCSC(void);

/**
 * @return whether or not the right suspension sensor is open or short circuit
 */
bool io_suspension_rightSensorOCSC(void);