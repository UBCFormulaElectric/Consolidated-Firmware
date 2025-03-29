#include "states/app_allStates.h"
// app
#include "app_sbgEllipse.h"
#include "app_canTx.h"
#include "app_canRx.h"
#include "app_canAlerts.h"
#include "app_heartbeatMonitor.h"
#include "app_lowVoltageBattery.h"
#include "app_currentSensing.h"
#include "app_efuse.h"
#include "app_pumpControl.h"
#include "app_shdnLoop.h"
#include "app_shdnLast.h"
#include "app_imu.h"
#include "app_loadTransfer.h"
// io
#include "io_sbgEllipse.h"
#include "io_imu.h"
#include "io_canLogging.h"
#include "io_pcm.h"

#include <app_heartbeatMonitors.h>

#define IGNORE_HEARTBEAT_CYCLES 3U

static uint16_t heartbeat_cycles = 0;

void app_allStates_runOnTick100Hz(void)
{
    // Enable PCM if HV up.
    const bool bms_in_drive = app_canRx_BMS_State_get() == BMS_DRIVE_STATE;
    io_pcm_set(bms_in_drive);

    app_lowVoltageBattery_broadcast();
    app_shdnLoop_broadcast();
    app_currentSensing_broadcast();
    app_efuse_broadcast();
    app_shdnLast_broadcast();

    app_collect_imu_data();
    const ImuData *imu = app_get_imu_struct();
    app_wheelVerticalForces_broadcast(imu);

    app_heartbeatMonitor_checkIn(&hb_monitor);

    if (heartbeat_cycles <= IGNORE_HEARTBEAT_CYCLES) // TODO make this part of the heartbeat monitor
        heartbeat_cycles++;
    else
        app_heartbeatMonitor_broadcastFaults(&hb_monitor);

    io_sbgEllipse_handleLogs();
    app_sbgEllipse_broadcast();

    // Set status to false (which blocks drive) if either inverter is faulted, or another board has set a fault.
}

void app_allStates_runOnTick1Hz(void)
{
    app_canTx_VC_FlowRate_set(app_pumpControl_getFlowRate());

    // Update SD card logging related signals.
    app_canTx_VC_CanLoggingRemainingErrors_set(io_canLogging_errorsRemaining());
    app_canAlerts_VC_Warning_CanLoggingErrored_set(io_canLogging_errorsRemaining() == 0);
}
