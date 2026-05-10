#include "hw_spis.hpp"
#include "main.h"
#include "hw_gpios.hpp"

#include <cassert>
#include <optional>

namespace hw::spi
{

static constexpr uint32_t SPI_TIMEOUT = 100U;

static constexpr hw::spi::bus spi1(hspi1);
static constexpr hw::spi::bus spi2(hspi2);

// timeouts??
const hw::spi::device imu1(spi1, imu_cs1, SPI_TIMEOUT);
const hw::spi::device imu2(spi1, imu_cs2, SPI_TIMEOUT);
const hw::spi::device imu3(spi1, imu_cs3, SPI_TIMEOUT);

[[nodiscard]] const hw::spi::bus &getBusFromHandle(const SPI_HandleTypeDef *handle)
{
    assert(handle == &spi1.handle || handle == &spi2.handle);

    if (handle == &spi1.handle)
    {
        return spi1;
    }
    if (handle == &spi2.handle)
    {
        return spi2;
    }
    return spi1;
}
} // namespace hw::spi