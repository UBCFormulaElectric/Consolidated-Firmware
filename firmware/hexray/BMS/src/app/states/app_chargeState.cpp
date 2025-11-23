#include <cstddef>
#include <cstdint>

#include "app_states.hpp"
#include "app_timer.hpp"
#include "io_irs.hpp"
#include "io_charger.hpp"
// #include "app_segments.hpp"

extern "C"
{
#include "app_canUtils.h"
#include "app_canRx.h"
#include "app_canTx.h"
}

// Charger/pack constants
namespace
{
// TODO: Change back if we ever get to 10 segments again
// constexpr float PACK_VOLTAGE_DC 581.0f // V – the battery pack’s nominal voltage (4.15V per cell * 14 cell per seg *
// 10 seg)
constexpr float PACK_VOLTAGE_DC    = 464.8f; // V – (4.15V * 14 cells/seg * 8 seg)
constexpr float CHARGER_EFFICIENCY = 0.93f;  // 93% – average DC-side efficiency of the Elcon
constexpr float MAX_DC_CURRENT     = 13.0f;  // A – battery limit of DC output current

constexpr float VAC_MIN = 208.0f; // V – lower end of typical North American grid
constexpr float VAC_MAX = 240.0f; // V – upper end of typical North American grid
constexpr float CAC_MAX = 32.0f;  // A – max current available of typical NA grid

constexpr uint32_t ELCON_ERR_DEBOUNCE_MS = 3000U; // 3 seconds
} // namespace

// Charge state implementation
namespace app::states::chargeState
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

// Optional BE encode/decode helpers (kept commented, C++-ready)
// static uint16_t canMsgEndianSwap(uint16_t can_signal)
// {
//     return static_cast<uint16_t>((can_signal >> 8) | (can_signal << 8));
// }
//
// static uint16_t encodeElconParam(float value)
// {
//     uint16_t raw = static_cast<uint16_t>(lrintf(value * 10.0f)); // e.g. 540 V -> 5400
//     return canMsgEndianSwap(raw);                                 // big-endian
// }
//
// static void encodeMaxVoltageBE(float voltage, uint8_t *high, uint8_t *low)
// {
//     uint16_t raw = static_cast<uint16_t>(lrintf(voltage * 10.0f)); // 0.1 V resolution
//     uint16_t be  = canMsgEndianSwap(raw);
//     *high        = static_cast<uint8_t>(be >> 8);
//     *low         = static_cast<uint8_t>(be & 0xFF);
// }
//
// static float decodeElconParam(uint8_t high, uint8_t low)
// {
//     uint16_t raw_be = static_cast<uint16_t>((high << 8) | low);
//     uint16_t raw_le = canMsgEndianSwap(raw_be);
//     return static_cast<float>(raw_le) / 10.0f;
// }

// Read current charger status from CAN→typed struct
static ElconRx readElconStatus()
{
    ElconRx rx{
        .hardwareFailure    = app_canRx_Elcon_HardwareFailure_get(),
        .overTemperature    = app_canRx_Elcon_ChargerOverTemperature_get(),
        .inputVoltageFault  = app_canRx_Elcon_InputVoltageError_get(),
        .chargingStateFault = app_canRx_Elcon_ChargingDisabled_get(),
        .commTimeout        = app_canRx_Elcon_CommunicationTimeout_get(),
        .outputVoltage_V    = app_canRx_Elcon_OutputVoltage_get(),
        .outputCurrent_A    = app_canRx_Elcon_OutputCurrent_get(),
    };
    return rx;
}

// Push a charging command to the BMS CAN TX table
static void buildTxFrame(const ElconTx &cmd)
{
    app_canTx_BMS_MaxChargingVoltage_set(cmd.maxVoltage_V);
    app_canTx_BMS_MaxChargingCurrent_set(cmd.maxCurrent_A);
    app_canTx_BMS_StopCharging_set(cmd.stopCharging);
}

// // Compute conservative DC current range from AC limits (kept commented until wiring ready)
// static DCRange_t calc_dc_current_range(float iac_max)
// {
//     DCRange_t range{};
//     const float pin_min  = VAC_MIN * iac_max;
//     const float pin_max  = VAC_MAX * iac_max;
//     const float pout_min = pin_min * CHARGER_EFFICIENCY;
//     const float pout_max = pin_max * CHARGER_EFFICIENCY;
//     range.idc_min        = pout_min / PACK_VOLTAGE_DC;
//     range.idc_max        = pout_max / PACK_VOLTAGE_DC;
//     if (range.idc_min > MAX_DC_CURRENT) range.idc_min = MAX_DC_CURRENT;
//     if (range.idc_max > MAX_DC_CURRENT) range.idc_max = MAX_DC_CURRENT;
//     return range;
// }

static TimerChannel elcon_err_debounce;

// --------------------------------- CHARGE state ---------------------------------------

