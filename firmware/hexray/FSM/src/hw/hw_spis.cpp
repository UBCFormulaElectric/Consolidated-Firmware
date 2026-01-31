#include "hw_spis.hpp"
#include <cassert>
#include "hw_gpios.hpp"
#include "main.h"

#define TIMEOUT (100U)

static hw::spi::SpiBus imu_spi_bus(hspi1);
hw::spi::SpiDevice     imu_spi(imu_spi_bus, hw::gpios::imu_nss, TIMEOUT);

hw::spi::SpiBus *hw_spi_getBusFromHandle(const SPI_HandleTypeDef *handle)
{
    assert(handle == &imu_spi_bus.handle);
    return &imu_spi_bus;
}
