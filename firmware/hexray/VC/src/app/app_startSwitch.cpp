#include "app_startSwitch.hpp"
#include "app_canRx.hpp"
#include "app_canUtils.hpp"
namespace app::startSwitch
{

bool hasRisingEdge(void)
{
    static app::can_utils::SwitchState last_switch_state  = app::can_utils::SwitchState::SWITCH_OFF;
    const app::can_utils::SwitchState  start_switch_state = app::can_rx::CRIT_StartSwitch_get();

    const bool has_rising_edge =
        (start_switch_state == app::can_utils::SwitchState::SWITCH_ON &&
         last_switch_state == app::can_utils::SwitchState::SWITCH_OFF);
    last_switch_state = start_switch_state;
    return has_rising_edge;
}
} // namespace app::startSwitch
