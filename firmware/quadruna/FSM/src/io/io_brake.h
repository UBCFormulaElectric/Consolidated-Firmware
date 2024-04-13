#pragma once

#include <stdbool.h>
#include "app_utils.h"

#ifdef TARGET_EMBEDDED
#include "hw_adc.h"

typedef struct
{
    AdcChannel rear_brake;
    AdcChannel front_brake;
} BrakeConfig;

#else
EMPTY_STRUCT(BrakeConfig)
#endif

/**
 * Sets up configuration for ADC pins in brake io layer
*/
void io_brake_init(BrakeConfig *brakes_config);

/**
 * Check if the brake is actuated, by getting the digital signal from the comparator on the FSM
 * @return True if the brake is actuated, false if the brake is not actuated
 */
bool io_brake_isActuated(void);

/**
 * Get the pressure in psi from the MSP-300-2K5-P-3-N-1 brake pressure sensor
 * @return The pressure of the primary brake pressure sensor in psi
 */
float io_brake_getFrontPressurePsi(void);

/**
 * @return whether there is a open or short circuit in the front pressure sensor
 */
bool io_brake_frontPressureSensorOCSC(void);

/**
 * Get the pressure in psi from the [REAR BRAKE PRESSURE SENSOR]
 * @return The pressure of the secondary brake pressure sensor in psi
 */
float io_brake_getRearPressurePsi(void);

/**
 * @return whether or not the rear pressure sensor is open or short circuit
 */
bool io_brake_rearPressureSensorOCSC(void);