#include "app_startSwitch.hpp"
extern "C"
{
#include "app_canRx.h"
}

namespace app::startSwitch
{

bool hasRisingEdge(void)
{
    static SwitchState last_switch_state  = SwitchState::SWITCH_OFF;
    const SwitchState  start_switch_state = app::can::rx::CRIT_StartSwitch_get();

    const bool has_rising_edge =
        (start_switch_state == SwitchState::SWITCH_ON && last_switch_state == SwitchState::SWITCH_OFF);
    last_switch_state = start_switch_state;
    return has_rising_edge;
}
} // namespace app::startSwitch
