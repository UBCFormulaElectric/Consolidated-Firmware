#pragma once
#include "io_led.h"
#include "hw_gpios.h"



void brake_light_set(bool val);

extern const io::binary_led brake_light{ hw::gpio::brake_light_en_pin };
extern const io::binary_led led{ hw::gpio::led_pin };


