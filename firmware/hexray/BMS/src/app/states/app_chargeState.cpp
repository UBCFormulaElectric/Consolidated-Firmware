#include <cstddef>
#include <cstdint>

#include "app_states.hpp"
#include "app_timer.hpp"
#include "io_irs.hpp"
#include "io_charger.hpp"
#include "app_charger.hpp"
#include "app_segments.hpp"
#include "app_canUtils.hpp"
#include "app_canRx.hpp"
#include "app_canTx.hpp"
#include "app_canAlerts.hpp"

// Charger/pack constants
namespace
{
// Per-cell charge thresholds
constexpr float CELL_V_TAPER_START = 4.10f; // V
constexpr float CELL_V_TERMINATE   = 4.18f; // V
constexpr float CELL_V_MAX_CHARGE  = 4.20f; // V
constexpr float I_TERMINATE_A      = 1.0f;  // A

// Pack-level target commanded to the Elcon
// TODO: Replace with actual pack voltage when segments is ready.
// constexpr float PACK_VOLTAGE_MAX = static_cast<float>(io::adbms::NUM_SEGMENTS) *
// static_cast<float>(io::adbms::CELLS_PER_SEGMENT) * CELL_V_MAX_CHARGE;
constexpr float PACK_VOLTAGE_MAX = 140.0f * CELL_V_MAX_CHARGE;

constexpr float CHARGER_EFFICIENCY = 0.93f; // average DC-side efficiency of the Elcon
constexpr float MAX_DC_CURRENT     = 12.0f; // 1C standard limit of DC input current to the pack

// TODO: Double-check these constants, refer to FIRM-175 I think the receptacle is 208V and max amperage is 20A
constexpr float VAC_MIN  = 208.0f; // lower end of typical North American grid
constexpr float CAC_MAX  = 20.0f;  // max current available off of the ac breaker at competition.
constexpr float CIRC_EFF = 0.8f;   // estimated efficiency of the entire charging circuit (charger + wiring losses etc)

constexpr app::charger::ElconChargingConfig stop{ .max_voltage_v = 0.0f, .max_current_a = 0.0f, .stop_charging = true };

} // namespace

namespace app::states
{

// Charge state implementation
namespace chargeState
{
    // Conservative DC current limit from AC supply
    static float calcDcCurrentLimit(float iac_max)
    {
        const float pin  = VAC_MIN * iac_max;
        const float pout = pin * CHARGER_EFFICIENCY;
        // Very conservative... Dividing by max pack voltage gives the lowest current estimate
        // TODO: Replace PACK_VOLTAGE_MAX with app::segments::getPackVoltage() once segments is merged in!!
        float idc = pout / PACK_VOLTAGE_MAX;
        return (idc > MAX_DC_CURRENT) ? MAX_DC_CURRENT : idc;
    }

    static void runOnEntry()
    {
        app::can_tx::BMS_State_set(app::can_utils::BmsState::BMS_CHARGE_STATE);

        // Reset charge status flags when entering charge state.
        app::can_tx::BMS_ChargingFaulted_set(false);
        app::can_tx::BMS_ChargingDone_set(false);

        // Close AIR+ to enable HV path
        io::irs::setPositive(app::can_utils::ContactorState::CONTACTOR_STATE_CLOSED);
    }

    static void runOnTick100Hz()
    {
        using namespace app::can_utils;
        const ChargerConnectedType charger_connection_status = io::charger::getConnectionStatus();
        const bool                 charger_conn =
            (charger_connection_status == ChargerConnectedType::CHARGER_CONNECTED_EVSE ||
             charger_connection_status == ChargerConnectedType::CHARGER_CONNECTED_WALL);

        const bool                           user_enable  = app::can_rx::Debug_StartCharging_get();
        const app::charger::ElconFaultConfig fault_status = app::charger::getFaultStatus();

        const float max_cell_v = app::segments::shared::getMaxCellVoltage().value;

        if (!user_enable || !charger_conn || fault_status)
        {
            app::charger::setChargingConfig(stop);
            app::StateMachine::set_next_state(&init_state);
            return;
        }

        // Terminate when cells are essentially full AND Elcon has already tapered down
        // Both conditions required (high V + high I = mid-CV-ramp, low I + low V = idle)
        if ((max_cell_v >= CELL_V_TERMINATE) && (app::charger::getOutputCurrent() < I_TERMINATE_A))
        {
            app::can_tx::BMS_ChargingDone_set(true); // Is this ever set to false? look into this!!!!!!!!!!!!
            app::charger::setChargingConfig(stop);
            app::StateMachine::set_next_state(&init_state);
            return;
        }

        // Derive DC current limit from AC supply capability. Same code path on wall and EVSE (only difference is the
        // available AC current)
        float i_max;
        if (charger_connection_status == ChargerConnectedType::CHARGER_CONNECTED_EVSE)
        {
            const float evse_iac         = app::charger::getAvailableCurrent();
            const float clamped_evse_iac = (evse_iac > CAC_MAX * CIRC_EFF) ? CAC_MAX * CIRC_EFF : evse_iac;
            i_max                        = calcDcCurrentLimit(clamped_evse_iac);
        }
        else
        {
            i_max = calcDcCurrentLimit(CAC_MAX * CIRC_EFF);
        }

        // CC/CV is implicit given the Elcon needs max V and max I commands
        // Per cell CC/CV fallback (if pack is unbalanced or not using EVSE).
        // Clamp to 0 at/above the per-cell max so we never command negative current.
        float i_cmd_max; // Look into this more. make sure u know it!!!!
        if (max_cell_v >= CELL_V_MAX_CHARGE)
        {
            i_cmd_max = 0.0f;
        }
        else if (max_cell_v > CELL_V_TAPER_START)
        {
            const float frac = (CELL_V_MAX_CHARGE - max_cell_v) / (CELL_V_MAX_CHARGE - CELL_V_TAPER_START);
            i_cmd_max        = i_max * frac;
        }
        else
        {
            i_cmd_max = i_max;
        }

        const app::charger::ElconChargingConfig tx{
            .max_voltage_v = PACK_VOLTAGE_MAX,
            .max_current_a = i_cmd_max,
            .stop_charging = false,
        };
        app::charger::setChargingConfig(tx);
    }

    static void runOnExit()
    {
        // Open AIR+ on exit from charging
        io::irs::setPositive(app::can_utils::ContactorState::CONTACTOR_STATE_OPEN);

        // Ensure we don't unintentionally re-enter charge state
        app::can_rx::Debug_StartCharging_update(false);

        // Reset command to zero on exit
        app::charger::setChargingConfig(stop);
    }

} // namespace chargeState

const ::app::State charge_state = {
    .name              = "CHARGE",
    .run_on_entry      = chargeState::runOnEntry,
    .run_on_tick_1Hz   = nullptr,
    .run_on_tick_100Hz = chargeState::runOnTick100Hz,
    .run_on_exit       = chargeState::runOnExit,
};

} // namespace app::states