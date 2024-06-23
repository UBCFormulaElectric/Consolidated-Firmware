#include <assert.h>
#include <stdlib.h>
#include "app_heartbeatMonitor.h"

void app_heartbeatMonitor_init(HeartbeatMonitorBoard *hb)
{
    assert(
        hb->getter != NULL && hb->resetter != NULL && hb->fault_setter != NULL && hb->fault_getter != NULL &&
        hb->timeout_ms > 0);
    hb->heartbeat_checked_in = false;
    hb->status               = false;
    app_timer_init(&hb->timer, hb->timeout_ms);
    hb->timer.state = TIMER_STATE_EXPIRED;
    hb->fault_setter(true);
}

/**
 * USES
 *  - getters - getter functions for heartbeats
 *  - resetters - update functions for heartbeats on local can tables
 *  - self_checkin - update function for current board
 */
void app_heartbeatMonitor_checkIn(HeartbeatMonitorBoard *hb)
{
    // check in, and reset board on local CAN table
    const bool board_status_good = hb->getter();
    hb->heartbeat_checked_in     = board_status_good;
    const TimerState state       = app_timer_runIfCondition(&hb->timer, !board_status_good);

    hb->status = board_status_good || state == TIMER_STATE_RUNNING;
    hb->resetter(false); // reset the CAN table so that it has to be checked in again
}

void app_heartbeatMonitor_broadcastFaults(const HeartbeatMonitorBoard *hb)
{
    hb->fault_setter(!hb->status);
}

bool app_heartbeatMonitor_isSendingMissingHeartbeatFault(const HeartbeatMonitorBoard *hb)
{
    return hb->fault_getter();
}

#ifdef TARGET_TEST
void app_heartbeatMonitor_clearFaults(void)
{
    hb_monitor.fault_setters[board](false);
}
#endif
