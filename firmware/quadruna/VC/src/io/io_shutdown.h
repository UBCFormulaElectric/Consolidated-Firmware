#pragma once
#include "app_utils.h"

#ifdef TARGET_EMBEDDED
#include "hw_gpio.h"
#include "hw_adc.h"

typedef struct
{
    const Gpio tsms_gpio;
    const Gpio pcm_gpio;
    const Gpio LE_stop_gpio;
    const Gpio RE_stop_gpio;
} ShutdownConfig;
#else
EMPTY_STRUCT(ShutdownConfig);
#endif

/*
 * Get the status of the tsms shutdown pin
 * @return the status of the tsms pin set in the shutdown config
 */
bool io_shutdown_hasTsmsFault(void);

/*
 * Get the status of the pcm shutdown pin
 * @return the status of the pcm pin set in the shutdown config
 */
bool io_shutdown_hasPcmFault(void);

/*
 * Get the status of the LEStop shutdown pin
 * @return the status of the LEStop pin set in the shutdown config
 */
bool io_shutdown_hasLEStopFault(void);

/*
 * Get the status of the REStop shutdown pin
 * @return the status of the REStop pin set in the shutdown config
 */
bool io_shutdown_hasREStopFault(void);

/*
 * Initialize config for shutdown gpio pins
 * @param config with shutdown gpio pins
 */
void io_shutdown_init(const ShutdownConfig *shutdown_config);