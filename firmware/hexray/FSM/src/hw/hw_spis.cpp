#include "hw_spis.hpp"
#include <cassert>
#include "hw_gpios.hpp"
#include "main.h"

static constexpr uint32_t SPI_TIMEOUT = 100U;

static hw::spi::SpiBus   imu_spi_bus(hspi1);
const hw::spi::SpiDevice imu_spi{ imu_spi_bus, imu_nss, SPI_TIMEOUT };

hw::spi::SpiBus *hw_spi_getBusFromHandle(const SPI_HandleTypeDef *handle)
{
    assert(handle == &imu_spi_bus.handle);
    return &imu_spi_bus;
}
