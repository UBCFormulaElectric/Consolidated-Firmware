#include "app_stateMachine.hpp"
#include "app_states.hpp"
#include "app_bspdwarning.hpp"
#include "torque_vectoring/driving_algorithm.hpp"
#include "app_bspdWarning.hpp"
#include "app_canTx.hpp"
#include "app_canRx.hpp"
#include "app_canUtils.hpp"
#include "app_canAlerts.hpp"

using namespace app::can_utils;

static volatile float apps = 0.0f;

// TODO: add power manager

namespace app::states
{
static bool driveStatePassPreCheck()
{
    // TODO:
    // check inverter warnings

    // check board warnings
    if (app::can_alerts::AnyBoardHasWarning())
        return false;

    // check possibly other faults
    apps = app::can_rx::FSM_PappsMappedPedalPercentage_get();

    if (app::bspdWarning::checkSoftwareBspd(apps))
        return false;

    // handle state transitions away from drive state
    // is this really needed ngl this could be consolidated into 100Hz tick
    return true;
}

static void driveStateRunOnEntry()
{
    // TODO:
    // enable inverters
    app::can_tx::VC_State_set(VCState::VC_DRIVE_STATE);

    // Ensure inverters are enabled
    inverter_enable_toggle(true, true, true, true);

    set_torque_limit_negative(MAX_REGEN_TORQUE_Nm, MAX_REGEN_TORQUE_Nm, MAX_REGEN_TORQUE_Nm, MAX_REGEN_TORQUE_Nm);
    set_torque_limit_positive(
        MAX_TORQUE_REQUEST_Nm, MAX_TORQUE_REQUEST_Nm, MAX_TORQUE_REQUEST_Nm, MAX_TORQUE_REQUEST_Nm);
    send_torque(NO_TORQUE_Nm, NO_TORQUE_Nm, NO_TORQUE_Nm, NO_TORQUE_Nm);
}

static void driveStateRunOnTick100Hz(void)
{
    // TODO:
    if (!driveStatePassPreCheck())
    {
        send_torque(NO_TORQUE_Nm, NO_TORQUE_Nm, NO_TORQUE_Nm, NO_TORQUE_Nm);
        // TODO: set speed requests to 0 as well
        return;
    }

    app::tv::algo::run(apps);
}

static void driveStateRunOnExit(void)
{
    // TODO:
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