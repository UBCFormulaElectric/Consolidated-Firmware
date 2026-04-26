#include "io_switch.hpp"

using namespace io;

[[nodiscard]] bool Switch::isClosed()
{
    const bool raw       = last_state_raw;
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

void Switch::setState(const bool state)
{
    last_state_raw = state;
}