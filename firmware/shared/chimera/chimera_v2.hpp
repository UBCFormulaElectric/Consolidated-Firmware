#pragma once
#include <optional>
#include <functional>

extern "C"
{
#include "main.h"
#include "pb.h"
#include "shared.pb.h"
}

#ifdef HAL_GPIO_MODULE_ENABLED
#include "hw_gpio.hpp"
#endif

#ifdef HAL_ADC_MODULE_ENABLED
#include "hw_adc.hpp"
#endif

#ifdef HAL_I2C_MODULE_ENABLED
#include "hw_i2c.hpp"
#endif

#ifdef HAL_SPI_MODULE_ENABLED
#include "hw_spi.hpp"
#endif

namespace hw::chimera_v2
{
// Configuration needed ro run chimera.
// Exposes mappings from net names to peripherals,
// and board-specific tags for each peripheral type.
class config
{
  public:
    virtual ~config() = default;

#ifdef HAL_GPIO_MODULE_ENABLED
  protected:
    // The Protobuf-generated tag for the board's GPIO pins, defined in shared.pb.h.
    pb_size_t gpio_net_name_tag{};

  public:
    // A table of Protobuf-generated net names to GPIO peripherals.
    virtual std::optional<std::reference_wrapper<const Gpio>> id_to_gpio(const _GpioNetName *gnn) const;
#endif

#ifdef HAL_ADC_MODULE_ENABLED
  protected:
    // The Protobuf-generated tag for the board's ADC pins, defined in shared.pb.h.
    pb_size_t adc_net_name_tag{};

  public:
    // A table of Protobuf-generated net names to ADC peripherals.
    virtual std::optional<std::reference_wrapper<const hw::Adc>> id_to_adc(const _AdcNetName *ann) const;
#endif

#ifdef HAL_I2C_MODULE_ENABLED
  protected:
    // The Protobuf-generated tag for the board's I2C pins, defined in shared.pb.h.
    pb_size_t i2c_net_name_tag{};

  public:
    // A table of Protobuf-generated net names to I2C peripherals.
    // virtual const hw::i2c::I2CDevice &id_to_i2c();
    virtual std::optional<std::reference_wrapper<const i2c::I2CDevice>> id_to_i2c(const _I2cNetName *inn) const;
#endif

#ifdef HAL_SPI_MODULE_ENABLED
  protected:
    // The Protobuf-generated tag for the board's SPI pins, defined in shared.pb.h.
    pb_size_t spi_net_name_tag{};

  public:
    // A table of Protobuf-generated net names to SPI peripherals.
    virtual std::optional<std::reference_wrapper<const hw::spi::SpiDevice>> id_to_spi(const _SpiNetName *snn) const;
#endif
};

/**
 * @brief A flag set internally by chimera indicating if it is running, exposed to block other jobs.
 */
extern bool enabled;

/**
 * @brief The main Chimera task loop, should be ran in it's own thread.
 * @param c Collection of protobuf enum to peripheral tables and net name tags.
 */
_Noreturn void task(const config &c);
} // namespace hw::chimera_v2
