#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "App_SharedMacros.h"

#ifdef TARGET_EMBEDDED
#include "hw_gpio.h"
#include "hw_adc.h"

typedef struct
{
    const Gpio       mux_0_gpio;
    const Gpio       mux_1_gpio;
    const Gpio       mux_2_gpio;
    const Gpio       mux_3_gpio;
    const AdcChannel thermistor_adc_channel;
} ThermistorsConfig;
#else
EMPTY_STRUCT(ThermistorsConfig);
#endif

/**
 * Initialize the thermistors driver.
 * @param therm_config Config struct.
 */
void io_thermistors_init(const ThermistorsConfig* therm_config);

/**
 * Change the selected Mux Channel
 * @param channel to change to
 */
void io_thermistors_muxSelect(uint8_t channel);

/**
 * Read the temperature of the current channel
 * @return Thermistor temperature in degrees C
 */
float io_thermistors_readSelectedTemp(void);
