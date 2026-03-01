#include "states/app_states.hpp"
#include "io_irs.hpp"
#include "app_canTx.hpp"
#include "app_canUtils.hpp"

namespace app::states
{
namespace driveState
{
    static void driveStateRunOnEntry()
    {
        ::app::can_tx::BMS_State_set(::app::can_utils::BmsState::BMS_DRIVE_STATE);
    }

    static void driveStateRunOnTick100Hz() {}

    static void driveStateRunOnExit()
    {
        // IR+ opens upon exiting drive state
        io::irs::setPositive(::app::can_utils::ContactorState::CONTACTOR_STATE_OPEN);
    }

} // namespace driveState

const ::app::State drive_state = {
    .name              = "DRIVE",
    .run_on_entry      = driveState::driveStateRunOnEntry,
    .run_on_tick_1Hz   = nullptr,
    .run_on_tick_100Hz = driveState::driveStateRunOnTick100Hz,
    .run_on_exit       = driveState::driveStateRunOnExit,
};
} // namespace app::states
