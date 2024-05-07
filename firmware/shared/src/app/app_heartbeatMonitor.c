#include "app_heartbeatMonitor.h"
#include "app_heartbeatBoardsEnum.h"
#include "app_timer.h"
#include <assert.h>
#include <stdlib.h>

typedef struct
{
    // CONFIG SETTINGS

    // self_checkin for own heartbeat
    void (*self_checkin)(bool);
    // Override to block heartbeat faults during tests.
    bool block_faults;

    // determines if the heartbeat monitor should check in on a board
    bool is_watching_heartbeat_for[HEARTBEAT_BOARD_COUNT];

    // gives if the heartbeat is checked in.
    // unmonitored heartbeats should be false.
    bool heartbeats_checked_in[HEARTBEAT_BOARD_COUNT];
    // gives if the heartbeat is in a valid state.
    // a flag in status is true under three conditions
    // 1) if it has been checked in
    // 2) it was not on our list of heartbeats to check
    // 3) if the heartbeat_checked_in is true but the timeout has not been elapsed.
    bool status[HEARTBEAT_BOARD_COUNT];

    // HEARTBEAT CAN SIGNALS

    // getters for heartbeats on the CAN table
    bool (*getters[HEARTBEAT_BOARD_COUNT])(void);
    // resetters on the local CAN table for other heartbeats
    // WARNING: only pass false into this function. idk how to make a closure, so we can't make this only return false
    void (*resetters[HEARTBEAT_BOARD_COUNT])(bool);

    // FAULT SETGET

    // fault broadcasters for each board's heartbeat from this board
    void (*fault_setters[HEARTBEAT_BOARD_COUNT])(bool);
    // fault getters for each board's heartbeat from this board
    bool (*fault_getters[HEARTBEAT_BOARD_COUNT])(void);
} HeartbeatMonitor;

#define HEARTBEAT_MONITOR_TIMEOUT_PERIOD_MS 400U

static TimerChannel timers[HEARTBEAT_BOARD_COUNT];

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
        hb_monitor.status[board]                    = true;
        hb_monitor.is_watching_heartbeat_for[board] = boards_to_check[board];
        hb_monitor.getters[board]                   = getters[board];
        hb_monitor.resetters[board]                 = updaters[board];
        hb_monitor.fault_setters[board]             = fault_setters[board];
        hb_monitor.fault_getters[board]             = fault_getters[board];

        app_timer_init(&timers[board], HEARTBEAT_MONITOR_TIMEOUT_PERIOD_MS); // TODO board specific timeout
        app_timer_restart(&timers[board]);
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
        if (!hb_monitor.is_watching_heartbeat_for[board])
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

void app_heartbeatMonitor_blockFaults(bool block_faults)
{
    hb_monitor.block_faults = block_faults;
}

#ifdef TARGET_TEST
HeartbeatMonitor *app_heartbeatMonitor_get(void)
{
    return &hb_monitor;
}
#endif
