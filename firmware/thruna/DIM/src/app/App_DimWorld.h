#pragma once

#include "App_CanTx.h"
#include "App_CanRx.h"
#include "App_CanAlerts.h"
#include "App_SharedHeartbeatMonitor.h"
#include "App_SharedClock.h"
#include "App_AvgPower.h"

struct DimWorld;

/**
 * Allocate and initialize a world in the application layer by registering
 * interfaces to it
 * @note This function does __not__ take ownership of any of the interfaces
 *       passed into it, which means the every interface must be kept alive for
 *       the lifetime of the created DIM world
 * @return A pointer to the created world, whose ownership is given to the
 * caller
 */
struct DimWorld *App_DimWorld_Create(
    struct HeartbeatMonitor *heartbeat_monitor,
    struct Clock *           clock,
    struct AvgPowerCalc *    avgPowerCalc);

/**
 * Deallocate the memory used by the given world
 * @param world The world to deallocate
 */
void App_DimWorld_Destroy(struct DimWorld *world);

/**
 * Get the heartbeat monitor for the given world
 * @param world The world to get heartbeat monitor for
 * @return The heartbeat monitor for the given world
 */
struct HeartbeatMonitor *App_DimWorld_GetHeartbeatMonitor(const struct DimWorld *world);

/**
 * Get the clock for the given world
 * @param world The world to get clock for
 * @return The clock for the given world
 */
struct Clock *App_DimWorld_GetClock(const struct DimWorld *world);

/**
 * Get the average power for the given world
 * @param world
 * @return The struct with the running and average power
 */
struct AvgPowerCalc *App_DimWorld_GetAvgPowerCalc(const struct DimWorld *world);
