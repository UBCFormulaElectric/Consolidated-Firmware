#include <stddef.h>
#include <stdlib.h>
#include <assert.h>

#include "App_FsmWorld.h"

struct SignalNode
{
    struct Signal *    signal;
    struct SignalNode *next;
};

struct FsmWorld
{
    struct HeartbeatMonitor * heartbeat_monitor;
    struct AcceleratorPedals *papps_and_sapps;
    struct Brake *            brake;
    struct Coolant *          coolant;
    struct Steering *         steering;
    struct Wheels *           wheels;
};

struct FsmWorld *App_FsmWorld_Create(
    struct HeartbeatMonitor *const heartbeat_monitor,

    struct AcceleratorPedals *const papps_and_sapps,
    struct Brake *const             brake,
    struct Coolant *const           coolant,
    struct Steering *const          steering,
    struct Wheels *const            wheels)
{
    struct FsmWorld *world = (struct FsmWorld *)malloc(sizeof(struct FsmWorld));
    assert(world != NULL);

    world->heartbeat_monitor = heartbeat_monitor;

    world->papps_and_sapps = papps_and_sapps;
    world->brake           = brake;
    world->coolant         = coolant;
    world->steering        = steering;
    world->wheels          = wheels;

    return world;
}

void App_FsmWorld_Destroy(struct FsmWorld *world)
{
    free(world);
}

struct HeartbeatMonitor *App_FsmWorld_GetHeartbeatMonitor(const struct FsmWorld *const world)
{
    return world->heartbeat_monitor;
}

struct AcceleratorPedals *App_FsmWorld_GetPappsAndSapps(const struct FsmWorld *const world)
{
    return world->papps_and_sapps;
}

struct Brake *App_FsmWorld_GetBrake(const struct FsmWorld *const world)
{
    return world->brake;
}

struct Coolant *App_FsmWorld_GetCoolant(const struct FsmWorld *const world)
{
    return world->coolant;
}

struct Steering *App_FsmWorld_GetSteering(const struct FsmWorld *const world)
{
    return world->steering;
}

struct Wheels *App_FsmWorld_GetWheels(const struct FsmWorld *const world)
{
    return world->wheels;
}
