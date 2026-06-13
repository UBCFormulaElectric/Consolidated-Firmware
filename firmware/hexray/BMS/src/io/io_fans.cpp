#include "io_fans.hpp"
#include "hw_gpios.hpp"
#include "app_timer.hpp"
#include <cstdint>

namespace io::fans
{
namespace
{
    constexpr uint8_t  MAX_RETRIES    = 5;
    constexpr uint32_t SETTLE_TIME_MS = 100;

    uint8_t    num_retries = 0;
    bool       fan_on      = false;
    app::Timer retry_timer{ SETTLE_TIME_MS };
} // namespace

void tick(const bool enable)
{
    if (!enable)
    {
        fan_en.writePin(false);
        fan_on      = false;
        num_retries = 0;
        retry_timer.stop();
        return;
    }

    if (retry_timer.updateAndGetState() == app::Timer::TimerState::RUNNING)
        return;

    if (!fan_on)
    {
        if (num_retries < MAX_RETRIES)
        {
            fan_en.writePin(true);
            fan_on = true;
            retry_timer.restart(); 
        }
        return;
    }

    if (!fan_pgood.readPin())
    {
        num_retries++;
        fan_en.writePin(false);
        fan_on = false;
        retry_timer.restart(); 
    }
}
} // namespace io::fans
