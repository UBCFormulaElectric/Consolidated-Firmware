#include "app_heartbeatMonitor.h"
#include "app_heartbeatMonitorBoard.h"

#include "app_canTx.h"
#include "app_canRx.h"
#include "app_canAlerts.h"

HeartbeatMonitorBoard bms_hbmonitor = { .getter       = app_canRx_BMS_Heartbeat_get,
                                        .resetter     = app_canRx_BMS_Heartbeat_update,
                                        .fault_setter = app_canAlerts_RSM_Fault_MissingBMSHeartbeat_set,
                                        .fault_getter = app_canAlerts_RSM_Fault_MissingBMSHeartbeat_get,
                                        .timeout_ms   = 200 };

HeartbeatMonitorBoard vc_hbmonitor = { .getter       = app_canRx_VC_Heartbeat_get,
                                       .resetter     = app_canRx_VC_Heartbeat_update,
                                       .fault_setter = app_canAlerts_RSM_Fault_MissingVCHeartbeat_set,
                                       .fault_getter = app_canAlerts_RSM_Fault_MissingVCHeartbeat_get,
                                       .timeout_ms   = 200 };

HeartbeatMonitorBoard fsm_hbmonitor = { .getter       = app_canRx_FSM_Heartbeat_get,
                                        .resetter     = app_canRx_FSM_Heartbeat_update,
                                        .fault_setter = app_canAlerts_RSM_Fault_MissingFSMHeartbeat_set,
                                        .fault_getter = app_canAlerts_RSM_Fault_MissingFSMHeartbeat_get,
                                        .timeout_ms   = 200 };

static bool block_faults = false;

void app_heartbeatMonitor_init(bool block_faults_init)
{
    block_faults = block_faults_init;
    app_heartbeatMonitorBoard_init(&bms_hbmonitor);
    app_heartbeatMonitorBoard_init(&vc_hbmonitor);
    app_heartbeatMonitorBoard_init(&fsm_hbmonitor);
}

void app_heartbeatMonitor_checkIn(void)
{
    app_canTx_RSM_Heartbeat_set(true);
    app_heartbeatMonitorBoard_checkIn(&bms_hbmonitor);
    app_heartbeatMonitorBoard_checkIn(&vc_hbmonitor);
    app_heartbeatMonitorBoard_checkIn(&fsm_hbmonitor);
}

void app_heartbeatMonitor_broadcastFaults(void)
{
    if (block_faults)
        return;
    app_heartbeatMonitorBoard_broadcastFaults(&bms_hbmonitor);
    app_heartbeatMonitorBoard_broadcastFaults(&vc_hbmonitor);
    app_heartbeatMonitorBoard_broadcastFaults(&fsm_hbmonitor);
}

bool app_heartbeatMonitor_isSendingMissingHeartbeatFault(void)
{
    if (block_faults)
        return false;
    return app_heartbeatMonitorBoard_isSendingMissingHeartbeatFault(&bms_hbmonitor) ||
           app_heartbeatMonitorBoard_isSendingMissingHeartbeatFault(&vc_hbmonitor) ||
           app_heartbeatMonitorBoard_isSendingMissingHeartbeatFault(&fsm_hbmonitor);
}

#ifdef TARGET_TEST
void app_heartbeatMonitor_clearFaults(void)
{
    app_heartbeatMonitorBoard_clearFaults(&bms_hbmonitor);
    app_heartbeatMonitorBoard_clearFaults(&vc_hbmonitor);
    app_heartbeatMonitorBoard_clearFaults(&fsm_hbmonitor);
}
#endif