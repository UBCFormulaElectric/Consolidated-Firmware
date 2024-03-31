#include <stddef.h>
#include "states/app_allStates.h"

#include "app_sbgEllipse.h"
#include "io_led.h"
#include "app_canTx.h"
#include "app_canRx.h"
#include "app_canAlerts.h"
#include "app_heartbeatMonitor.h"
#include "app_globals.h"
#include "io_sbgEllipse.h"
#include "app_lowVoltageBattery.h"
#include "app_shutdown.h"
#include "app_currentSensing.h"
#include "app_efuse.h"
#include "app_utils.h"
#include "io_time.h"
#include "app_pumpControl.h"
#include "io_imu.h"

#define IGNORE_HEARTBEAT_CYCLES 3U

static uint16_t num_cycles = 0;

bool app_allStates_runOnTick100Hz(void)
{
    app_lowVoltageBattery_broadcast();
    app_shutdown_broadcast();
    app_currentSensing_broadcast();
    app_efuse_broadcast();

    app_heartbeatMonitor_checkIn();
    app_heartbeatMonitor_tick();
    app_heartbeatMonitor_broadcastFaults();

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
    app_canAlerts_VC_Fault_LeftInverterFault_set(left_inverter_fault);
    app_canAlerts_VC_Fault_RightInverterFault_set(right_inverter_fault);

    const bool bms_fault  = app_canAlerts_BoardHasFault(BMS_ALERT_BOARD);
    const bool vc_fault   = app_canAlerts_BoardHasFault(VC_ALERT_BOARD);
    const bool fsm_fault  = app_canAlerts_BoardHasFault(FSM_ALERT_BOARD);
    const bool crit_fault = app_canAlerts_BoardHasFault(CRIT_ALERT_BOARD);

    // once crit dim is done add dim_fault
    const bool any_board_has_fault = bms_fault || vc_fault || fsm_fault || crit_fault;

    io_sbgEllipse_handleLogs();
    app_sbgEllipse_broadcast();

    // Set status to false (which blocks drive) if either inverter is faulted, or another board has set a fault.
    return !(left_inverter_fault || right_inverter_fault || any_board_has_fault);
}

void app_allStates_runOnTick1Hz() {}