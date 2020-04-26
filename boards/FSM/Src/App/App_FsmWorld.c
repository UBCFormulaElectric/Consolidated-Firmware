#include <stddef.h>
#include <stdlib.h>
#include <assert.h>

#include "App_FsmWorld.h"

struct FsmWorld
{
    struct FsmCanTxInterface *can_tx_interface;
    struct FsmCanRxInterface *can_rx_interface;
    struct HeartbeatMonitor * heartbeat_monitor;
    struct FlowMeter *        primary_flow_meter;
    struct FlowMeter *        secondary_flow_meter;
};

struct FsmWorld *App_FsmWorld_Create(
    struct FsmCanTxInterface *const can_tx_interface,
    struct FsmCanRxInterface *const can_rx_interface,
    struct HeartbeatMonitor *const  heartbeat_monitor,
    struct FlowMeter *const         primary_flow_meter,
    struct FlowMeter *const         secondary_flow_meter)
{
    struct FsmWorld *world = (struct FsmWorld *)malloc(sizeof(struct FsmWorld));
    assert(world != NULL);

    world->can_tx_interface     = can_tx_interface;
    world->can_rx_interface     = can_rx_interface;
    world->heartbeat_monitor    = heartbeat_monitor;
    world->primary_flow_meter   = primary_flow_meter;
    world->secondary_flow_meter = secondary_flow_meter;

    return world;
}

void App_FsmWorld_Destroy(struct FsmWorld *world)
{
    free(world);
}

struct FsmCanTxInterface *
    App_FsmWorld_GetCanTx(const struct FsmWorld *const world)
{
    return world->can_tx_interface;
}

struct FsmCanRxInterface *
    App_FsmWorld_GetCanRx(const struct FsmWorld *const world)
{
    return world->can_rx_interface;
}

struct HeartbeatMonitor *
    App_FsmWorld_GetHeartbeatMonitor(const struct FsmWorld *const world)
{
    return world->heartbeat_monitor;
}

struct FlowMeter *
    App_FsmWorld_GetPrimaryFlowMeter(const struct FsmWorld *const world)
{
    return world->primary_flow_meter;
}

struct FlowMeter *
    App_FsmWorld_GetSecondaryFlowMeter(const struct FsmWorld *const world)
{
    return world->secondary_flow_meter;
}
