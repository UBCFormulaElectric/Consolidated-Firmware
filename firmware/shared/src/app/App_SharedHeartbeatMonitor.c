#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>

#include "App_SharedHeartbeatMonitor.h"

struct HeartbeatMonitor* App_SharedHeartbeatMonitor_Create(
    uint32_t (*get_current_ms)(void),
    uint32_t timeout_period_ms,
    bool     boards_to_check[HEARTBEAT_BOARD_COUNT],
    bool (*getters[HEARTBEAT_BOARD_COUNT])(),
    void (*updaters[HEARTBEAT_BOARD_COUNT])(bool),
    void (*setter)(bool),
    void (*fault_setters[HEARTBEAT_BOARD_COUNT])(bool),
    bool (*fault_getters[HEARTBEAT_BOARD_COUNT])())
{
    struct HeartbeatMonitor* const heartbeat_monitor = malloc(sizeof(struct HeartbeatMonitor));
    assert(heartbeat_monitor != NULL);

    heartbeat_monitor->setter = setter;

    heartbeat_monitor->get_current_ms      = get_current_ms;
    heartbeat_monitor->timeout_period_ms   = timeout_period_ms;
    heartbeat_monitor->previous_timeout_ms = 0U;

    for (int board = 0; board < HEARTBEAT_BOARD_COUNT; board++)
    {
        heartbeat_monitor->heartbeats_checked_in[board] = false;
        heartbeat_monitor->status[board]                = true;
        heartbeat_monitor->heartbeats_to_check[board]   = boards_to_check[board];
        heartbeat_monitor->getters[board]               = getters[board];
        heartbeat_monitor->updaters[board]              = updaters[board];
        heartbeat_monitor->fault_setters[board]         = fault_setters[board];
        heartbeat_monitor->fault_getters[board]         = fault_getters[board];
    }

    return heartbeat_monitor;
}

void App_SharedHeartbeatMonitor_Destroy(struct HeartbeatMonitor* const heartbeat_monitor)
{
    free(heartbeat_monitor);
}

void App_SharedHeartbeatMonitor_Tick(struct HeartbeatMonitor* const heartbeat_monitor)
{
    const uint32_t current_ms = heartbeat_monitor->get_current_ms();

    if ((current_ms - heartbeat_monitor->previous_timeout_ms) >= heartbeat_monitor->timeout_period_ms)
    {
        heartbeat_monitor->previous_timeout_ms = current_ms;

        // a flag in status is true under two conditions
        // 1) if it has been checked in
        // 2) it was not on our list of heartbeats to check
        for (int board = 0; board < HEARTBEAT_BOARD_COUNT; board++)
        {
            heartbeat_monitor->status[board] =
                heartbeat_monitor->heartbeats_checked_in[board] || !heartbeat_monitor->heartbeats_to_check[board];

            heartbeat_monitor->heartbeats_checked_in[board] = false;
        }
    }
}

// checks in boards for tick later
// getters - getter functions for heartbeats
// updaters - update functions for heartbeats on local can tables
// boardSetter - update function for current board
void App_SharedHeartbeatMonitor_CheckIn(struct HeartbeatMonitor* const heartbeat_monitor)
{
    // check in current heartbeat
    (heartbeat_monitor->setter)(true);

    for (int board = 0; board < HEARTBEAT_BOARD_COUNT; board++)
    {
        // if boards heartbeat returned
        if (heartbeat_monitor->getters[board] != NULL && (*heartbeat_monitor->getters[board])())
        {
            // check in, and reset board on local CAN table
            heartbeat_monitor->heartbeats_checked_in[board] = true;
            if (heartbeat_monitor->updaters[board] != NULL)
            {
                (*heartbeat_monitor->updaters[board])(false);
            }
        }
    }
}

// gets state to broadcast via can, and can callbacks to use to broadcast
void App_SharedHeartbeatMonitor_BroadcastFaults(struct HeartbeatMonitor* const heartbeat_monitor)
{
    for (int board = 0; board < HEARTBEAT_BOARD_COUNT; board++)
    {
        // send fault over CAN if state bit is false
        if (heartbeat_monitor->fault_setters[board] != NULL)
        {
            (*heartbeat_monitor->fault_setters[board])(!heartbeat_monitor->status[board]);
        }
    }
}

// gets a list of heartbeat getters
// returns false if all are false, else return true
bool App_SharedHeartbeatMonitor_CheckFaults(struct HeartbeatMonitor* const heartbeat_monitor)
{
    bool res = false;

    // or over all bool results in callbacks
    for (int board = 0; board < HEARTBEAT_BOARD_COUNT; board++)
    {
        if (heartbeat_monitor->fault_getters[board] != NULL)
        {
            res |= (*heartbeat_monitor->fault_getters[board])();
        }
    }

    return res;
}