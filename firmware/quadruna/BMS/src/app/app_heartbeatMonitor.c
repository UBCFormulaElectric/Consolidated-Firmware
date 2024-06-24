#include "app_heartbeatMonitor.h"
#include "app_heartbeatMonitorBoard.h"

#include "app_canRx.h"
#include "app_canTx.h"
#include "app_canAlerts.h"

HeartbeatMonitorBoard vc_hbmonitor = {
    .getter       = app_canRx_VC_Heartbeat_get,
    .resetter     = app_canRx_VC_Heartbeat_update,
    .fault_setter = app_canAlerts_BMS_Warning_MissingVCHeartbeat_set,
    .fault_getter = app_canAlerts_BMS_Warning_MissingVCHeartbeat_get,
    .timeout_ms   = 200,
};

static bool block_faults = false;

void app_heartbeatMonitor_init(bool block_faults_init)
{
    block_faults = block_faults_init;
    app_heartbeatMonitorBoard_init(&vc_hbmonitor);
}

void app_heartbeatMonitor_checkIn(void)
{
    app_canTx_BMS_Heartbeat_set(true);
    app_heartbeatMonitorBoard_checkIn(&vc_hbmonitor);
}

void app_heartbeatMonitor_broadcastFaults(void)
{
    if (block_faults)
        return;
    app_heartbeatMonitorBoard_broadcastFaults(&vc_hbmonitor);
}

bool app_heartbeatMonitor_isSendingMissingHeartbeatFault(void)
{
    if (block_faults)
        return false;
    return app_heartbeatMonitorBoard_isSendingMissingHeartbeatFault(&vc_hbmonitor);
}

#ifdef TARGET_TEST
void app_heartbeatMonitor_clearFaults(void)
{
    app_heartbeatMonitorBoard_clearFaults(&vc_hbmonitor);
}
#endif