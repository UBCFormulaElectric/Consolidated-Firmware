#include "io_leds.h"
#include "hw_gpios.h"

const BinaryLed brake_light = { .gpio = &brake_light_en_pin };
const BinaryLed led         = { .gpio = &led_pin };
