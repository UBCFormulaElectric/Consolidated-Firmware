#include "app_states.hpp"
#include "app_bspdwarning.hpp"
#include "app_canTx.hpp"
#include "app_canRx.hpp"
#include "app_canAlerts.hpp"
#include "io_log.hpp"
#include "app_canUtils.hpp"
#include "app_canAlerts.hpp"
#include "app_inverter.hpp"
#include "app_powerManager.hpp"
#include "torque_vectoring/datatypes/torque_limits.hpp"

#include "app_powerManager.hpp"

using namespace app::can_utils;
using namespace app::inverter;
using namespace app::powerManager;
using namespace app::tv::datatypes::torque_limits;

static const app::powerManager::PowerManagerConfig power_manager_state = { .efuse_configs = { {
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
                                                                           } } };

static volatile float apps = 0.0f;

namespace app::states
{
static bool driveStatePassPreCheck()
{
    // check inverter warnings
    app::inverter::FaultCheck();

    // check board warnings
    if (app::can_alerts::AnyBoardHasWarning())
        return false;

    // check possibly other faults
    apps = app::can_rx::FSM_PappsMappedPedalPercentage_get();

    if (app::bspdWarning::checkSoftwareBspd(apps))
        return false;

    return true;
}

static void driveStateRunOnEntry()
{
    // enable inverters
    app::can_tx::VC_State_set(VCState::VC_DRIVE_STATE);
    updateConfig(power_manager_state);

    // Ensure inverters are enabled
    inverter_enable_toggle(true, true, true, true);

    set_torque_limit_negative(MAX_REGEN_TORQUE_Nm, MAX_REGEN_TORQUE_Nm, MAX_REGEN_TORQUE_Nm, MAX_REGEN_TORQUE_Nm);
    set_torque_limit_positive(
        MAX_TORQUE_REQUEST_Nm, MAX_TORQUE_REQUEST_Nm, MAX_TORQUE_REQUEST_Nm, MAX_TORQUE_REQUEST_Nm);
    send_torque(NO_TORQUE_Nm, NO_TORQUE_Nm, NO_TORQUE_Nm, NO_TORQUE_Nm);
}

static void driveStateRunOnTick100Hz(void)
{
    efuseProtocolTick_100Hz();

    if (!driveStatePassPreCheck())
    {
        send_torque(NO_TORQUE_Nm, NO_TORQUE_Nm, NO_TORQUE_Nm, NO_TORQUE_Nm);
        // TODO: set speed requests to 0 as well
        return;
    }

    // TODO: add driving algorithm handling here
    // just for spinning wheels
    const float torque = TORQUE_REQUEST(apps * MAX_TORQUE_REQUEST_Nm);
    send_torque(torque, torque, torque, torque);
}

static void driveStateRunOnExit(void)
{
    // disable inverters
    inverter_enable_toggle(false, false, false, false);
    set_torque_limit_negative(NO_TORQUE_Nm, NO_TORQUE_Nm, NO_TORQUE_Nm, NO_TORQUE_Nm);
    set_torque_limit_positive(NO_TORQUE_Nm, NO_TORQUE_Nm, NO_TORQUE_Nm, NO_TORQUE_Nm);
}

app::State drive_state = { .name              = "DRIVE",
                           .run_on_entry      = driveStateRunOnEntry,
                           .run_on_tick_1Hz   = nullptr,
                           .run_on_tick_100Hz = driveStateRunOnTick100Hz,
                           .run_on_exit       = driveStateRunOnExit };
} // namespace app::states
