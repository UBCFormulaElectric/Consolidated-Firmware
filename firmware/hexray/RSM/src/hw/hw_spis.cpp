#include <cassert>

#include "hw_spis.hpp"
#include "main.h"
#include "hw_gpios.hpp"

namespace hw::spi
{

static bus   spi3(hspi3);
const device imu_sd(spi3, imu_cs, 10);

[[nodiscard]] const bus &getBusFromHandle(const SPI_HandleTypeDef *handle)
{
    assert(handle == &hspi3);
    return spi3;
}
} // namespace hw::spi