#include "io_switch.h"
#include "hw_gpios.h"

#define DEBOUNCE_DELAY_MS 50

extern Switch push_drive_switch;
extern Switch regen_switch;
extern Switch vanilla_override_switch;
extern Switch launch_control_switch;
extern Switch telem_switch;

static void io_switch_init(Switch *sw)
{
    GPIO_PinState pin      = HAL_GPIO_ReadPin(sw->gpio->port, sw->gpio->pin);
    bool          raw      = (pin == (sw->closed_state ? GPIO_PIN_SET : GPIO_PIN_RESET));
    sw->last_raw_state     = raw;
    sw->debounced_state    = raw;
    sw->last_debounce_time = HAL_GetTick();
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
    GPIO_PinState pin = HAL_GPIO_ReadPin(sw->gpio->port, sw->gpio->pin);
    bool          raw = (pin == (sw->closed_state ? GPIO_PIN_SET : GPIO_PIN_RESET));

    if (raw != sw->last_raw_state)
    {
        sw->last_debounce_time = HAL_GetTick();
        sw->last_raw_state     = raw;
    }

    if ((HAL_GetTick() - sw->last_debounce_time) >= DEBOUNCE_DELAY_MS)
    {
        sw->debounced_state = raw;
    }

    return sw->debounced_state;
}
