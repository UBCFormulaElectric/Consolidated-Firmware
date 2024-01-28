#include "states/app_allStates.h"
#include "app_sbgEllipse.h"
#include "io_led.h"
#include "app_canTx.h"
#include "app_canRx.h"
#include "app_canAlerts.h"
#include "app_heartbeatMonitor.h"
#include "app_globals.h"
#include "io_sbgEllipse.h"

#define IGNORE_HEARTBEAT_CYCLES 3U

static uint16_t num_cycles = 0;

static void sendAndReceiveHeartbeat(void) {}

bool app_allStates_runOnTick100Hz(void)
{
    sendAndReceiveHeartbeat();

    const bool brake_actuated = app_canRx_FSM_BrakeActuated_get();
    io_led_enable(globals->config->brake_light, brake_actuated);

    if (num_cycles <= IGNORE_HEARTBEAT_CYCLES)
    {
        num_cycles++;
    }

    app_heartbeatMonitor_checkIn();
    app_heartbeatMonitor_tick();

    if (num_cycles > IGNORE_HEARTBEAT_CYCLES)
    {
        app_heartbeatMonitor_broadcastFaults();
    }

    const bool left_inverter_fault  = app_canRx_INVL_VsmState_get() == INVERTER_VSM_BLINK_FAULT_CODE_STATE;
    const bool right_inverter_fault = app_canRx_INVR_VsmState_get() == INVERTER_VSM_BLINK_FAULT_CODE_STATE;
    app_canAlerts_DCM_Fault_LeftInverterFault_set(left_inverter_fault);
    app_canAlerts_DCM_Fault_RightInverterFault_set(right_inverter_fault);

    const bool bms_fault           = app_canAlerts_BoardHasFault(BMS_ALERT_BOARD);
    const bool dcm_fault           = app_canAlerts_BoardHasFault(DCM_ALERT_BOARD);
    const bool fsm_fault           = app_canAlerts_BoardHasFault(FSM_ALERT_BOARD);
    const bool pdm_fault           = app_canAlerts_BoardHasFault(PDM_ALERT_BOARD);
    const bool dim_fault           = app_canAlerts_BoardHasFault(DIM_ALERT_BOARD);
    const bool any_board_has_fault = bms_fault || dcm_fault || fsm_fault || pdm_fault || dim_fault;

    io_sbgEllipse_handleLogs();
    app_sbgEllipse_broadcast();

    // Set status to false (which blocks drive) if either inverter is faulted, or another board has set a fault.
    return !(left_inverter_fault || right_inverter_fault || any_board_has_fault);
}
