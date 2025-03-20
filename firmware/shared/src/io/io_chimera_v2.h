#pragma once
#include "pb.h"

// This provides the defines required to determine which peripherals are active
// Note: this is required to not throw an error for the below include.
// There is a circular import here, so it doesn't matter it will all be included anyways
#ifdef STM32F412Rx
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_conf.h"
#elif defined(STM32H733xx)
#include "stm32h7xx_hal.h"
#include "stm32h7xx_hal_conf.h"
#else
#error Either STM32H733xx or STM32F412Rx must be defined, so that chimera knows which HAL config to use
#endif

#ifdef HAL_ADC_MODULE_ENABLED
#include "hw_adc.h"
#endif
#ifdef HAL_GPIO_MODULE_ENABLED
#include "hw_gpio.h"
#endif
#ifdef HAL_I2C_MODULE_ENABLED
#include "hw_i2c.h"
#endif
#ifdef HAL_SPI_MODULE_ENABLED
#include "hw_spi.h"
#endif

/**
 * @brief A flag set internally by chimera indicating if it is running, exposed to block other jobs.
 */
extern bool io_chimera_v2_enabled;

/**
 * @brief The main Chimera loop - will run chimera_v2 if and only if the USB device is connected.
 * @param gpio_tag The Protobuf-generated tag for the board's GPIO pins, defined in shared.pb.h.
 * @param gpio_conf A table of Protobuf-generated net names to Gpio pins.
 * @param adc_tag The Protobuf-generated tag for the board's ADC channels, defined in shared.pb.h.
 * @param adc_conf A table of Protobuf-generated net names to ADC channels.
 * @param i2c_tag The Protobuf-generated tag for the board's I2C devices, defined in shared.pb.h.
 * @param i2c_conf A table of Protobuf-generated net names to I2C busses.
 * @param spi_tag The Protobuf-generated tag for the board's SPI devices, defined in shared.pb.h.
 * @param spi_conf A table of Protobuf-generated net names to SPI devices.
 */
void io_chimera_v2_mainOrContinue(
#ifdef HAL_GPIO_MODULE_ENABLED
    pb_size_t   gpio_tag,
    const Gpio *gpio_conf[]
#endif
#ifdef HAL_ADC_MODULE_ENABLED
// determines if we need a comma here
#if defined(HAL_GPIO_MODULE_ENABLED)
    ,
#endif
    pb_size_t         adc_tag,
    const AdcChannel *adc_conf[]
#endif
#ifdef HAL_I2C_MODULE_ENABLED
// determines if we need a comma here
#if defined(HAL_GPIO_MODULE_ENABLED) || defined(HAL_ADC_MODULE_ENABLED)
    ,
#endif
    pb_size_t        i2c_tag,
    const I2cDevice *i2c_conf[]
#endif
#ifdef HAL_SPI_MODULE_ENABLED
// determines if we need a comma here
#if defined(HAL_GPIO_MODULE_ENABLED) || defined(HAL_ADC_MODULE_ENABLED) || defined(HAL_I2C_MODULE_ENABLED)
    ,
#endif
    pb_size_t        spi_tag,
    const SpiDevice *spi_conf[]
#endif
);