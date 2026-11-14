#include "app_controlsConfig.hpp"
#include "app_canRx.hpp"

namespace app::controlsConfig
{
controls::ControlsConfig fromCan()
{
    return {
        .regen_enabled = false,
        .drive_mode    = can_rx::CRIT_DriveMode_get(),
    };
}
} // namespace app::controlsConfig
