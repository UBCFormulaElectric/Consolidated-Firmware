#include "io_leds.hpp"
#include "hw_gpios.hpp" //Not yet created

const Binary brake_light = { .gpio = &brake_light_en_pin }
const Binary led         = { .gpio = &led_pin }

//Not sure why error