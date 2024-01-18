#include "states/app_allStates.h"
#include "app_sbgEllipse.h"
#include "io_led.h"
#include "App_CanTx.h"
#include "App_CanRx.h"
#include "App_CanAlerts.h"
#include "configs/App_HeartbeatMonitorConfig.h"
#include "App_SharedHeartbeatMonitor.h"
#include "app_globals.h"
#include "io_sbgEllipse.h"

#define IGNORE_HEARTBEAT_CYCLES 3U

static uint16_t num_cycles = 0;

static void sendAndReceiveHeartbeat(void)
{
    App_CanTx_DCM_Heartbeat_Set(true);

    if (App_CanRx_BMS_Heartbeat_Get())
    {
        App_SharedHeartbeatMonitor_CheckIn(globals->hb_monitor, BMS_HEARTBEAT_ONE_HOT);
        App_CanRx_BMS_Heartbeat_Update(false);
    }

    if (App_CanRx_FSM_Heartbeat_Get())
    {
        App_SharedHeartbeatMonitor_CheckIn(globals->hb_monitor, FSM_HEARTBEAT_ONE_HOT);
        App_CanRx_FSM_Heartbeat_Update(false);
    }

    if (App_CanRx_DIM_Heartbeat_Get())
    {
        App_SharedHeartbeatMonitor_CheckIn(globals->hb_monitor, DIM_HEARTBEAT_ONE_HOT);
        App_CanRx_DIM_Heartbeat_Update(false);
    }
}

void app_allStates_runOnTick1Hz(struct StateMachine *const state_machine)
{
    UNUSED(state_machine);
}

bool app_allStates_runOnTick100Hz(struct StateMachine *const state_machine)
{
    sendAndReceiveHeartbeat();

    const bool brake_actuated = App_CanRx_FSM_BrakeActuated_Get();
    io_led_enable(globals->config->brake_light, brake_actuated);

    if (num_cycles <= IGNORE_HEARTBEAT_CYCLES)
    {
        num_cycles++;
    }

    const bool is_missing_hb =
        !App_SharedHeartbeatMonitor_Tick(globals->hb_monitor) && num_cycles > IGNORE_HEARTBEAT_CYCLES;
    App_CanAlerts_DCM_Fault_MissingHeartbeat_Set(is_missing_hb);

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

    io_sbgEllipse_handleLogs();
    app_sbgEllipse_broadcast();

    // Set status to false (which blocks drive) if either inverter is faulted, or another board has set a fault.
    return !(left_inverter_fault || right_inverter_fault || any_board_has_fault);
}
