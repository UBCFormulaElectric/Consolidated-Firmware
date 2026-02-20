#include "hw_spis.hpp"
#include "main.h"
#include "hw_gpios.hpp"

#include <cassert>

namespace hw::spi
{
// TODO determine timeout_ms w/ datasheet of imu?
SpiBus    spi3(hspi3);
SpiDevice imu_sd(spi3, gpio::imu_cs, 10);

// should I write if (handle != &hspi) {assert(false); return nullptr} else {return &spi3} instead?
// or does this count as hard coding?
const SpiBus &getBusFromHandle(const SPI_HandleTypeDef *handle)
{
    assert(handle == &hspi3);
    return spi3;
}

} // namespace hw::spi