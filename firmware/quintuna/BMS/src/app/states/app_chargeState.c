#include "app_stateMachine.h"
#include "app_chargeState.h"
#include "app_chargeFaultState.h"
#include "app_chargeInitState.h"
#include "io_irs.h"
#include "io_charger.h"
#include "app_charger.h"
#include "app_canRx.h"
#include "app_canTx.h"

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

// ElconRx readElconStatus(void)
// {
//     ElconRx s = { .hardwareFailure    = app_canRx_Elcon_HardwareFailure_get(),
//                   .overTemperature    = app_canRx_Elcon_ChargerOverTemperature_get(),
//                   .inputVoltageFault  = app_canRx_Elcon_InputVoltageError_get(),
//                   .chargingStateFault = app_canRx_Elcon_ChargingDisabled_get(),
//                   .commTimeout        = app_canRx_Elcon_CommunicationTimeout_get(),
//                   .outputVoltage_V    = app_canRx_Elcon_OutputVoltage_get(),
//                   .outputCurrent_A    = app_canRx_Elcon_OutputCurrent_get() };
//     return s;
// }

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
static DCRange_t calc_dc_current_range(float iac_max)
{
    DCRange_t range;

    // 1. AC input power at the low-voltage extreme:
    //    P_in_min = VAC_MIN * iac_max (W)
    float pin_min = VAC_MIN * iac_max;

    // 2. AC input power at the high-voltage extreme:
    //    P_in_max = VAC_MAX * iac_max (W)
    float pin_max = VAC_MAX * iac_max;

    // 3. DC output power accounting for efficiency:
    //    P_out = P_in * CHARGER_EFFICIENCY
    float pout_min = pin_min * CHARGER_EFFICIENCY;
    float pout_max = pin_max * CHARGER_EFFICIENCY;

    // 4. Corresponding DC currents at PACK_VOLTAGE_DC:
    //    I_dc = P_out / PACK_VOLTAGE_DC (A)
    range.idc_min = pout_min / PACK_VOLTAGE_DC;
    range.idc_max = pout_max / PACK_VOLTAGE_DC;

    // 5. Clamp to the charger’s maximum DC output current:
    if (range.idc_min > MAX_DC_CURRENT)
        range.idc_min = MAX_DC_CURRENT;
    if (range.idc_max > MAX_DC_CURRENT)
        range.idc_max = MAX_DC_CURRENT;

    return range;
}

static void buildTxFrame(const ElconTx *cmd)
{
    app_canTx_BMS_MaxChargingVoltage_set(cmd->maxVoltage_V);
    app_canTx_BMS_MaxChargingCurrent_set(cmd->maxCurrent_A);
    app_canTx_BMS_StopCharging_set(cmd->stopCharging);
}

static void app_chargeStateRunOnEntry(void) {}

static void app_chargeStateRunOnTick1Hz(void) {}

static void app_chargeStateRunOnTick100Hz(void)
{
    const ConnectionStatus charger_connection_status = io_charger_getConnectionStatus();
    const bool             extShutdown               = !io_irs_isNegativeClosed();
    const bool             chargerConn               = (charger_connection_status == EVSE_CONNECTED || WALL_CONNECTED);
    const bool             userEnable                = app_canRx_Debug_StartCharging_get();

    ElconRx rx = readElconStatus();

    const bool fault = extShutdown || !chargerConn || rx.hardwareFailure || rx.overTemperature ||
                       rx.inputVoltageFault || rx.chargingStateFault || rx.commTimeout;

    if (fault)
        app_stateMachine_setNextState(app_chargeFaultState_get());
    else if (!userEnable)
        app_stateMachine_setNextState(app_chargeInitState_get());

    DCRange_t idc_range;
    if (charger_connection_status == EVSE_CONNECTED)
    {
        const float evse_iac = app_charger_getAvaliableCurrent();
        idc_range            = calc_dc_current_range(evse_iac);
    }
    else // else wall charger is connected
    {
        const float wall_iac = 32.0f;
        idc_range            = calc_dc_current_range(wall_iac);
    }

    const ElconTx tx = { .maxVoltage_V = PACK_VOLTAGE_DC,   // always cap at 581V
                         .maxCurrent_A = idc_range.idc_min, // cap at min idc value to stay on the safe side
                         .stopCharging = !userEnable };
    buildTxFrame(&tx);

    /**
     * if any cell has reached the cutoff voltge charging has completed
     */
    // const float max_cell_voltage; // TODO: make max cell voltage function
    // if (max_cell_voltage >= CHARGING_CUTOFF_MAX_CELL_VOLTAGE)
    // {
    //     app_stateMachine_setNextState(app_chargeInitState_get());
    // }
}

static void app_chargeStateRunOnExit(void)
{
    io_irs_openPositive();
}

const State *app_chargeState_get(void)
{
    static State charge_state = {
        .name              = "CHARGE",
        .run_on_entry      = app_chargeStateRunOnEntry,
        .run_on_tick_1Hz   = app_chargeStateRunOnTick1Hz,
        .run_on_tick_100Hz = app_chargeStateRunOnTick100Hz,
        .run_on_exit       = app_chargeStateRunOnExit,
    };

    return &charge_state;
}
