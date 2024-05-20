#include "states/app_allStates.h"
// app
#include "app_sbgEllipse.h"
#include "app_canTx.h"
#include "app_heartbeatMonitor.h"
#include "app_lowVoltageBattery.h"
#include "app_currentSensing.h"
#include "app_efuse.h"
#include "app_pumpControl.h"
#include "app_shdnLoop.h"
#include "app_shdnLast.h"
// io
#include "io_sbgEllipse.h"
#include "io_imu.h"

#define IGNORE_HEARTBEAT_CYCLES 3U

static uint16_t heartbeat_cycles = 0;

void app_allStates_runOnTick100Hz(void)
{
    app_lowVoltageBattery_broadcast();
    app_shdn_loop_broadcast();
    app_currentSensing_broadcast();
    app_efuse_broadcast();
    app_shdnLast_broadcast();

    float lin_accel_x = 0;
    float lin_accel_y = 0;
    float lin_accel_z = 0;

    bool has_lin_accel_x = io_imu_getLinearAccelerationX(&lin_accel_x);
    bool has_lin_accel_y = io_imu_getLinearAccelerationY(&lin_accel_y);
    bool has_lin_accel_z = io_imu_getLinearAccelerationZ(&lin_accel_z);

    if (has_lin_accel_x && has_lin_accel_y && has_lin_accel_z)
    {
        app_canTx_VC_ImuAccelerationX_set(lin_accel_x);
        app_canTx_VC_ImuAccelerationY_set(lin_accel_y);
        app_canTx_VC_ImuAccelerationZ_set(lin_accel_z);
    }

    app_heartbeatMonitor_checkIn();

    if (heartbeat_cycles <= IGNORE_HEARTBEAT_CYCLES) // TODO make this part of the heartbeat monitor
        heartbeat_cycles++;
    else
        app_heartbeatMonitor_broadcastFaults();

    io_sbgEllipse_handleLogs();
    app_sbgEllipse_broadcast();

    // Set status to false (which blocks drive) if either inverter is faulted, or another board has set a fault.
}

void app_allStates_runOnTick1Hz(void)
{
    app_canTx_VC_FlowRate_set(app_pumpControl_getFlowRate());
}
