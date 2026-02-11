#include <cstdint>
#include "io_timeFake.hpp"

static uint32_t current_time_ms = 0;

namespace io::time
{
uint32_t getCurrentMs()
{
    return current_time_ms;
}

void delay(uint32_t ms)
{
    (void)ms;
}

void delayUntil(uint32_t time)
{
    (void)time;
}
} // namespace io::time

namespace fakes::time
{
void setTimeFake(const uint32_t ms)
{
    current_time_ms = ms;
}
} // namespace fakes::time

extern "C"
{
    uint32_t io_time_getCurrentMs(void)
    {
        return current_time_ms;
    }

    void io_time_delay(uint32_t ms)
    {
        (void)ms;
    }

    void fake_io_time_getCurrentMs_returns(uint32_t ms)
    {
        current_time_ms = ms;
    }
}