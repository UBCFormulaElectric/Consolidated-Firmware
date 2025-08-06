#include "app_canUtils.h"
#include "states/app_states.h"
#include "app_timer.h"
#include "io_irs.h"
#include "io_charger.h"
#include "app_canRx.h"
#include "app_canTx.h"
#include "app_segments.h"

#define NUM_SEG 10.0f
#define NUM_CELLS_PER_SEG 14.0f
#define CHARGING_CUTOFF_MAX_CELL_VOLTAGE 4.15f

// Charger / pack constants
#define PACK_VOLTAGE_DC 581.0f // V – the battery pack’s nominal voltage (4.15V per cell * 14 cell per seg * 10 seg)
#define CHARGER_EFFICIENCY 0.93f // 93% – average DC-side efficiency of the Elcon

// Charger’s own output limit (never command above this)
#define MAX_DC_CURRENT 13.0f // A – battery limit of DC output current

// AC supply range to cover worst/best mains voltages
#define VAC_MIN 208.0f // V – lower end of typical North American grid
#define VAC_MAX 240.0f // V – upper end of typical North American grid
#define CAC_MAX 32.0f  // A - max current avaliable of typical North American grid

typedef struct
{
    bool  hardwareFailure;
    bool  overTemperature;
    bool  inputVoltageFault;
    bool  chargingStateFault;
    bool  commTimeout;
    float outputVoltage_V;
    float outputCurrent_A;
} ElconRx;

typedef struct
{
    float maxVoltage_V; // physical units
    float maxCurrent_A;
    bool  stopCharging; // true = send 0x01 in control byte
} ElconTx;

// Structure to hold min/max DC current based on AC supply extremes
typedef struct
{
    float idc_min; // A – DC current at VAC_MIN
    float idc_max; // A – DC current at VAC_MAX
} DCRange_t;

/**
 * Swap for CAN BE
 */
// static uint16_t canMsgEndianSwap(uint16_t can_signal)
// {
//     // byte-swap 0x1234 -> 0x3412
//     return (uint16_t)((can_signal >> 8) | (can_signal << 8));
// }

/**
 * Swap for CAN BE
 */
// static uint16_t encodeElconParam(float value)
// {
//     uint16_t raw = (uint16_t)lrintf(value * 10.0f); // e.g. 540 V -> 5400
//     return canMsgEndianSwap(raw);                   // big-endian
// }

/**
 * Swap for CAN BE
 */
// void encodeMaxVoltageBE(float voltage, uint8_t *high, uint8_t *low)
// {
//     // scale by 10 → 0.1 V resolution, round to nearest integer
//     uint16_t raw = (uint16_t)lrintf(voltage * 10.0f);
//     // swap to big-endian
//     uint16_t be = canMsgEndianSwap(raw);

//     *high = (uint8_t)(be >> 8);
//     *low  = (uint8_t)(be & 0xFF);
// }

/**
 * Swap for CAN BE
 */
// static float decodeElconParam(uint8_t high, uint8_t low)
// {
//     uint16_t raw_be = (uint16_t)((high << 8) | low); // MSB first in frame
//     uint16_t raw_le = canMsgEndianSwap(raw_be);      // swap to host order
//     return (float)raw_le / 10.0f;                    // back to physical
// }

/**
 * @return Elcon status
 */
static ElconRx readElconStatus(void)
{
    return (ElconRx){ .hardwareFailure    = app_canRx_Elcon_HardwareFailure_get(),
                      .overTemperature    = app_canRx_Elcon_ChargerOverTemperature_get(),
                      .inputVoltageFault  = app_canRx_Elcon_InputVoltageError_get(),
                      .chargingStateFault = app_canRx_Elcon_ChargingDisabled_get(),
                      .commTimeout        = app_canRx_Elcon_CommunicationTimeout_get(),
                      .outputVoltage_V    = app_canRx_Elcon_OutputVoltage_get(),
                      .outputCurrent_A    = app_canRx_Elcon_OutputCurrent_get() };
}

/**
 * @brief  Calculate max DC currents for given AC current limit.
 * @param  iac_max    Maximum AC current (A) as advertised by EVSE or measured.
 * @return DCRange_t  { idc_min, idc_max } in DC amperes.
 *
 * Steps:
 *   1. Compute input power at min/max AC voltage:
 *        P_in = V_ac * I_ac
 *   2. Account for charger efficiency:
 *        P_out = P_in * efficiency
 *   3. Convert to DC current at fixed pack voltage:
 *        I_dc = P_out / V_dc
 *   4. Clamp to battery DC current ceiling.
 */
// static DCRange_t calc_dc_current_range(float iac_max)
// {
//     DCRange_t range;

//     // 1. AC input power at the low-voltage extreme:
//     //    P_in_min = VAC_MIN * iac_max (W)
//     float pin_min = VAC_MIN * iac_max;

//     // 2. AC input power at the high-voltage extreme:
//     //    P_in_max = VAC_MAX * iac_max (W)
//     float pin_max = VAC_MAX * iac_max;

//     // 3. DC output power accounting for efficiency:
//     //    P_out = P_in * CHARGER_EFFICIENCY
//     float pout_min = pin_min * CHARGER_EFFICIENCY;
//     float pout_max = pin_max * CHARGER_EFFICIENCY;

//     // 4. Corresponding DC currents at PACK_VOLTAGE_DC:
//     //    I_dc = P_out / PACK_VOLTAGE_DC (A)
//     range.idc_min = pout_min / PACK_VOLTAGE_DC;
//     range.idc_max = pout_max / PACK_VOLTAGE_DC;

