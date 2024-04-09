#pragma once

#include <stdbool.h>
#include "app_utils.h"

#ifdef TARGET_EMBEDDED
#include "hw_adc.h"

typedef struct
{
    AdcChannel apps1;
    AdcChannel apps2;
} AppsConfig;

#else
EMPTY_STRUCT(AppsConfig)
#endif
/**
 * Performs initial calculations for the rest and max angle of PAPPS and SAPPS
 * @param apps_config, a pointer to a struct w/ adc pins for apps1 and apps2
 */
void io_apps_init(AppsConfig *apps_config);

/**
 * Calculates pedal percentage by comparing the current angle to the max angle
 * @return pedal percentage based on primary accelerator pedal sensor
 */
float io_apps_getPrimary(void);

/**
 * @return whether or not the primary accelerator pedal sensor is open or short circuit
 */
bool io_apps_isPrimaryOCSC(void);

/**
 * Calculates pedal percentage by comparing the current angle to the max angle
 * @return pedal percentage based on secondary accelerator pedal sensor
 */
float io_apps_getSecondary(void);

/**
 * @return whether or not the secondary accelerator pedal sensor is open or short circuit
 */
bool io_apps_isSecondaryOCSC(void);
