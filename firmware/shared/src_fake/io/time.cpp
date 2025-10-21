#include "io/timeFake.hpp"

static uint32_t current_time_ms = 0;

extern "C"
{
#include "io/time.h"
    uint32_t io_time_getCurrentMs()
    {
        return current_time_ms;
    }
    void io_time_delay(const uint32_t ms)
    {
        (void)ms;
    }
}

namespace fakes::time
{
void setTime(const uint32_t ms)
{
    current_time_ms = ms;
}
} // namespace fakes::time