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

void app_allStates_runOnTick100Hz(void)
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
    io_sbgEllipse_handleLogs();
    app_sbgEllipse_broadcast();

    // Set status to false (which blocks drive) if either inverter is faulted, or another board has set a fault.
}

void app_allStates_runOnTick1Hz() {}