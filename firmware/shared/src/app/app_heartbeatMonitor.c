#include "app_heartbeatMonitor.h"
#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>
#include "io_time.h"

static HeartbeatMonitor hb_monitor;

void app_heartbeatMonitor_init(
    uint32_t timeout_period_ms,
    bool     boards_to_check[HEARTBEAT_BOARD_COUNT],
    bool (*getters[HEARTBEAT_BOARD_COUNT])(),
    void (*updaters[HEARTBEAT_BOARD_COUNT])(bool),
    void (*setter)(bool),
    void (*fault_setters[HEARTBEAT_BOARD_COUNT])(bool),
    bool (*fault_getters[HEARTBEAT_BOARD_COUNT])())
{
    hb_monitor.setter              = setter;
    hb_monitor.timeout_period_ms   = timeout_period_ms;
    hb_monitor.previous_timeout_ms = 0U;
    hb_monitor.block_faults        = false;

    for (int board = 0; board < HEARTBEAT_BOARD_COUNT; board++)
    {
        hb_monitor.heartbeats_checked_in[board] = false;
        hb_monitor.status[board]                = true;
        hb_monitor.heartbeats_to_check[board]   = boards_to_check[board];
        hb_monitor.getters[board]               = getters[board];
        hb_monitor.updaters[board]              = updaters[board];
        hb_monitor.fault_setters[board]         = fault_setters[board];
        hb_monitor.fault_getters[board]         = fault_getters[board];
    }
}

void app_heartbeatMonitor_tick(void)
{
    const uint32_t current_ms = io_time_getCurrentMs();

    if ((current_ms - hb_monitor.previous_timeout_ms) >= hb_monitor.timeout_period_ms)
    {
        hb_monitor.previous_timeout_ms = current_ms;

        // a flag in status is true under two conditions
        // 1) if it has been checked in
        // 2) it was not on our list of heartbeats to check
        for (int board = 0; board < HEARTBEAT_BOARD_COUNT; board++)
        {
            hb_monitor.status[board] =
                hb_monitor.heartbeats_checked_in[board] || !hb_monitor.heartbeats_to_check[board];

            hb_monitor.heartbeats_checked_in[board] = false;
        }
    }
}

// checks in boards for tick later
// getters - getter functions for heartbeats
// updaters - update functions for heartbeats on local can tables
// boardSetter - update function for current board
void app_heartbeatMonitor_checkIn(void)
{
    // check in current heartbeat
    (hb_monitor.setter)(true);

    for (int board = 0; board < HEARTBEAT_BOARD_COUNT; board++)
    {
        // if boards heartbeat returned
        if (hb_monitor.getters[board] != NULL && (*hb_monitor.getters[board])())
        {
            // check in, and reset board on local CAN table
            hb_monitor.heartbeats_checked_in[board] = true;
            if (hb_monitor.updaters[board] != NULL)
            {
                (*hb_monitor.updaters[board])(false);
            }
        }
    }
}

// gets state to broadcast via can, and can callbacks to use to broadcast
void app_heartbeatMonitor_broadcastFaults(void)
{
    // Don't check if blocking faults.
    if (hb_monitor.block_faults)
    {
        return;
    }

    for (int board = 0; board < HEARTBEAT_BOARD_COUNT; board++)
    {
        // send fault over CAN if state bit is false
        if (hb_monitor.fault_setters[board] != NULL)
        {
            (*hb_monitor.fault_setters[board])(!hb_monitor.status[board]);
        }
    }
}

// gets a list of heartbeat getters
// returns false if all are false, else return true
bool app_heartbeatMonitor_checkFaults(void)
{
    if (hb_monitor.block_faults)
    {
        return false;
    }

    // or over all bool results in callbacks
    bool res = false;
    for (int board = 0; board < HEARTBEAT_BOARD_COUNT; board++)
    {
        if (hb_monitor.fault_getters[board] != NULL)
        {
            res |= (*hb_monitor.fault_getters[board])();
        }
    }

    return res;
}

void app_heartbeatMonitor_blockFaults(bool block_faults)
{
    hb_monitor.block_faults = block_faults;
}

HeartbeatMonitor *app_heartbeatMonitor_get(void)
{
    return &hb_monitor;
}
