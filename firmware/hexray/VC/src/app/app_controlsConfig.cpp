#include "app_controlsConfig.hpp"
#include "app_canRx.hpp"

namespace app::controlsConfig
{
controls::ControlsConfig getConfig()
{
    return {
        .regen_enabled = can_rx::CRIT_RegenSwitch_get() == can_utils::SwitchState::ON,
        .tv_enabled    = can_rx::CRIT_TorqueVectoringSwitch_get() == can_utils::SwitchState::ON,
        .drive_mode    = can_rx::CRIT_DriveMode_get(),
    };
}
} // namespace app::controlsConfig
