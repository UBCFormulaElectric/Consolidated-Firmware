#include "io_switches.h"
#include "app_canTx.h"

static const Switches *switches = NULL;

void io_switches_init(const Switches *switches_in)
{
    switches = switches_in;
}

bool io_switches_start_get(void)
{
    return io_switch_isClosed(switches->start_switch);
}

bool io_switches_regen_get(void)
{
    return io_switch_isClosed(switches->regen_switch);
}

bool io_switches_torquevec_get(void)
{
    return io_switch_isClosed(switches->torquevec_switch);
}
