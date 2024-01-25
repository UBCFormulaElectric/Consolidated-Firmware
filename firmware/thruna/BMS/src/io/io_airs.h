#pragma once

#include <stdbool.h>
#include "App_SharedMacros.h"

#ifdef TARGET_EMBEDDED
#include "hw_gpio.h"
#include "hw_adc.h"

typedef struct
{
    const Gpio       air_p_gpio;
    const Gpio       air_n_gpio;
    const Gpio       precharge_gpio;
    const AdcChannel loop_isense_channel;
} AirsConfig;
#else
EMPTY_STRUCT(AirsConfig);
#endif

/**
 * Initialize the AIRs driver.
 * @param therm_config Config struct.
 */
void io_airs_init(const AirsConfig *airs_config);

/**
 * Check if the AIR- is closed
 * @return true if AIR- is closed. Else, false.
 */
bool io_airs_isNegativeClosed(void);

/**
 * Close AIR+
 */
void io_airs_closePositive(void);

/**
 * Open AIR+
 */
void io_airs_openPositive(void);

/**
 * Check if the AIR+ is closed
 * @return true if AIR+ is closed. Else, false.
 */
bool io_airs_isPositiveClosed(void);

/**
 * Close precharge relay
 */
void io_airs_closePrecharge(void);

/**
 * Open precharge relay
 */
void io_airs_openPrecharge(void);

/**
 * Check if the precharge relay is closed
 * @return true if precharge relay is closed. Else, false.
 */
bool io_airs_isPrechargeClosed(void);
