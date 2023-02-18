#include <stddef.h>
#include <stdlib.h>
#include <assert.h>

#include "App_PdmWorld.h"

struct PdmWorld
{
    struct HeartbeatMonitor * heartbeat_monitor;
    struct RgbLedSequence *   rgb_led_sequence;
    struct LowVoltageBattery *low_voltage_battery;
    struct Clock *            clock;
    struct Efuse *            efuse1;
    struct Efuse *            efuse2;
    struct Efuse *            efuse3;
    struct Efuse *            efuse4;
    struct RailMonitoring *   rail_monitor;
};

struct PdmWorld *App_PdmWorld_Create(
    struct HeartbeatMonitor *const  heartbeat_monitor,
    struct RgbLedSequence *const    rgb_led_sequence,
    struct LowVoltageBattery *const low_voltage_battery,
    struct Clock *const             clock,
    struct Efuse *const             efuse1,
    struct Efuse *const             efuse2,
    struct Efuse *const             efuse3,
    struct Efuse *const             efuse4,
    struct RailMonitoring *const    rail_monitor)
{
    struct PdmWorld *world = (struct PdmWorld *)malloc(sizeof(struct PdmWorld));
    assert(world != NULL);

    world->heartbeat_monitor   = heartbeat_monitor;
    world->rgb_led_sequence    = rgb_led_sequence;
    world->low_voltage_battery = low_voltage_battery;
    world->clock               = clock;
    world->efuse1              = efuse1;
    world->efuse2              = efuse2;
    world->efuse3              = efuse3;
    world->efuse4              = efuse4;
    world->rail_monitor        = rail_monitor;

    return world;
}

void App_PdmWorld_Destroy(struct PdmWorld *const world)
{
    free(world);
}

struct HeartbeatMonitor *App_PdmWorld_GetHeartbeatMonitor(const struct PdmWorld *const world)
{
    return world->heartbeat_monitor;
}

struct RgbLedSequence *App_PdmWorld_GetRgbLedSequence(const struct PdmWorld *const world)
{
    return world->rgb_led_sequence;
}

struct LowVoltageBattery *App_PdmWorld_GetLowVoltageBattery(const struct PdmWorld *const world)
{
    return world->low_voltage_battery;
}

struct Clock *App_PdmWorld_GetClock(const struct PdmWorld *const world)
{
    return world->clock;
}

struct Efuse *App_PdmWorld_GetEfuse1(const struct PdmWorld *world)
{
    return world->efuse1;
}

struct Efuse *App_PdmWorld_GetEfuse2(const struct PdmWorld *world)
{
    return world->efuse2;
}

struct Efuse *App_PdmWorld_GetEfuse3(const struct PdmWorld *world)
{
    return world->efuse3;
}

struct Efuse *App_PdmWorld_GetEfuse4(const struct PdmWorld *world)
{
    return world->efuse4;
}

struct RailMonitoring *App_PdmWorld_GetRailMonitoring(const struct PdmWorld *world)
{
    return world->rail_monitor;
}
