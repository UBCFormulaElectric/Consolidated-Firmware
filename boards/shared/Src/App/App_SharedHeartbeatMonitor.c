#include <assert.h>
#include <stdlib.h>

#include "App_SharedHeartbeatMonitor.h"

struct HeartbeatMonitor
{
    uint32_t (*get_current_ms)(void);
    uint32_t             timeout_period_ms;
    uint32_t             last_check_in_time_ms;
    enum HeartbeatOneHot heartbeats_checked_in;
    enum HeartbeatOneHot heartbeats_to_check;
};

struct HeartbeatMonitor *App_SharedHeartbeatMonitor_Create(
    uint32_t (*const get_current_ms)(void),
    uint32_t             timeout_period_ms,
    enum HeartbeatOneHot heartbeats_to_check)
{
    struct HeartbeatMonitor *const heartbeat_monitor = malloc(sizeof(struct HeartbeatMonitor));
    assert(heartbeat_monitor != NULL);

    heartbeat_monitor->get_current_ms        = get_current_ms;
    heartbeat_monitor->timeout_period_ms     = timeout_period_ms;
    heartbeat_monitor->last_check_in_time_ms = get_current_ms();
    heartbeat_monitor->heartbeats_checked_in = 0;
    heartbeat_monitor->heartbeats_to_check   = heartbeats_to_check;

    return heartbeat_monitor;
}

void App_SharedHeartbeatMonitor_Destroy(struct HeartbeatMonitor *const heartbeat_monitor)
{
    free(heartbeat_monitor);
}

bool App_SharedHeartbeatMonitor_Tick(struct HeartbeatMonitor *const heartbeat_monitor)
{
    static bool status = true;

    const uint32_t current_ms = heartbeat_monitor->get_current_ms();
    const bool     timeout_detected =
        (current_ms - heartbeat_monitor->last_check_in_time_ms) >= heartbeat_monitor->timeout_period_ms;

    if (timeout_detected)
    {
        heartbeat_monitor->last_check_in_time_ms += heartbeat_monitor->timeout_period_ms;

        // Check if the board received all the heartbeats it's listening for
        status = heartbeat_monitor->heartbeats_to_check == heartbeat_monitor->heartbeats_checked_in;

        // Clear the list of boards that have checked in
        heartbeat_monitor->heartbeats_checked_in = 0U;
    }

    return status;
}

void App_SharedHeartbeatMonitor_CheckIn(
    struct HeartbeatMonitor *const heartbeat_monitor,
    enum HeartbeatOneHot           heartbeat)
{
    heartbeat_monitor->heartbeats_checked_in |= heartbeat;
}
