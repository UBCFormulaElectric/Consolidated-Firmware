#pragma once
#include "main.h"
#include "pb.h"

#ifdef HAL_GPIO_MODULE_ENABLED
#include "hw_gpio.h"
#endif

#ifdef HAL_ADC_MODULE_ENABLED
#include "hw_adc.h"
#endif

#ifdef HAL_I2C_MODULE_ENABLED
#include "hw_i2c.h"
#endif

#ifdef HAL_SPI_MODULE_ENABLED
#include "hw_spi.h"
#endif

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
} hw_chimera_v2_Config;

/**
 * @brief A flag set internally by chimera indicating if it is running, exposed to block other jobs.
 */
extern bool hw_chimera_v2_enabled;

/**
 * @brief The main Chimera task loop, should be ran in it's own thread.
 * @param config Collection of protobuf enum to peripheral tables and net name tags.
 */
_Noreturn void hw_chimera_v2_task(const hw_chimera_v2_Config *config);
