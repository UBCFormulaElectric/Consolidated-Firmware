#include "hw_spis.hpp"
#include <cassert>
#include "hw_gpios.hpp"
#include "main.h"

namespace hw::spi
{
constexpr uint32_t TIMEOUT = 100U;

static SpiBus imu_spi_bus(hspi1);
SpiDevice     imu_spi(imu_spi_bus, hw::gpios::imu_nss, TIMEOUT);

SpiBus *hw_spi_getBusFromHandle(const SPI_HandleTypeDef *handle)
{
    assert(handle == &imu_spi_bus.handle);
    return &imu_spi_bus;
}
} // namespace hw::spi
