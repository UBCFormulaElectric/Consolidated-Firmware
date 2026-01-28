#include "hw_spis.hpp"
#include "main.h"
#include <cstddef>
#include "hw_gpios.hpp"

static hw::spi::SpiBus pwr_chg(hspi1);
static hw::spi::SpiBus leds(hspi2);
static hw::spi::SpiBus seven_seg(hspi3);

hw::spi::SpiDevice pwr_chg_device(pwr_chg, led_rck, 100); // No NSS pin for this device so should we bit bang?
hw::spi::SpiDevice leds_device(leds, led_rck, 100);
hw::spi::SpiDevice seven_seg_device(seven_seg, seven_seg_rck, 100);
