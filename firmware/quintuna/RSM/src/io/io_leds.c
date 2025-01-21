#include "io_leds.h"

const BinaryLed brake_light = { .gpio = &brake_light_en_pin };
const BinaryLed led         = { .gpio = &led_pin };
