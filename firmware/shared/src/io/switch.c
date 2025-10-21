#include "io/switch.h"
#include "hw/gpio.h"
#include "hw/gpios.h"
#include "io/time.h"

#define DEBOUNCE_DELAY_MS 50

extern Switch push_drive_switch;
extern Switch regen_switch;
extern Switch vanilla_override_switch;
extern Switch launch_control_switch;
extern Switch telem_switch;

static void io_switch_init(Switch *sw)
{
    GPIO_PinState raw      = hw_gpio_readPin(sw->gpio);
    sw->last_raw_state     = raw;
    sw->debounced_state    = raw;
    sw->last_debounce_time = io_time_getCurrentMs();
}

void io_switch_initAll(void)
{
    io_switch_init(&push_drive_switch);
    io_switch_init(&regen_switch);
    io_switch_init(&vanilla_override_switch);
    io_switch_init(&launch_control_switch);
    io_switch_init(&telem_switch);
}

bool io_switch_isClosed(Switch *sw)
{
    bool raw = hw_gpio_readPin(sw->gpio);

    if (raw != sw->last_raw_state)
    {
        sw->last_debounce_time = io_time_getCurrentMs();
        sw->last_raw_state     = raw;
    }

    if ((io_time_getCurrentMs() - sw->last_debounce_time) >= DEBOUNCE_DELAY_MS)
    {
        sw->debounced_state = raw;
    }

    return sw->debounced_state;
}
