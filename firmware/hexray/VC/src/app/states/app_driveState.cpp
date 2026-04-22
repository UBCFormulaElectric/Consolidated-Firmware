#include "app_stateMachine.hpp"
#include "app_states.hpp"
#include "torque_vectoring/driving_algorithm.hpp"
#include "app_bspdWarning.hpp"
#include "app_canTx.hpp"
#include "app_canRx.hpp"
#include "app_canUtils.hpp"

using namespace app::can_utils;

namespace app::states
{
static bool driveStatePassPreCheck()
{
// TODO:
// check inverter warnings
// check board warnings
// check possibly other faults 
// check switches and debounce them
// check bspd
// handle state transitions away from drive state
// is this really needed ngl this could be consolidated into 100Hz tick
return true;
}

/*
static SwitchState read_tv_switch()
{
    // TODO: implement
    static SwitchState last_state = SwitchState::SWITCH_OFF;
    return last_state;
}

static SwitchState read_regen_switch()
{
    // TODO: implement
    static SwitchState last_state = SwitchState::SWITCH_OFF;
    return last_state;
}

static SwitchState read_launch_switch()
{
    // TODO: implement
    static SwitchState last_state = SwitchState::SWITCH_OFF;
    return last_state;
}
*/

static void driveStateRunOnEntry()
{
// TODO:
// enable inverters
    app::can_tx::VC_State_set(VCState::VC_INIT_STATE);

    // Ensure inverters are enabled
    inverter_enable_toggle(true, true, true, true);

    set_torque_limit_negative(MAX_REGEN_TORQUE_Nm, MAX_REGEN_TORQUE_Nm, MAX_REGEN_TORQUE_Nm, MAX_REGEN_TORQUE_Nm);
    set_torque_limit_positive(
        MAX_TORQUE_REQUEST_Nm, MAX_TORQUE_REQUEST_Nm, MAX_TORQUE_REQUEST_Nm, MAX_TORQUE_REQUEST_Nm);
}

static void driveStateRunOnTick100Hz(void)
{
// TODO:
    if (!driveStatePassPreCheck())
        return;
// run driving algo
}

static void driveStateRunOnExit(void)
{
// TODO:
// disable inverters
}

app::State drive_state = { .name              = "DRIVE",
                           .run_on_entry      = driveStateRunOnEntry,
                           .run_on_tick_1Hz   = nullptr,
                           .run_on_tick_100Hz = driveStateRunOnTick100Hz,
                           .run_on_exit       = driveStateRunOnExit };
}