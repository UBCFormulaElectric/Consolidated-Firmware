#include "io_time.hpp"

static uint32_t current_time_ms = 0;

#include "io_time.hpp"
// TODO: PLZ FIX THIS
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
void setFakeTime(const uint32_t ms)
{
    current_time_ms = ms;
}
} // namespace fakes::time