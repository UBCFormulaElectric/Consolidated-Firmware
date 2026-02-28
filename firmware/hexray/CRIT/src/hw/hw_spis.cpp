#include "hw_spis.hpp"
#include "main.h"
#include "hw_gpios.hpp"
#include <cassert>
#include <optional>

namespace hw::spi{
static constexpr uint32_t SPI_TIMEOUT = 100U;

static SpiBus pwr_chg(hspi1);
static SpiBus leds(hspi2);
static SpiBus seven_seg(hspi3);

const SpiDevice pwr_chg_device(pwr_chg, std::nullopt, SPI_TIMEOUT); // No NSS pin for this device so should we bit bang?
const SpiDevice leds_device(leds, std::nullopt, SPI_TIMEOUT);
const SpiDevice seven_seg_device(seven_seg, std::nullopt, SPI_TIMEOUT);

[[nodiscard]] const SpiBus &getBusFromHandle(const SPI_HandleTypeDef *handle)
{
    assert(handle == &pwr_chg.handle || handle == &leds.handle || handle == &seven_seg.handle);
    if (handle == &pwr_chg.handle)
    {
        return pwr_chg;
    }
    else if (handle == &leds.handle)
    {
        return leds;
    }
    return seven_seg;
}
}
