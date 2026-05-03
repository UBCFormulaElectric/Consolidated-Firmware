#include "io_timeFake.hpp"

static uint32_t current_time_ms = 0;

extern "C"
{
#include "io_time.h"
    uint32_t io_time_getCurrentMs()
    {
        return current_time_ms;
    }
    void io_time_delay(const uint32_t ms)
    {
        (void)ms;
    }
}
#include "io_time.hpp"
namespace io::time
{
uint32_t getCurrentMs()
{
    return current_time_ms;
}
void delay(const uint32_t ms)
{
    // TODO
}
void delayUntil(const uint32_t time)
{
    // TODO
}
} // namespace io::time

namespace fakes::time
{
void setTime(const uint32_t ms)
{
    current_time_ms = ms;
}
} // namespace fakes::time