#include "states/App_AllStates.h"
#include "states/App_FaultState.h"
#include "App_SetPeriodicCanSignals.h"
#include "App_Accumulator.h"

enum HeartbeatOneHot GetHeartbeat(struct BmsCanRxInterface *can_rx_interface)
{
    enum HeartbeatOneHot heartbeat_check = 0;
    uint8_t              dcm_heartbeat =
        App_CanRx_DCM_HEARTBEAT_GetSignal_DUMMY_VARIABLE(can_rx_interface);
    if (dcm_heartbeat == 1)
    {
        heartbeat_check |= DCM_HEARTBEAT_ONE_HOT;
        App_CanRx_DCM_HEARTBEAT_SetSignal_DUMMY_VARIABLE(can_rx_interface, 0);
    }
    uint8_t fsm_heartbeat =
        App_CanRx_FSM_HEARTBEAT_GetSignal_DUMMY_VARIABLE(can_rx_interface);
    if (fsm_heartbeat == 1)
    {
        heartbeat_check |= FSM_HEARTBEAT_ONE_HOT;
        App_CanRx_FSM_HEARTBEAT_SetSignal_DUMMY_VARIABLE(can_rx_interface, 0);
    }
    uint8_t dim_heartbeat =
        App_CanRx_DIM_HEARTBEAT_GetSignal_DUMMY_VARIABLE(can_rx_interface);
    if (dim_heartbeat == 1)
    {
        heartbeat_check |= DIM_HEARTBEAT_ONE_HOT;
        App_CanRx_DIM_HEARTBEAT_SetSignal_DUMMY_VARIABLE(can_rx_interface, 0);
    }
    uint8_t pdm_heartbeat =
        App_CanRx_PDM_HEARTBEAT_GetSignal_DUMMY_VARIABLE(can_rx_interface);
    if (pdm_heartbeat == 1)
    {
        heartbeat_check |= PDM_HEARTBEAT_ONE_HOT;
        App_CanRx_PDM_HEARTBEAT_SetSignal_DUMMY_VARIABLE(can_rx_interface, 0);
    }

    return (enum HeartbeatOneHot)heartbeat_check;
}

void App_AllStatesRunOnTick1Hz(struct StateMachine *const state_machine)
{
    struct BmsWorld *world = App_SharedStateMachine_GetWorld(state_machine);
    struct BmsCanTxInterface *can_tx = App_BmsWorld_GetCanTx(world);
    struct RgbLedSequence *   rgb_led_sequence =
        App_BmsWorld_GetRgbLedSequence(world);
    struct Charger *charger = App_BmsWorld_GetCharger(world);

    App_SharedRgbLedSequence_Tick(rgb_led_sequence);

    bool charger_is_connected = App_Charger_IsConnected(charger);
    App_CanTx_SetPeriodicSignal_IS_CONNECTED(can_tx, charger_is_connected);
}

void App_AllStatesRunOnTick100Hz(struct StateMachine *const state_machine)
{
    struct BmsWorld *world = App_SharedStateMachine_GetWorld(state_machine);
    struct BmsCanRxInterface *can_rx      = App_BmsWorld_GetCanRx(world);
    struct BmsCanTxInterface *can_tx      = App_BmsWorld_GetCanTx(world);
    struct Imd *              imd         = App_BmsWorld_GetImd(world);
    struct OkStatus *         bms_ok      = App_BmsWorld_GetBmsOkStatus(world);
    struct OkStatus *         imd_ok      = App_BmsWorld_GetImdOkStatus(world);
    struct OkStatus *         bspd_ok     = App_BmsWorld_GetBspdOkStatus(world);
    struct Airs *             airs        = App_BmsWorld_GetAirs(world);
    struct Accumulator *      accumulator = App_BmsWorld_GetAccumulator(world);
    struct ErrorTable *       error_table = App_BmsWorld_GetErrorTable(world);
    struct HeartbeatMonitor * heartbeat_monitor =
        App_BmsWorld_GetHeartbeatMonitor(world);

    App_Accumulator_RunOnTick100Hz(accumulator, can_tx);
    App_SetPeriodicCanSignals_Imd(can_tx, imd);

    App_CanTx_SetPeriodicSignal_AIR_NEGATIVE(
        can_tx, App_SharedBinaryStatus_IsActive(App_Airs_GetAirNegative(airs)));
    App_CanTx_SetPeriodicSignal_AIR_POSITIVE(
        can_tx, App_SharedBinaryStatus_IsActive(App_Airs_GetAirPositive(airs)));

    if (App_OkStatus_IsEnabled(bms_ok))
    {
        App_CanTx_SetPeriodicSignal_BMS_OK(can_tx, true);
    }
    else
    {
        App_CanTx_SetPeriodicSignal_BMS_OK(can_tx, false);
    }

    if (App_OkStatus_IsEnabled(imd_ok))
    {
        App_CanTx_SetPeriodicSignal_IMD_OK(can_tx, true);
    }
    else
    {
        App_CanTx_SetPeriodicSignal_IMD_OK(can_tx, false);
    }

    if (App_OkStatus_IsEnabled(bspd_ok))
    {
        App_CanTx_SetPeriodicSignal_BSPD_OK(can_tx, true);
    }
    else
    {
        App_CanTx_SetPeriodicSignal_BSPD_OK(can_tx, false);
    }

    if (App_SharedErrorTable_HasAnyCriticalErrorSet(error_table))
    {
        App_SharedStateMachine_SetNextState(state_machine, App_GetFaultState());
    }

    App_CanTx_SetPeriodicSignal_HEARTBEAT(can_tx, true);

    App_SharedHeartbeatMonitor_CheckIn(heartbeat_monitor, GetHeartbeat(can_rx));

    App_SharedHeartbeatMonitor_Tick(heartbeat_monitor);
}
