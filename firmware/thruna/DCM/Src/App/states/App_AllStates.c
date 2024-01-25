#include <stddef.h>
#include "App_SharedMacros.h"
#include "states/App_AllStates.h"
#include "App_EllipseImu.h"

#define IGNORE_HEARTBEAT_CYCLES 3U

static uint16_t num_cycles = 0;

static void App_SendAndReceiveHeartbeat(struct HeartbeatMonitor* hb_monitor)
{
    App_SharedHeartbeatMonitor_CheckIn(hb_monitor);
}

void App_AllStatesRunOnTick1Hz(struct StateMachine* const state_machine)
{
    UNUSED(state_machine);
}

bool App_AllStatesRunOnTick100Hz(struct StateMachine* const state_machine)
{
    bool                     status      = true;
    struct DcmWorld*         world       = App_SharedStateMachine_GetWorld(state_machine);
    struct BrakeLight*       brake_light = App_DcmWorld_GetBrakeLight(world);
    struct HeartbeatMonitor* hb_monitor  = App_DcmWorld_GetHeartbeatMonitor(world);

    App_SendAndReceiveHeartbeat(hb_monitor);

    const bool brake_actuated = App_CanRx_FSM_BrakeActuated_Get();
    App_BrakeLight_SetLightStatus(brake_light, brake_actuated);

    if (num_cycles <= IGNORE_HEARTBEAT_CYCLES)
    {
        num_cycles++;
    }

    App_SharedHeartbeatMonitor_Tick(hb_monitor);

    if (num_cycles > IGNORE_HEARTBEAT_CYCLES)
    {
        App_SharedHeartbeatMonitor_BroadcastFaults(hb_monitor);
    }

    const bool left_inverter_fault  = App_CanRx_INVL_VsmState_Get() == INVERTER_VSM_BLINK_FAULT_CODE_STATE;
    const bool right_inverter_fault = App_CanRx_INVR_VsmState_Get() == INVERTER_VSM_BLINK_FAULT_CODE_STATE;
    App_CanAlerts_DCM_Fault_LeftInverterFault_Set(left_inverter_fault);
    App_CanAlerts_DCM_Fault_RightInverterFault_Set(right_inverter_fault);

    const bool bms_fault           = App_CanAlerts_BoardHasFault(BMS_ALERT_BOARD);
    const bool dcm_fault           = App_CanAlerts_BoardHasFault(DCM_ALERT_BOARD);
    const bool fsm_fault           = App_CanAlerts_BoardHasFault(FSM_ALERT_BOARD);
    const bool pdm_fault           = App_CanAlerts_BoardHasFault(PDM_ALERT_BOARD);
    const bool dim_fault           = App_CanAlerts_BoardHasFault(DIM_ALERT_BOARD);
    const bool any_board_has_fault = bms_fault || dcm_fault || fsm_fault || pdm_fault || dim_fault;

    // Set status to false (which blocks drive) if either inverter is faulted, or another board has set a fault.
    if (left_inverter_fault || right_inverter_fault || any_board_has_fault)
    {
        status = false;
    }

    App_EllipseImu_HandleLogs();
    App_EllipseImu_Broadcast();

    return status;
}
