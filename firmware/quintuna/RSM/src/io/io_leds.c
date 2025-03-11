#include "io_leds.h"
#include "hw_gpios.h"

const BinaryLed brake_light = { .gpio = &brake_light_en_pin };
const BinaryLed led         = { .gpio = &led_pin };

static const BinaryLed *brake_light1 = NULL;

void io_brake_light_set(bool val)
{
    io_led_enable(brake_light1, val);
   // app_canTx_RSM_BrakeLight_set(val);
}