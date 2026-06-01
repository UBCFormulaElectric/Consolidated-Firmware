#include "io_buzzer.hpp"
#include "io_buzzerFake.hpp"

namespace
{
bool         s_enabled       = false;
unsigned int s_enable_count  = 0;
unsigned int s_disable_count = 0;
} // namespace

namespace io::buzzer
{
void enable()
{
    s_enabled = true;
    ++s_enable_count;
}

void disable()
{
    s_enabled = false;
    ++s_disable_count;
}
} // namespace io::buzzer

namespace fakes::buzzer
{
void reset()
{
    s_enabled       = false;
    s_enable_count  = 0;
    s_disable_count = 0;
}

bool isEnabled()
{
    return s_enabled;
}

unsigned int enableCount()
{
    return s_enable_count;
}

unsigned int disableCount()
{
    return s_disable_count;
}
} // namespace fakes::buzzer
