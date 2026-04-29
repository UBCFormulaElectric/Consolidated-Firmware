#include "hw_spis.hpp"
#include "main.h"
#include "hw_gpios.hpp"

#include <cassert>

namespace hw::spi
{
bus spi1(hspi1);
bus spi2(hspi2);

// timeouts??
device imu1(spi1, imu_cs1, 10);
device imu2(spi1, imu_cs2, 10);
device imu3(spi1, imu_cs3, 10);
// constexpr SpiDevice rpi(spi2, gpio::rpi_cs, 10); fix this after spi driver is changed

const bus &getBusFromHandle(const SPI_HandleTypeDef *handle)
{
    if (handle == &hspi1)
    {
        return spi1;
    }
    else if (handle == &hspi2)
    {
        return spi2;
    }
    // fallback
    assert(false);
    return spi1;
}
} // namespace hw::spi
