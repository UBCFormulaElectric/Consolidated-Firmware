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

using namespace app::can_utils;
using namespace app::inverter;
using namespace app::powerManager;
using namespace app::tv::datatypes::torque_limits;

// TODO: Why does this need to be a struct??? we can just pass the reference of the array
static PowerManagerConfig pwr_mgr_cfg{ .efuse_configs = { {
                                           EfuseConfig{ .efuse_enable = true, .timeout = 0, .max_retry = 5 },
                                           EfuseConfig{ .efuse_enable = true, .timeout = 0, .max_retry = 5 },
                                           EfuseConfig{ .efuse_enable = true, .timeout = 0, .max_retry = 5 },
                                           EfuseConfig{ .efuse_enable = true, .timeout = 0, .max_retry = 5 },
                                           EfuseConfig{ .efuse_enable = true, .timeout = 0, .max_retry = 5 },
                                           EfuseConfig{ .efuse_enable = true, .timeout = 0, .max_retry = 5 },
                                           EfuseConfig{ .efuse_enable = true, .timeout = 0, .max_retry = 5 },
                                           EfuseConfig{ .efuse_enable = true, .timeout = 0, .max_retry = 5 },
                                           EfuseConfig{ .efuse_enable = true, .timeout = 200, .max_retry = 5 },
                                           EfuseConfig{ .efuse_enable = true, .timeout = 200, .max_retry = 5 },
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

    // handle state transitions away from drive state
    // is this really needed ngl this could be consolidated into 100Hz tick
    return true;
}

static void driveStateRunOnEntry()
{
    // enable inverters
    app::can_tx::VC_State_set(VCState::VC_DRIVE_STATE);
    updateConfig(pwr_mgr_cfg);

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
