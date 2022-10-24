#include <stddef.h>
#include <stdlib.h>
#include <assert.h>
#include <linkedlist.h>

#include "App_FsmWorld.h"
#include "configs/App_SignalCallbackDurations.h"

struct SignalNode
{
    struct Signal *    signal;
    struct SignalNode *next;
};

struct FsmWorld
{
    struct FsmCanTxInterface *can_tx_interface;
    struct FsmCanRxInterface *can_rx_interface;
    struct HeartbeatMonitor * heartbeat_monitor;
    struct Clock *            clock;
    struct AcceleratorPedals *papps_and_sapps;
    struct Brake *            brake;
    struct Coolant *          coolant;
    struct Steering *         steering;
    struct Wheels *           wheels;
    struct RgbLedSequence *   rgb_led_sequence;
};

struct FsmWorld *App_FsmWorld_Create(
    struct FsmCanTxInterface *const can_tx_interface,
    struct FsmCanRxInterface *const can_rx_interface,
    struct HeartbeatMonitor *const  heartbeat_monitor,
    struct Clock *const             clock,

    struct AcceleratorPedals *const papps_and_sapps,
    struct Brake *const             brake,
    struct Coolant *const           coolant,
    struct Steering *const          steering,
    struct Wheels *const            wheels,

    struct RgbLedSequence *const rgb_led_sequence)
{
    struct FsmWorld *world = (struct FsmWorld *)malloc(sizeof(struct FsmWorld));
    assert(world != NULL);

    world->can_tx_interface  = can_tx_interface;
    world->can_rx_interface  = can_rx_interface;
    world->heartbeat_monitor = heartbeat_monitor;
    world->clock             = clock;

    world->papps_and_sapps  = papps_and_sapps;
    world->brake            = brake;
    world->coolant          = coolant;
    world->steering         = steering;
    world->wheels           = wheels;
    world->rgb_led_sequence = rgb_led_sequence;

    return world;
}

void App_FsmWorld_Destroy(struct FsmWorld *world)
{
    free(world);
}

struct FsmCanTxInterface *App_FsmWorld_GetCanTx(const struct FsmWorld *const world)
{
    return world->can_tx_interface;
}

struct FsmCanRxInterface *App_FsmWorld_GetCanRx(const struct FsmWorld *const world)
{
    return world->can_rx_interface;
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

struct RgbLedSequence *App_FsmWorld_GetRgbLedSequence(const struct FsmWorld *const world)
{
    return world->rgb_led_sequence;
}

struct Clock *App_FsmWorld_GetClock(const struct FsmWorld *const world)
{
    return world->clock;
}
