#pragma once
#include <stdlib.h>
#include "pb.h"
#include "hw_adc.h"
#include "hw_gpio.h"
#include "hw_i2cs.h"

/**
 * @brief The main Chimera loop.
 * @param gpio_tag The Protobuf-generated tag for the board's GPIO pins, defined in shared.pb.h.
 * @param id_to_gpio A table of Protobuf-generated net names to Gpio pins.
 * @param adc_tag The Protobuf-generated tag for the board's ADC channels, defined in shared.pb.h.
 * @param id_to_adc A table of Protobuf-generated net names to ADC channels.
 * @param i2c_tag The Protobuf-generated tag for the board's I2C devices, defined in shared.pb.h.
 * @param id_to_i2c A table of Protobuf-generated net names to I2C busses.
 */
void io_chimera_v2_main(
    pb_size_t         gpio_tag,
    const Gpio       *id_to_gpio[],
    pb_size_t         adc_tag,
    const AdcChannel *id_to_adc[],
    pb_size_t         i2c_tag,
    const I2cDevice  *id_to_i2c[]);