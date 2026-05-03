#include "hw_spis.hpp"
#include "main.h"

#include <cassert>
#include <optional>

static constexpr uint32_t SPI_TIMEOUT = 100U;

static hw::spi::bus pwr_chg(hspi1);
static hw::spi::bus leds(hspi2);
static hw::spi::bus seven_seg(hspi3);

constexpr hw::spi::device
    pwr_chg_device(pwr_chg, std::nullopt, SPI_TIMEOUT); // No NSS pin for this device so should we bit bang?
constexpr hw::spi::device leds_device(leds, std::nullopt, SPI_TIMEOUT);
constexpr hw::spi::device seven_seg_device(seven_seg, std::nullopt, SPI_TIMEOUT);

[[nodiscard]] const hw::spi::bus &hw::spi::getBusFromHandle(const SPI_HandleTypeDef *handle)
{
    assert(handle == &pwr_chg.handle || handle == &leds.handle || handle == &seven_seg.handle);
    if (handle == &pwr_chg.handle)
    {
        return pwr_chg;
    }
    if (handle == &leds.handle)
    {
        return leds;
    }
    return seven_seg;
}