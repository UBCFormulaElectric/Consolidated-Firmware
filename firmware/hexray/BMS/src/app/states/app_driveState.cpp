#include "app_states.hpp"
#include "io_irs.hpp"
#include "app_canTx.hpp"
#include "app_canUtils.hpp"
#include "app_tractiveSystem.hpp"
#include "app_timer.hpp"
#include "io_semaphore.hpp"
#include "app_segments.hpp"


constexpr float    TS_UNDERVOLTAGE_DELTA_V           = 20.0f;
constexpr float    TS_UNDERVOLTAGE_IMMEDIATE_DELTA_V = 50.0f;
constexpr uint32_t TS_UNDERVOLTAGE_DEBOUNCE_MS       = 500U;
namespace app::states
{
namespace driveState
{
    static app::Timer ts_undervoltage_debounce(TS_UNDERVOLTAGE_DEBOUNCE_MS);

    static void driveStateRunOnEntry()
    {
        app::can_tx::BMS_State_set(::app::can_utils::BmsState::BMS_DRIVE_STATE);
    }

    static void driveStateRunOnTick100Hz()
    {
        float ts_voltage_drop_v;
        io::unique_semaphore s{ shared_lock };
        {
        ts_voltage_drop_v = app::segments::shared::getPackVoltage() - app::ts::getVoltage();
        }
        const bool  ts_uv             = ts_voltage_drop_v < TS_UNDERVOLTAGE_DELTA_V;
        const bool  ts_uv_immediate   = ts_voltage_drop_v < TS_UNDERVOLTAGE_IMMEDIATE_DELTA_V;
        if (ts_undervoltage_debounce.runIfCondition(ts_uv) == app::Timer::TimerState::EXPIRED || ts_uv_immediate)
        {
            app::StateMachine::set_next_state(&init_state);
        }
    }

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
