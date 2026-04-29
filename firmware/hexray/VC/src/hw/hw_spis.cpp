#include "hw_spis.hpp"
#include "main.h"
#include "hw_gpios.hpp"

#include <cassert>

const hw::spi::bus spi1(hspi1);
const hw::spi::bus spi2(hspi2);

// timeouts??
const hw::spi::device imu1(spi1, imu_cs1, 10);
const hw::spi::device imu2(spi1, imu_cs2, 10);
const hw::spi::device imu3(spi1, imu_cs3, 10);
// constexpr SpiDevice rpi(spi2, gpio::rpi_cs, 10); fix this after spi driver is changed

const hw::spi::bus &getBusFromHandle(const SPI_HandleTypeDef *handle)
{
    if (handle == &hspi1)
    {
        return spi1;
    }
    if (handle == &hspi2)
    {
        return spi2;
    }
    // fallback
    assert(false);
    return spi1;
}
