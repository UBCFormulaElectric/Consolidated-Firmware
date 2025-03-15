#pragma once
#include <stdlib.h>
#include "pb.h"
#include "hw_adc.h"
#include "hw_gpio.h"
#include "hw_i2c.h"

/**
 * @brief A flag set internally by chimera indicating if it is running, exposed to block other jobs.
 */
extern bool io_chimera_v2_enabled;

/**
 * @brief The main Chimera loop - will run chimera_v2 if and only if the USB device is connected.
 * @param gpio_tag The Protobuf-generated tag for the board's GPIO pins, defined in shared.pb.h.
 * @param id_to_gpio A table of Protobuf-generated net names to Gpio pins.
 * @param adc_tag The Protobuf-generated tag for the board's ADC channels, defined in shared.pb.h.
 * @param id_to_adc A table of Protobuf-generated net names to ADC channels.
 * @param i2c_tag The Protobuf-generated tag for the board's I2C devices, defined in shared.pb.h.
 * @param id_to_i2c A table of Protobuf-generated net names to I2C busses.
 */
void io_chimera_v2_mainOrContinue(
    pb_size_t         gpio_tag,
    const Gpio       *id_to_gpio[],
    pb_size_t         adc_tag,
    const AdcChannel *id_to_adc[],
    pb_size_t         i2c_tag,
    const I2cDevice  *id_to_i2c[]);