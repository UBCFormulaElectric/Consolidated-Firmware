#include "app_states.hpp"
#include "app_canTx.hpp"
#include "app_canRx.hpp"
#include "app_canAlerts.hpp"
#include "io_log.hpp"
#include "app_canUtils.hpp"
#include "app_inverter.hpp"

#include "app_powerManager.hpp"

using namespace app::can_utils;

namespace app::states
{
namespace driveState
{

    static void runOnEntry(void)
    {
        static const app::powerManager::PowerManagerConfig power_manager_state = {
            .efuse_configs = {{
                { true, 200, 5 }, // rr_pump
                { true, 200, 5 }, // rl_pump
                { true, 200, 5 }, // r_rad_fan
                { true, 200, 5 }, // l_rad_fan
                { true, 0, 5 },   // f_inv
                { true, 0, 5 },   // r_inv
                { true, 0, 5 },   // rsm
                { true, 0, 5 },   // bms
                { true, 0, 5 },   // dam
                { true, 0, 5 },   // front
            }}
        };
        app::powerManager::updateConfig(power_manager_state);
    }


    static void runOnTick100Hz(void) {}

    static void runOnExit(void) {}

} // namespace driveState

State drive_state = { .name              = "Drive State",
                      .run_on_entry      = driveState::runOnEntry,
                      .run_on_tick_1Hz   = nullptr,
                      .run_on_tick_100Hz = driveState::runOnTick100Hz,
                      .run_on_exit       = driveState::runOnExit };

} // namespace app::states
