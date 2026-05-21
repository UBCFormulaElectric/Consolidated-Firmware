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

// OVERALL TODO: Refer to FIRM-175 and FIRM-544

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
constexpr float VAC_MAX  = 240.0f; // upper end of typical North American grid
constexpr float CAC_MAX  = 20.0f;  // max current available off of the ac breaker at competition.
constexpr float CIRC_EFF = 0.8f;   // estimated efficiency of the entire charging circuit (charger + wiring losses etc)

constexpr uint32_t ELCON_ERR_DEBOUNCE_MS = 3000U; // 3 seconds
} // namespace

namespace app::states
{

// Charge state implementation
namespace chargeState
{
    struct ElconRx
    {
        bool  hardwareFailure;
        bool  overTemperature;
        bool  inputVoltageFault;
        bool  chargingStateFault;
        bool  commTimeout;
        float outputVoltage_V;
        float outputCurrent_A;
    };

    struct ElconTx
    {
        float maxVoltage_V; // physical units
        float maxCurrent_A;
        bool  stopCharging; // true = send 0x01 in control byte
    };

    // Structure to hold min/max DC current based on AC supply extremes
    struct DCRange_t
    {
        float idc_min; // A – DC current at VAC_MIN
        float idc_max; // A – DC current at VAC_MAX
    };

    // Read current charger status from CAN→typed struct
    static ElconRx readElconStatus()
    {
        ElconRx rx{
            .hardwareFailure    = app::can_rx::Elcon_HardwareFailure_get(),
            .overTemperature    = app::can_rx::Elcon_ChargerOverTemperature_get(),
            .inputVoltageFault  = app::can_rx::Elcon_InputVoltageError_get(),
            .chargingStateFault = app::can_rx::Elcon_ChargingDisabled_get(),
            .commTimeout        = app::can_rx::Elcon_CommunicationTimeout_get(),
            .outputVoltage_V    = app::can_rx::Elcon_OutputVoltage_get(),
            .outputCurrent_A    = app::can_rx::Elcon_OutputCurrent_get(),
        };
        return rx;
    }

    // Push a charging command to the BMS CAN TX table
    static void buildTxFrame(const ElconTx &cmd)
    {
        app::can_tx::BMS_MaxChargingVoltage_set(cmd.maxVoltage_V);
        app::can_tx::BMS_MaxChargingCurrent_set(cmd.maxCurrent_A);
        app::can_tx::BMS_StopCharging_set(cmd.stopCharging);
    }

    // Compute conservative DC current range from AC limits (kept commented until wiring ready)
    static DCRange_t calc_dc_current_range(float iac_max)
    {
        DCRange_t   range{};
        const float pin_min  = VAC_MIN * iac_max;
        const float pin_max  = VAC_MAX * iac_max;
        const float pout_min = pin_min * CHARGER_EFFICIENCY;
        const float pout_max = pin_max * CHARGER_EFFICIENCY;
        // Conservative: dividing by max pack voltage gives the lowest current estimate
        range.idc_min = pout_min / PACK_VOLTAGE_MAX;
        range.idc_max = pout_max / PACK_VOLTAGE_MAX;
        if (range.idc_min > MAX_DC_CURRENT)
            range.idc_min = MAX_DC_CURRENT;
        if (range.idc_max > MAX_DC_CURRENT)
            range.idc_max = MAX_DC_CURRENT;
        return range;
    }

    static app::Timer elcon_err_debounce(ELCON_ERR_DEBOUNCE_MS);

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
        const ChargerConnectedType charger_connection_status = io::charger::getConnectionStatus();
        const bool                 chargerConn =
            (charger_connection_status == ChargerConnectedType::CHARGER_CONNECTED_EVSE ||
             charger_connection_status == ChargerConnectedType::CHARGER_CONNECTED_WALL);

        const bool    userEnable = app::can_rx::Debug_StartCharging_get();
        const ElconRx rx         = readElconStatus();

        // const float max_cell_V = app::segments::getMaxCellVoltage();
        const float max_cell_V = 3.6f; // TODO: Replace with actual cell voltage reading when segments is ready
        // Transient faults (charger, shutdown loop, comm blip) get debounced
        const bool transient_fault = !chargerConn || rx.hardwareFailure || rx.chargingStateFault ||
                                     rx.overTemperature || rx.inputVoltageFault || rx.commTimeout;
        const bool transient_latched =
            (elcon_err_debounce.runIfCondition(transient_fault) == app::Timer::TimerState::EXPIRED);

        const ElconTx stop{ .maxVoltage_V = 0.0f, .maxCurrent_A = 0.0f, .stopCharging = true };

        if (transient_latched)
        {
            app::can_tx::BMS_ChargingFaulted_set(true);
            buildTxFrame(stop);
            app::StateMachine::set_next_state(&init_state);
            return;
        }

        if (!userEnable)
        {
            buildTxFrame(stop);
            app::StateMachine::set_next_state(&init_state);
            return;
        }

        // Terminate when cells are essentially full AND Elcon has already tapered down
        // Both conditions required (high V + high I = mid-CV-ramp, low I + low V = idle)
        if ((max_cell_V >= CELL_V_TERMINATE) && (rx.outputCurrent_A < I_TERMINATE_A))
        {
            app::can_tx::BMS_ChargingDone_set(true);
            buildTxFrame(stop);
            app::StateMachine::set_next_state(&init_state);
            return;
        }

        // Derive DC current limit from AC supply capability. Same code path on wall and EVSE (only difference is the
        // available AC current)
        DCRange_t idc_range;
        if (charger_connection_status == ChargerConnectedType::CHARGER_CONNECTED_EVSE)
        {
            const float evse_iac         = app::charger::getAvailableCurrent();
            const float clamped_evse_iac = (evse_iac > CAC_MAX * CIRC_EFF) ? CAC_MAX * CIRC_EFF : evse_iac;
            idc_range                    = calc_dc_current_range(clamped_evse_iac);
        }
        else
        {
            idc_range = calc_dc_current_range(CAC_MAX * CIRC_EFF);
        }

        // CC/CV is implicit given the Elcon needs max V and max I commands
        // Per cell CC/CV fallback (if pack is unbalanced or not using EVSE)
        float I_cmd_max = idc_range.idc_min;
        if (max_cell_V > CELL_V_TAPER_START)
        {
            const float frac = (CELL_V_MAX_CHARGE - max_cell_V) / (CELL_V_MAX_CHARGE - CELL_V_TAPER_START);
            I_cmd_max        = idc_range.idc_min * frac;
        }

        const ElconTx tx{
            .maxVoltage_V = PACK_VOLTAGE_MAX,
            .maxCurrent_A = I_cmd_max,
            .stopCharging = false,
        };
        buildTxFrame(tx);
    }

    static void runOnExit()
    {
        // Open AIR+ on exit from charging
        io::irs::setPositive(app::can_utils::ContactorState::CONTACTOR_STATE_OPEN);

        // Ensure we don't unintentionally re-enter charge state
        app::can_rx::Debug_StartCharging_update(false);

        // Reset command to zero on exit
        const ElconTx tx{ .maxVoltage_V = 0.0f, .maxCurrent_A = 0.0f, .stopCharging = true };
        buildTxFrame(tx);
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