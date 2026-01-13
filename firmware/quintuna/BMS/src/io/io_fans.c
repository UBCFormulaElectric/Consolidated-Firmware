#include "io_fans.h"
#include "hw_gpio.h"
#include "hw_gpios.h"

#define MAX_RETRIES (5U)

static uint32_t num_retries = 0;

void io_fans_tick(bool enable)
{
    if (enable)
    {
        hw_gpio_writePin(&fan_en_pin, true);

        // PGOOD = 0 means the load switch has faulted. If so, retry it by disabling it (it'll be re-enabled on the next
        // tick).
        if (!hw_gpio_readPin(&fan_pgood_pin) && num_retries < MAX_RETRIES)
        {
            num_retries++;
            hw_gpio_writePin(&fan_en_pin, false);
        }
    }
    else
    {
        hw_gpio_writePin(&fan_en_pin, false);
    }
}
