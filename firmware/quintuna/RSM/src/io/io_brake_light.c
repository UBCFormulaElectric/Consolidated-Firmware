#include "io_brake_light.h"
#include "app_canTx.h"

static const BinaryLed *brake_light = NULL;

void io_brake_light_init(const BinaryLed *brake_light_in)
{
    brake_light = brake_light_in;
}

// tests on the can signal sending 1 when true and 0 when false were failing (can signal always set to 0) thus
// explicitly sending the desired input
void io_brake_light_set(bool val)
{
    io_led_enable(brake_light, val);
    app_canTx_RSM_BrakeLight_set(val);
}
