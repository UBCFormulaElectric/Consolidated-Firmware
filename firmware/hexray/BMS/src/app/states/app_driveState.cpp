#include "states/app_states.hpp"
#include "io_irs.hpp"

extern "C"
{
#include "app_canTx.h"
#include "app_canUtils.h"
}

namespace app::states::driveState
{
static void driveStateRunOnEntry()
{
    app_canTx_BMS_State_set(BMS_DRIVE_STATE);
}

static void driveStateRunOnTick100Hz() {}

static void driveStateRunOnExit()
{
    // IR+ opens upon exiting drive state
    io::irs::setPositive(CONTACTOR_STATE_OPEN);
}

} // namespace app::states::driveState

const State drive_state = {
    .name              = "DRIVE",
    .run_on_entry      = app::states::driveState::driveStateRunOnEntry,
    .run_on_tick_100Hz = app::states::driveState::driveStateRunOnTick100Hz,
    .run_on_exit       = app::states::driveState::driveStateRunOnExit,
};