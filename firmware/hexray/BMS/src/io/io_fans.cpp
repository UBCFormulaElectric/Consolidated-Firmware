#include "io_fans.hpp"
#include "hw_gpio.hpp"
#include "hw_gpios.hpp"
#include <stdint.h>

constexpr uint8_t MAX_RETRIES = 5;
namespace io::fans
{

void tick(bool enable)
{
    if (enable)
    {
        hw::Gpio::writePin(&hw::gpios::fan_en_pin, true);

        if (!hw::Gpio::readPin(&hw::gpios::fan_pgood_pin))
        {
            static uint8_t num_retries = 0;
            if (num_retries < MAX_RETRIES)
            {
                num_retries++;
                hw::Gpio::writePin(&hw::gpios::fan_en_pin, false);
            }
        }
    }
    else
    {
        hw::Gpio::writePin(&hw::gpios::fan_en_pin, false);
    }
}
} // namespace io::fans