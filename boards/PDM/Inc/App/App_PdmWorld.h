#pragma once

#include "App_CanTx.h"
#include "App_CanRx.h"
#include "App_CanAlerts.h"
#include "App_InRangeCheck.h"
#include "App_SharedHeartbeatMonitor.h"
#include "App_SharedRgbLedSequence.h"
#include "App_LowVoltageBattery.h"
#include "App_RailMonitoring.h"
#include "App_SharedClock.h"
#include "App_Efuse.h"

struct PdmWorld;

/**
 * Allocate and initialize a world in the application layer by registering
 * interfaces to it
 * @note This function does __not__ take ownership of any of the interfaces
 *       passed into it, which means the every interface must be kept alive for
 *       the lifetime of the created world
 * @return A pointer to the created world, whose ownership is given to the
 *         caller
 */
struct PdmWorld *App_PdmWorld_Create(
    struct HeartbeatMonitor * heartbeat_monitor,
    struct RgbLedSequence *   rgb_led_sequence,
    struct LowVoltageBattery *low_voltage_battery,
    struct Clock *            clock,
    struct Efuse *            efuse1,
    struct Efuse *            efuse2,
    struct Efuse *            efuse3,
    struct Efuse *            efuse4,
    struct RailMonitoring *   rail_monitor);

/**
 * Deallocate the memory used by the given world
 * @param world The world to deallocate
 */
void App_PdmWorld_Destroy(struct PdmWorld *world);

/**
 * Get the heartbeat monitor for the given world
 * @param world The world to get heartbeat monitor for
 * @return The heartbeat monitor for the given world
 */
struct HeartbeatMonitor *App_PdmWorld_GetHeartbeatMonitor(const struct PdmWorld *world);

/**
 * Get the RGB LED sequence for the given world
 * @param world The world to get RGB LED sequence for
 * @return The RGB LED sequence for the given world
 */
struct RgbLedSequence *App_PdmWorld_GetRgbLedSequence(const struct PdmWorld *world);

/**
 * Get the low voltage battery for the given world
 * @param world The world to get low voltage battery for
 * @return The low voltage battery for the given world
 */
struct LowVoltageBattery *App_PdmWorld_GetLowVoltageBattery(const struct PdmWorld *world);

/**
 * Get the clock for the given world
 * @param world The world to get clock for
 * @return The clock for the given world
 */
struct Clock *App_PdmWorld_GetClock(const struct PdmWorld *world);

/**
 * Gets Efuse1 for the given world
 * @param world The world to get Efuse1 for
 * @return The Efuse1 for the given world
 */
struct Efuse *App_PdmWorld_GetEfuse1(const struct PdmWorld *world);

/**
 * Gets Efuse2 for the given world
 * @param world The world to get Efuse2 for
 * @return The Efuse2 for the given world
 */
struct Efuse *App_PdmWorld_GetEfuse2(const struct PdmWorld *world);

/**
 * Gets Efuse3 for the given world
 * @param world The world to get Efuse3 for
 * @return The Efuse3 for the given world
 */
struct Efuse *App_PdmWorld_GetEfuse3(const struct PdmWorld *world);

/**
 * Gets Efuse4 for the given world
 * @param world The world to get Efuse4 for
 * @return The Efuse4 for the given world
 */
struct Efuse *App_PdmWorld_GetEfuse4(const struct PdmWorld *world);

/**
 * Rail Monitoring for the given world
 * @param world The world to get Rail Monitoring for
 * @return The Rail Monitoring for the given world
 */
struct RailMonitoring *App_PdmWorld_GetRailMonitoring(const struct PdmWorld *world);
