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
        fan_en.writePin(true);

        if (!fan_pgood.readPin())
        {
            static uint8_t num_retries = 0;
            if (num_retries < MAX_RETRIES)
            {
                num_retries++;
                fan_en.writePin(false);
            }
        }
    }
    else
    {
        fan_en.writePin(false);
    }
}
} // namespace io::fans
