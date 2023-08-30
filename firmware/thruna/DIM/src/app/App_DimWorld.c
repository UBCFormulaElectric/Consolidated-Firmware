#include <stddef.h>
#include <stdlib.h>
#include <assert.h>

#include "App_DimWorld.h"

struct DimWorld
{
    struct HeartbeatMonitor *heartbeat_monitor;
    struct Clock *           clock;
    struct AvgPowerCalc *    avg_power_calc;
};

struct DimWorld *App_DimWorld_Create(
    struct HeartbeatMonitor *const heartbeat_monitor,
    struct Clock *const            clock,
    struct AvgPowerCalc *const     avg_power_calc)
{
    struct DimWorld *world = (struct DimWorld *)malloc(sizeof(struct DimWorld));
    assert(world != NULL);

    world->heartbeat_monitor  = heartbeat_monitor;
    world->clock              = clock;
    world->avg_power_calc     = avg_power_calc;

    return world;
}

void App_DimWorld_Destroy(struct DimWorld *world)
{
    free(world);
}

struct HeartbeatMonitor *App_DimWorld_GetHeartbeatMonitor(const struct DimWorld *const world)
{
    return world->heartbeat_monitor;
}

struct Clock *App_DimWorld_GetClock(const struct DimWorld *world)
{
    return world->clock;
}

struct AvgPowerCalc *App_DimWorld_GetAvgPowerCalc(const struct DimWorld *world)
{
    return world->avg_power_calc;
}
