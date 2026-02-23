#include <cassert>
#include "hw_spis.hpp"
#include "hw_gpios.hpp"
#include "main.h"

namespace hw::spi
{
static constexpr uint32_t SPI_TIMEOUT = 100U;

static SpiBus   imu_spi_bus(hspi1);
const SpiDevice imu_spi(imu_spi_bus, imu_nss, SPI_TIMEOUT);

[[nodiscard]] const SpiBus &getBusFromHandle(const SPI_HandleTypeDef *handle)
{
    assert(handle == &imu_spi_bus.handle);
    return imu_spi_bus;
}
}