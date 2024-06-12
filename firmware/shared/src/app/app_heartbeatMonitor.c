#include "app_heartbeatMonitor.h"
#include "app_heartbeatBoardsEnum.h"
#include "app_timer.h"
#include <assert.h>
#include <stdlib.h>

static TimerChannel timers[HEARTBEAT_BOARD_COUNT];
// static TimerChannel init_timer;

static HeartbeatMonitor hb_monitor;

void app_heartbeatMonitor_init(
    const bool boards_to_check[HEARTBEAT_BOARD_COUNT],
    bool (*const getters[HEARTBEAT_BOARD_COUNT])(void),
    void (*const updaters[HEARTBEAT_BOARD_COUNT])(bool),
    void (*self_checkiner)(bool),
    void (*const fault_setters[HEARTBEAT_BOARD_COUNT])(bool),
    bool (*const fault_getters[HEARTBEAT_BOARD_COUNT])(void))
{
    hb_monitor.self_checkin = self_checkiner;
    hb_monitor.block_faults = false;

    for (int board = 0; board < HEARTBEAT_BOARD_COUNT; board++)
    {
        hb_monitor.heartbeats_checked_in[board]     = false;
        hb_monitor.status[board]                    = false;
        hb_monitor.is_watching_heartbeat_for[board] = boards_to_check[board];
        hb_monitor.getters[board]                   = getters[board];
        hb_monitor.resetters[board]                 = updaters[board];
        hb_monitor.fault_setters[board]             = fault_setters[board];
        hb_monitor.fault_getters[board]             = fault_getters[board];

        app_timer_init(&timers[board], HEARTBEAT_MONITOR_TIMEOUT_PERIOD_MS); // TODO board specific timeout
        // Initialize timer to expired as to cause fault to be present on power-on (missing heartbeat timeout period
        // initially expired)
        timers[board].state = TIMER_STATE_EXPIRED;

        // app_timer_init(&init_timer, HEARTBEAT_MONITOR_INIT_PERIOD);
        // init_timer.state = TIMER_STATE_RUNNING;

        if (!hb_monitor.is_watching_heartbeat_for[board])
            continue;
        assert((hb_monitor.fault_setters[board] != NULL));
        // By default, set fault for all boards to true on start-up
        hb_monitor.fault_setters[board](true);
    }
}

/**
 * USES
 *  - getters - getter functions for heartbeats
 *  - resetters - update functions for heartbeats on local can tables
 *  - self_checkin - update function for current board
 */
void app_heartbeatMonitor_checkIn(void)
{
    // check in current heartbeat
    (hb_monitor.self_checkin)(true);

    for (int board = 0; board < HEARTBEAT_BOARD_COUNT; board++)
    {
        // if boards heartbeat returned
        if (!hb_monitor.is_watching_heartbeat_for[board])// || app_timer_getElapsedTime(&init_timer) < HEARTBEAT_MONITOR_INIT_PERIOD)
        {
            hb_monitor.status[board] = true;
            continue;
        }
        assert(hb_monitor.getters[board] != NULL && hb_monitor.resetters[board] != NULL);

        // check in, and reset board on local CAN table
        const bool board_status_good            = hb_monitor.getters[board]();
        hb_monitor.heartbeats_checked_in[board] = board_status_good;
        const TimerState state                  = app_timer_runIfCondition(&timers[board], !board_status_good);

        hb_monitor.status[board] = board_status_good || state == TIMER_STATE_RUNNING;
        hb_monitor.resetters[board](false); // reset the CAN table so that it has to be checked in again
    }
}

void app_heartbeatMonitor_broadcastFaults(void)
{
    // Don't check if blocking faults.
    if (hb_monitor.block_faults)
    {
        return;
    }

    for (int board = 0; board < HEARTBEAT_BOARD_COUNT; board++)
    {
        if (!hb_monitor.is_watching_heartbeat_for[board])
            continue;
        assert((hb_monitor.fault_setters[board] != NULL));
        const bool board_has_fault = !hb_monitor.status[board];
        hb_monitor.fault_setters[board](board_has_fault);
    }
}

bool app_heartbeatMonitor_isSendingMissingHeartbeatFault(void)
{
    if (hb_monitor.block_faults)
    {
        return false;
    }

    bool res = false;
    for (int board = 0; board < HEARTBEAT_BOARD_COUNT; board++)
    {
        // TODO switch this to checking if hb_monitor.is_watching_heartbeat_for[board] is true
        if (hb_monitor.fault_getters[board] == NULL)
            continue;
        res |= (*hb_monitor.fault_getters[board])();
    }

    return res;
}

#ifdef TARGET_TEST
void app_heartbeatMonitor_blockFaults(bool block_faults)
{
    hb_monitor.block_faults = block_faults;
}

void app_heartbeatMonitor_clearFaults(void)
{
    for (int board = 0; board < HEARTBEAT_BOARD_COUNT; board++)
    {
        if (!hb_monitor.is_watching_heartbeat_for[board])
            continue;
        assert((hb_monitor.fault_setters[board] != NULL));

        hb_monitor.fault_setters[board](false);
    }
}

HeartbeatMonitor *app_heartbeatMonitor_get(void)
{
    return &hb_monitor;
}
#endif
