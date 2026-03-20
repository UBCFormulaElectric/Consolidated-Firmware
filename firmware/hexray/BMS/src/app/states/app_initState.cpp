#include <cstddef>

#include "app_states.hpp"
#include "app_tractiveSystem.hpp"
#include "io_irs.hpp"
#include "io_charger.hpp"
#include "app_canRx.hpp"
#include "app_canTx.hpp"
#include "app_canUtils.hpp"

namespace app::states::initState
{

constexpr float TS_DISCHARGED_THRESHOLD_V = 10.0f;

void RunOnEntry()
{
    app::can_tx::BMS_State_set(app::can_utils::BmsState::BMS_INIT_STATE);

    // AIR+ opens upon entering init state.
    // Should always be opened at this point from other states; this is only for redundancy
    // since we really don't want AIR+ closed in init.
    io::irs::setPositive(app::can_utils::ContactorState::CONTACTOR_STATE_OPEN);
}

void RunOnTick100Hz()
{
    const bool irs_negative_closed =
        (io::irs::negativeState() == app::can_utils::ContactorState::CONTACTOR_STATE_CLOSED);
    const bool ts_discharged = (app::ts::getVoltage() < TS_DISCHARGED_THRESHOLD_V);

    // ONLY RUN THIS WHEN CELLS HAVE HAD TIME TO SETTLE
    // if (app_canRx_Debug_ResetSoc_MinCellV_get())
    // {
    //     app_soc_resetSocFromVoltage();
    // }
    // else if (app_canRx_Debug_ResetSoc_CustomEnable_get())
    // {
    //     app_soc_resetSocCustomValue(app_canRx_Debug_ResetSoc_CustomVal_get());
    // }

    if (irs_negative_closed && ts_discharged)
    {
        const bool external_charging_request = app::can_rx::Debug_StartCharging_get();

        const auto conn_status       = io::charger::getConnectionStatus();
        const bool charger_connected = (conn_status == app::can_utils::ChargerConnectedType::CHARGER_CONNECTED_WALL) ||
                                       (conn_status == app::can_utils::ChargerConnectedType::CHARGER_CONNECTED_EVSE);

        const bool precharge_for_driving  = (app::can_rx::VC_State_get() == app::can_utils::VCState::VC_BMS_ON_STATE);
        const bool cell_balancing_enabled = app::can_rx::Debug_CellBalancingRequest_get();

        if (external_charging_request && charger_connected)
        {
            app::StateMachine::set_next_state(&precharge_charge_state);
        }
        else if (precharge_for_driving)
        {
            app::StateMachine::set_next_state(&precharge_drive_state);
        }
        else if (cell_balancing_enabled)
        {
            app::StateMachine::set_next_state(&balancing_state);
        }
    }
}

} // namespace app::states::initState

[[maybe_unused]] const app::State init_state = {
    .name              = "INIT",
    .run_on_entry      = app::states::initState::RunOnEntry,
    .run_on_tick_1Hz   = nullptr,
    .run_on_tick_100Hz = app::states::initState::RunOnTick100Hz,
    .run_on_exit       = nullptr,
};
