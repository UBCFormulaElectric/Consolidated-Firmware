#include <cassert>

#include "hw_spis.hpp"
#include "main.h"
#include "hw_gpios.hpp"

namespace hw::spi
{

static SpiBus   spi3(hspi3);
const SpiDevice imu_sd(spi3, gpio::imu_cs, 10);

[[nodiscard]] const SpiBus &getBusFromHandle(const SPI_HandleTypeDef *handle)
{
    assert(handle == &hspi3);
    return spi3;
}
} // namespace hw::spi