//     // 5. Clamp to the charger’s maximum DC output current:
//     if (range.idc_min > MAX_DC_CURRENT)
//         range.idc_min = MAX_DC_CURRENT;
//     if (range.idc_max > MAX_DC_CURRENT)
//         range.idc_max = MAX_DC_CURRENT;

//     return range;
// }

#define ELCON_ERR_DEBOUNCE_MS (3000)

static TimerChannel elcon_err_debounce;

static void buildTxFrame(const ElconTx *cmd)
{
    app_canTx_BMS_MaxChargingVoltage_set(cmd->maxVoltage_V);
    app_canTx_BMS_MaxChargingCurrent_set(cmd->maxCurrent_A);
    app_canTx_BMS_StopCharging_set(cmd->stopCharging);
}

static void app_chargeStateRunOnEntry(void)
{
    app_canTx_BMS_State_set(BMS_CHARGE_STATE);

    // Reset charge state CAN when entering charge state.
    app_canTx_BMS_ChargingFaulted_set(false);
    app_canTx_BMS_ChargingDone_set(false);

    io_irs_setPositive(CONTACTOR_STATE_CLOSED);

    app_timer_init(&elcon_err_debounce, ELCON_ERR_DEBOUNCE_MS);
}

static void app_chargeStateRunOnTick100Hz(void)
{
    // const ChargerConnectedType charger_connection_status = CHARGER_CONNECTED_EVSE; // io_charger_getConnectionStatus();
    const bool extShutdown = io_irs_negativeState() == CONTACTOR_STATE_OPEN;
    const bool chargerConn = true; // (charger_connection_status == CHARGER_CONNECTED_EVSE || CHARGER_CONNECTED_WALL);
    const bool userEnable  = app_canRx_Debug_StartCharging_get();

    const ElconRx rx = readElconStatus();

    const bool fault = extShutdown || !chargerConn || rx.hardwareFailure || rx.chargingStateFault ||
                       rx.overTemperature || rx.inputVoltageFault || rx.commTimeout;
    const bool fault_latched = app_timer_runIfCondition(&elcon_err_debounce, fault) == TIMER_STATE_EXPIRED;

    if (fault_latched || !userEnable)
    {
        app_canTx_BMS_ChargingFaulted_set(fault);
        app_stateMachine_setNextState(&init_state);
    }

    // TODO: Fix calc_dc_current_range
    // DCRange_t idc_range;
    // if (charger_connection_status == CHARGER_CONNECTED_EVSE)
    // {
    //     const float evse_iac = app_charger_getAvaliableCurrent();
    //     idc_range            = calc_dc_current_range(evse_iac);
    // }
    // else // else wall charger is connected
    // {
    //     const float wall_iac = 32.0f;
    //     idc_range            = calc_dc_current_range(wall_iac);
    // }

    // TODO: Consider more careful max voltage and current for charging.
    const ElconTx tx = {
        .maxVoltage_V =
            PACK_VOLTAGE_DC, // always cap at 581V
                             // .maxCurrent_A = idc_range.idc_min, // cap at min idc value to stay on the safe side
        .maxCurrent_A = app_canRx_Debug_ChargingCurrent_get(),
        .stopCharging = !userEnable
    };
    buildTxFrame(&tx);

    /**
     * if any cell has reached the cutoff voltge charging has completed
     */
    const float max_cell_voltage = app_segments_getMaxCellVoltage().value;
    if (max_cell_voltage >= CHARGING_CUTOFF_MAX_CELL_VOLTAGE)
    {
        app_canTx_BMS_ChargingDone_set(true);
        app_stateMachine_setNextState(&init_state);
    }
}

static void app_chargeStateRunOnExit(void)
{
    io_irs_setPositive(CONTACTOR_STATE_OPEN);

    // Just in case we exited charging not due to CAN (fault, etc.) set the CAN table back to false so we don't
    // unintentionally re-enter charge state.
    app_canRx_Debug_StartCharging_update(false);

    // Reset to zero on exit.
    const ElconTx tx = { .maxVoltage_V = 0, .maxCurrent_A = 0, .stopCharging = true };
    buildTxFrame(&tx);
}

const State charge_state = {
    .name              = "CHARGE",
    .run_on_entry      = app_chargeStateRunOnEntry,
    .run_on_tick_100Hz = app_chargeStateRunOnTick100Hz,
    .run_on_exit       = app_chargeStateRunOnExit,
};

static void app_chargeFaultStateRunOnEntry()
{
    app_canTx_BMS_State_set(BMS_CHARGE_FAULT_STATE);
}
static void app_chargeFaultStateRunOnTick100Hz() {}
static void app_chargeFaultStateRunOnExit() {}

const State charge_fault_state = {
    .name              = "CHARGE FAULT",
    .run_on_entry      = app_chargeFaultStateRunOnEntry,
    .run_on_tick_100Hz = app_chargeFaultStateRunOnTick100Hz,
    .run_on_exit       = app_chargeFaultStateRunOnExit,
};

static void app_chargeInitStateRunOnEntry()
{
    app_canTx_BMS_State_set(BMS_CHARGE_INIT_STATE);
}
static void app_chargeInitStateRunOnTick100Hz() {}
static void app_chargeInitStateRunOnExit() {}

const State charge_init_state = {
    .name              = "CHARGE INIT",
    .run_on_entry      = app_chargeInitStateRunOnEntry,
    .run_on_tick_100Hz = app_chargeInitStateRunOnTick100Hz,
    .run_on_exit       = app_chargeInitStateRunOnExit,
};