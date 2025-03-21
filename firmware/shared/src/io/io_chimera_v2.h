#pragma once
#include <stdlib.h>
#include "pb.h"
#include "hw_adc.h"
#include "hw_gpio.h"
#include "hw_i2c.h"
#include "hw_spi.h"

// Configuration needed ro run chimera.
// Exposes mappings from net names to peripherals,
// and board-specific tags for each peripheral type.
typedef struct
{
#ifdef HAL_GPIO_MODULE_ENABLED
    // The Protobuf-generated tag for the board's GPIO pins, defined in shared.pb.h.
    pb_size_t gpio_net_name_tag;

    // A table of Protobuf-generated net names to GPIO peripherals.
    const Gpio **id_to_gpio;
#endif

#ifdef HAL_ADC_MODULE_ENABLED
    // The Protobuf-generated tag for the board's ADC pins, defined in shared.pb.h.
    pb_size_t adc_net_name_tag;

    // A table of Protobuf-generated net names to ADC peripherals.
    const AdcChannel **id_to_adc;
#endif

#ifdef HAL_I2C_MODULE_ENABLED
    // The Protobuf-generated tag for the board's I2C pins, defined in shared.pb.h.
    pb_size_t i2c_net_name_tag;

    // A table of Protobuf-generated net names to I2C peripherals.
    const I2cDevice **id_to_i2c;
#endif

#ifdef HAL_SPI_MODULE_ENABLED
    // The Protobuf-generated tag for the board's SPI pins, defined in shared.pb.h.
    pb_size_t spi_net_name_tag;

    // A table of Protobuf-generated net names to SPI peripherals.
    const SpiDevice **id_to_spi;
#endif
} io_chimera_v2_Config;

/**
 * @brief A flag set internally by chimera indicating if it is running, exposed to block other jobs.
 */
extern bool io_chimera_v2_enabled;

/**
 * @brief The main Chimera loop - will run chimera_v2 if and only if the USB device is connected.
 * @param config Collection of protobuf enum to peripheral tables, and net name tags.
 */
void io_chimera_v2_mainOrContinue(io_chimera_v2_Config *config);