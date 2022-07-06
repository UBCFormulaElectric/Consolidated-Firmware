#pragma once

#include "App_CanTx.h"
#include "App_CanRx.h"
#include "App_Imd.h"
#include "App_SharedHeartbeatMonitor.h"
#include "App_SharedRgbLedSequence.h"
#include "App_Accumulator.h"
#include "App_TractiveSystem.h"
#include "App_SharedErrorTable.h"
#include "App_SharedClock.h"

struct BmsWorld;

/**
 * Allocate and initialize a world in the application layer by registering
 * interfaces to it
 * @note This function does __not__ take ownership of any of the interfaces
 *       passed into it, which means the every interface must be kept alive for
 *       the lifetime of the created world
 * @return A pointer to the created world, whose ownership is given to the
 * caller
 */
struct BmsWorld *App_BmsWorld_Create(
    struct BmsCanTxInterface *can_tx_interface,
    struct BmsCanRxInterface *can_rx_interface,
    struct Imd *              imd,
    struct HeartbeatMonitor * heartbeat_monitor,
    struct RgbLedSequence *   rgb_led_sequence,
    struct Accumulator *      accumulator,
    struct TractiveSystem *   tractive_system,
    struct ErrorTable *       error_table,
    struct Clock *            clock);

/**
 * Deallocate the memory used by the given world
 * @param world The world to deallocate
 */
void App_BmsWorld_Destroy(struct BmsWorld *world);

/**
 * Get the CAN TX interface for the given world
 * @param world The world to get CAN TX interface for
 * @return The CAN TX interface for the given world
 */
struct BmsCanTxInterface *App_BmsWorld_GetCanTx(const struct BmsWorld *world);

/**
 * Get the CAN RX interface for the given world
 * @param world The world to get CAN RX interface for
 * @return The CAN RX interface for the given world
 */
struct BmsCanRxInterface *App_BmsWorld_GetCanRx(const struct BmsWorld *world);

/**
 * Get the IMD for the given world
 * @param world The world to get IMD for
 * @return The IMD for the given world
 */
struct Imd *App_BmsWorld_GetImd(const struct BmsWorld *world);

/**
 * Get the heartbeat monitor for the given world
 * @param world The world to get heartbeat monitor for
 * @return The heartbeat monitor for the given world
 */
struct HeartbeatMonitor *App_BmsWorld_GetHeartbeatMonitor(const struct BmsWorld *world);

/**
 * Get the RGB LED sequence for the given world
 * @param world The world to get RGB LED sequence for
 * @return The RGB LED sequence for the given world
 */
struct RgbLedSequence *App_BmsWorld_GetRgbLedSequence(const struct BmsWorld *world);

/**
 * Get the accumulator for the given world
 * @param world The world to get the accumulator for
 * @return The accumulator for the given world
 */
struct Accumulator *App_BmsWorld_GetAccumulator(const struct BmsWorld *world);

/**
 * Get the tractive system for the given world
 * @param world The world to get the tractive system for
 * @return The tractive system for the given world
 */
struct TractiveSystem *App_BmsWorld_GetTractiveSystem(const struct BmsWorld *world);

/**
 * Get the error table for the given world
 * @param world The world to get the error table from
 * @return The error table for the given world
 */
struct ErrorTable *App_BmsWorld_GetErrorTable(const struct BmsWorld *world);

/**
 * Get the clock for the given world
 * @param world The world to get clock for
 * @return The clock for the given world
 */
struct Clock *App_BmsWorld_GetClock(const struct BmsWorld *world);
