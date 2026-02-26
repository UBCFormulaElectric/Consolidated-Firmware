#include "hw_spis.hpp"
#include "main.h"
#include "hw_gpios.hpp"
#include <cassert>
#include <optional>

static hw::spi::SpiBus pwr_chg(hspi1);
static hw::spi::SpiBus leds(hspi2);
static hw::spi::SpiBus seven_seg(hspi3);

const hw::spi::SpiDevice pwr_chg_device(pwr_chg, led_rck, 100); // No NSS pin for this device so should we bit bang?
const hw::spi::SpiDevice leds_device(leds, std::nullopt, 100);
const hw::spi::SpiDevice seven_seg_device(seven_seg, std::nullopt, 100);

[[nodiscard]] const hw::spi::SpiBus &hw::spi::getBusFromHandle(const SPI_HandleTypeDef *handle)
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
