#include "io_switches.h"
#include "app_canTx.h"

static const Switches *switches = NULL;

void io_switches_init(const Switches *switches_in)
{
    switches = switches_in;
}

bool io_switches_launch_control_get(void)
{
    return io_switch_isClosed(switches->launch_control);
}

bool io_switches_torque_vectoring_get(void)
{
    return io_switch_isClosed(switches->torque_vectoring);
}

bool io_switches_regen_get(void)
{
    return io_switch_isClosed(switches->regen);
}

bool io_switches_telem_get(void)
{
    return io_switch_isClosed(switches->telem);
}

bool io_switches_push_drive_get(void)
{
    return io_switch_isClosed(switches->push_drive);
}
