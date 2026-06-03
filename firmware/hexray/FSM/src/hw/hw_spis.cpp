#include <cassert>
#include "hw_spis.hpp"
#include "hw_gpios.hpp"
#include "main.h"

namespace hw::spi
{

static bus   spi1(hspi1);
const device imu_spi(spi1, imu_cs, 100U);

[[nodiscard]] const bus &getBusFromHandle(const SPI_HandleTypeDef *handle)
{
    assert(handle == &hspi1);
    return spi1;
}
} // namespace hw::spi