static void runOnEntry()
{
    app_canTx_BMS_State_set(BMS_CHARGE_STATE);

    // Reset charge status flags when entering charge state.
    app_canTx_BMS_ChargingFaulted_set(false);
    app_canTx_BMS_ChargingDone_set(false);

    // Close AIR+ to enable HV path
    io_irs_setPositive(CONTACTOR_STATE_CLOSED);

    app_timer_init(&elcon_err_debounce, ELCON_ERR_DEBOUNCE_MS);
}

static void runOnTick100Hz()
{
    // TODO: Fix charger connection status reading
    // const ChargerConnectedType charger_connection_status = io_charger_getConnectionStatus();
    const bool extShutdown = (io_irs_negativeState() == CONTACTOR_STATE_OPEN);
    const bool chargerConn = true; // (charger_connection_status == CHARGER_CONNECTED_EVSE || CHARGER_CONNECTED_WALL);
    const bool userEnable  = app_canRx_Debug_StartCharging_get();

    const ElconRx rx = readElconStatus();

    const bool fault = extShutdown || !chargerConn || rx.hardwareFailure || rx.chargingStateFault ||
                       rx.overTemperature || rx.inputVoltageFault || rx.commTimeout;

    const bool fault_latched = (app_timer_runIfCondition(&elcon_err_debounce, fault) == TIMER_STATE_EXPIRED);

    if (fault_latched || !userEnable)
    {
        app_canTx_BMS_ChargingFaulted_set(fault);
        app::StateMachine::set_next_state(&init_state);
        return;
    }

    // TODO: Fix calc_dc_current_range and EVSE available current path
    // DCRange_t idc_range;
    // if (charger_connection_status == CHARGER_CONNECTED_EVSE)
    // {
    //     const float evse_iac = app_charger_getAvaliableCurrent();
    //     idc_range            = calc_dc_current_range(evse_iac);
    // }
    // else // wall charger is connected
    // {
    //     const float wall_iac = 32.0f;
    //     idc_range            = calc_dc_current_range(wall_iac);
    // }

    // For now, command nominal pack voltage and a debug-selected current
    const ElconTx tx{
        .maxVoltage_V = PACK_VOLTAGE_DC, // cap at 464.8V (8 segments)
        // .maxCurrent_A = idc_range.idc_min,                 // conservative min DC current (when enabled)
        .maxCurrent_A = app_canRx_Debug_ChargingCurrent_get(), // debug-controlled current
        .stopCharging = !userEnable,
    };
    buildTxFrame(tx);

    // Charging completion: if any cell reaches cutoff, stop charging
    const float max_cell_voltage = app_segments_getMaxCellVoltage().value;
    if (max_cell_voltage >= MAX_CELL_VOLTAGE_WARNING_V)
    {
        app_canTx_BMS_ChargingDone_set(true);
        app::StateMachine::set_next_state(&init_state);
    }
}

static void runOnExit()
{
    // Open AIR+ on exit from charging
    io_irs_setPositive(CONTACTOR_STATE_OPEN);

    // Ensure we don't unintentionally re-enter charge state
    app_canRx_Debug_StartCharging_update(false);

    // Reset command to zero on exit
    const ElconTx tx{ .maxVoltage_V = 0.0f, .maxCurrent_A = 0.0f, .stopCharging = true };
    buildTxFrame(tx);
}

} // namespace app::states::chargeState

const State charge_state = {
    .name              = "CHARGE",
    .run_on_entry      = app::states::chargeState::runOnEntry,
    .run_on_tick_100Hz = app::states::chargeState::runOnTick100Hz,
    .run_on_exit       = app::states::chargeState::runOnExit,
};

// Charge fault state implementation
namespace app::states::chargeFaultState
{
static void runOnEntry()
{
    app_canTx_BMS_State_set(BMS_CHARGE_FAULT_STATE);
}
static void runOnTick100Hz() {}
static void runOnExit() {}
} // namespace app::states::chargeFaultState

const State charge_fault_state = {
    .name              = "CHARGE FAULT",
    .run_on_entry      = app::states::chargeFaultState::runOnEntry,
    .run_on_tick_100Hz = app::states::chargeFaultState::runOnTick100Hz,
    .run_on_exit       = app::states::chargeFaultState::runOnExit,
};

// Charge init state implementation
namespace app::states::chargeInitState
{
static void runOnEntry()
{
    app_canTx_BMS_State_set(BMS_CHARGE_INIT_STATE);
}
static void runOnTick100Hz() {}
static void runOnExit() {}
} // namespace app::states::chargeInitState

const app::State charge_init_state = {
    .name              = "CHARGE INIT",
    .run_on_entry      = app::states::chargeInitState::runOnEntry,
    .run_on_tick_100Hz = app::states::chargeInitState::runOnTick100Hz,
    .run_on_exit       = app::states::chargeInitState::runOnExit,
};