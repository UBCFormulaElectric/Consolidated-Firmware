#include "app_stateMachine.h"
#include "app_chargeState.h"
#include "io_irs.h"
#include "io_charger.h"
#include "stdlib.h"

#define MAX_CHARGING_CURRENT_A 13.0f // TODO: verify this
#define MAX_CHARGING_VOLTAGE_V -1.0f // TODO: verify this

#define CHARGING_CUTOFF_MAX_CELL_VOLTAGE 4.15f

static uint16_t canMsgEndianSwap(uint16_t can_signal)
{
    // byte-swap 0x1234 -> 0x3412 
    return (uint16_t)((can_signal >> 8) | (can_signal << 8));
}

static uint16_t encodeElconParam(float value)
{
    uint16_t raw = (uint16_t)lrintf(value * 10.0f);   // e.g. 540 V -> 5400
    return canMsgEndianSwap(raw);                     // big-endian
}

static float decodeElconParam(uint8_t high, uint8_t low)
{
    uint16_t raw_be = ((uint16_t)high << 8) | low;    // MSB first in frame
    uint16_t raw_le = canMsgEndianSwap(raw_be);       // swap to host order 
    return (float)raw_le / 10.0f;                     // back to physical
}

static ElconRx readElconStatus(void)
{
    ElconRx s = {
        .hardwareFailure     = app_canRx_Elcon_HardwareFailure_get(),
        .overTemperature     = app_canRx_Elcon_ChargerOverTemperature_get(),
        .inputVoltageFault   = app_canRx_Elcon_InputVoltageError_get(),
        .chargingStateFault  = app_canRx_Elcon_ChargingState_get(),
        .commTimeout         = app_canRx_Elcon_CommunicationTimeout_get(),
        .outputVoltage_V     = decodeElconParam(
                                    app_canRx_Elcon_OutputVoltageHighByte_get(),
                                    app_canRx_Elcon_OutputVoltageLowByte_get()),
        .outputCurrent_A     = decodeElconParam(
                                    app_canRx_Elcon_OutputCurrentHighByte_get(),
                                    app_canRx_Elcon_OutputCurrentLowByte_get())
    };
    return s;
}

static void buildTxFrame(const ElconTx *cmd)
{
    app_canTx_BMS_MaxChargingVoltage_set(encodeElconParam(cmd->maxVoltage_V));
    app_canTx_BMS_MaxChargingCurrent_set(encodeElconParam(cmd->maxCurrent_A));
    app_canTx_BMS_StopCharging(cmd->stopCharging);
}

static void app_chargeStateRunOnEntry()
{

}

static void app_chargeStateRunOnTick1Hz()
{

}

static void app_chargeStateRunOnTick100Hz()
{
    const bool extShutdown   = !io_irs_isNegativeClosed();
    const bool chargerConn   = (io_charger_getConnectionStatus() == EVSE_CONNECTED || WALL_CONNECTED);
    const bool userEnable    = app_canRx_Debug_StartCharging_get();

    ElconRx rx = readElconStatus();

    const bool fault = extShutdown                  ||
                       !chargerConn                 ||
                       rx.hardwareFailure           ||
                       rx.overTemperature           ||
                       rx.inputVoltageFault         ||
                       rx.chargingStateFault        ||
                       rx.commTimeout;
    
    if (fault)
        app_stateMachine_setNextState(app_chargeFaultState_get());
    else if (!userEnable)
        app_stateMachine_setNextState(app_chargeInitState_get());

    const ElconTx tx = {
        .maxVoltage_V = MAX_CHARGING_VOLTAGE_V,
        .maxCurrent_A = MAX_CHARGING_CURRENT_A,
        .stopCharging = !userEnable
    };
    buildTxFrame(&tx);

    const float max_cell_voltage; // TODO: make max cell voltage function
    if(max_cell_voltage >= CHARGING_CUTOFF_MAX_CELL_VOLTAGE)
    {
        app_stateMachine_setNextState(app_chargeInitState_get());
    }
}

static void app_chargeStateRunOnExit()
{
    io_irs_openPositive();
}

const State *app_chargeState_get(void)
{
    static State charge_state = {
        .name               = "CHARGE",
        .run_on_entry       = app_chargeStateRunOnEntry,
        .run_on_tick_1Hz    = app_chargeStateRunOnTick1Hz,
        .run_on_tick_100Hz  = app_chargeStateRunOnTick100Hz,
        .run_on_exit        = app_chargeStateRunOnExit,
    };

    return &charge_state;
}