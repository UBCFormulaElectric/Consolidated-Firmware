extern "C"
{
#include "app_canRx.h"
}
#include "app_startSwitch.hpp"

namespace app::startSwitch
{
enum class SwitchState : uint8_t
{
    SWITCH_OFF = 0,
    SWITCH_ON  = 1,
};
constexpr uint8_t SWITCH_OFF = 0;
bool              app_startSwitch_hasRisingEdge(void)
{
    static SwitchState last_switch_state  = SwitchState::SWITCH_OFF;
    const SwitchState  start_switch_state = app_canRx_CRIT_StartSwitch_get();

    const bool has_rising_edge =
        (start_switch_state == SwitchState::SWITCH_ON && last_switch_state == SwitchState::SWITCH_OFF);
    last_switch_state = start_switch_state;
    return has_rising_edge;
}
} // namespace app::startSwitch
