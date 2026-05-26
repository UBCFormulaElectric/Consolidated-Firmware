#include "io_tsim.hpp"
#include "io_tsimFake.hpp"

namespace
{
fakes::tsim::Color s_last_color = fakes::tsim::Color::OFF;
unsigned int       s_call_count = 0;
} // namespace

namespace io::tsim
{
void set_off()
{
    s_last_color = fakes::tsim::Color::OFF;
    ++s_call_count;
}

void set_red()
{
    s_last_color = fakes::tsim::Color::RED;
    ++s_call_count;
}

void set_green()
{
    s_last_color = fakes::tsim::Color::GREEN;
    ++s_call_count;
}
} // namespace io::tsim

namespace fakes::tsim
{
void reset()
{
    s_last_color = Color::OFF;
    s_call_count = 0;
}

Color lastColor()
{
    return s_last_color;
}

unsigned int callCount()
{
    return s_call_count;
}
} // namespace fakes::tsim
