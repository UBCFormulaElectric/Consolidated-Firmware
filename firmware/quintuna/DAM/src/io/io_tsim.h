#pragma once

#include <stdbool.h>

#ifdef TARGET_EMBEDDED
#include "hw_gpio.h"

typedef struct
{
    const Gpio       *tsim_red_en;
    const Gpio       *ntsim_green_en;
} TsimLightConfig;

#else
#include "app_utils.h"
EMPTY_STRUCT(TsimLightConfig)
#endif

/**
 * @param tsim_config wrapper around tsim pins
 */
void io_tsim_init(const TsimLightConfig *stim_config);

/**
 * Turn on the red light on the TSIM
 */
void io_tsim_set_red_high(void);

/**
 * Turn off the red light on the TSIM
 */
void io_tsim_set_red_low(void);

/**
 * Turn on the green light on the TSIM
 */
void io_tsim_set_green_high(void);

/**
 * Turn off the green light on the TSIM
 */
void io_tsim_set_green_low(void);