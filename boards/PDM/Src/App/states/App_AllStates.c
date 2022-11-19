#include <stdint-gcc.h>
#include "states/App_AllStates.h"
#include "states/App_FaultState.h"
#include "App_CanTx.h"

#define NUM_CYCLES_TO_SETTLE (3U)

void App_AllStatesRunOnTick1Hz(struct StateMachine *const state_machine)
{
    struct PdmWorld *      world            = App_SharedStateMachine_GetWorld(state_machine);
    struct RgbLedSequence *rgb_led_sequence = App_PdmWorld_GetRgbLedSequence(world);

    App_SharedRgbLedSequence_Tick(rgb_led_sequence);
}

bool App_AllStatesRunOnTick100Hz(struct StateMachine *const state_machine)
{
    struct PdmWorld *         world        = App_SharedStateMachine_GetWorld(state_machine);
    struct PdmCanTxInterface *can_tx       = App_PdmWorld_GetCanTx(world);
    struct PdmCanRxInterface *can_rx       = App_PdmWorld_GetCanRx(world);
    struct HeartbeatMonitor * hb_monitor   = App_PdmWorld_GetHeartbeatMonitor(world);
    struct RailMonitoring *   rail_monitor = App_PdmWorld_GetRailMonitoring(world);
    struct Efuse *            efuse1       = App_PdmWorld_GetEfuse1(world);
    struct Efuse *            efuse2       = App_PdmWorld_GetEfuse2(world);
    struct Efuse *            efuse3       = App_PdmWorld_GetEfuse3(world);
    struct Efuse *            efuse4       = App_PdmWorld_GetEfuse4(world);

    bool           status                = true;
    static uint8_t acc_meas_settle_count = 0U;

    // Main Rail Monitoring:
    App_CanTx_SetPeriodicSignal_VBAT(can_tx, App_RailMonitoring_Get_VBAT_Voltage(rail_monitor));
    App_CanTx_SetPeriodicSignal__24_V_ACC(can_tx, App_RailMonitoring_Get__24V_ACC_Voltage(rail_monitor));
    App_CanTx_SetPeriodicSignal__22_V_AUX(can_tx, App_RailMonitoring_Get__22V_AUX_Voltage(rail_monitor));

    // Load Switch Current Monitoring:
    App_CanTx_SetPeriodicSignal(can_tx, App_Efuse_GetChannel0Current(efuse1));
    App_CanTx_SetPeriodicSignal(can_tx, App_Efuse_GetChannel1Current(efuse1));
    App_CanTx_SetPeriodicSignal(can_tx, App_Efuse_GetChannel0Current(efuse2));
    App_CanTx_SetPeriodicSignal(can_tx, App_Efuse_GetChannel1Current(efuse2));
    App_CanTx_SetPeriodicSignal(can_tx, App_Efuse_GetChannel0Current(efuse3));
    App_CanTx_SetPeriodicSignal(can_tx, App_Efuse_GetChannel1Current(efuse3));
    App_CanTx_SetPeriodicSignal(can_tx, App_Efuse_GetChannel0Current(efuse4));
    App_CanTx_SetPeriodicSignal(can_tx, App_Efuse_GetChannel1Current(efuse4));

    App_CanTx_SetPeriodicSignal_HEARTBEAT(can_tx, true);

    if (App_CanRx_BMS_VITALS_GetSignal_HEARTBEAT(can_rx))
    {
        App_SharedHeartbeatMonitor_CheckIn(hb_monitor, BMS_HEARTBEAT_ONE_HOT);
        App_CanRx_BMS_VITALS_SetSignal_HEARTBEAT(can_rx, false);
    }

    if (acc_meas_settle_count < NUM_CYCLES_TO_SETTLE)
    {
        acc_meas_settle_count++;
    }
    else if (App_SharedErrorTable_HasAnyCriticalErrorSet(error_table))
    {
        status = false;
        App_SharedStateMachine_SetNextState(state_machine, App_GetFaultState());
    }

    return status;
}
