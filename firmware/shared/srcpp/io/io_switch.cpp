#include "io_switch.hpp"

namespace io
{
void Switch::init()
{
    last_state_raw = pin.readPin();
    state          = last_state_raw;
}

bool Switch::isClosed()
{
    const bool raw       = pin.readPin();
    const bool debounced = last_state_raw == raw;
    last_state_raw       = raw;

    if (!debounced)
    {
        elapsed = 0;
        return state;
    }

    if (elapsed >= debounce_ticks)
    {
        state = last_state_raw;
        return state;
    }

    elapsed++;

    return state;
}
} // namespace io