#pragma once

#include "App_CanTx.h"
#include "App_CanRx.h"
#include "App_SharedHeartbeatMonitor.h"
#include "App_SharedRgbLedSequence.h"
#include "App_SharedErrorTable.h"
#include "App_SharedRgbLed.h"
#include "App_SharedClock.h"
#include "App_GateDrive.h"

struct InvWorld;

/**
 * Allocate and initialize a world in the application layer by registering
 * interfaces to it
 * @note This function does __not__ take ownership of any of the interfaces
 *       passed into it, which means the every interface must be kept alive for
 *       the lifetime of the created DIM world
 * @return A pointer to the created world, whose ownership is given to the
 * caller
 */
struct InvWorld *App_InvWorld_Create(
        struct InvCanTxInterface *const can_tx_interface,
        struct InvCanRxInterface *const can_rx_interface,
        struct HeartbeatMonitor *const  heartbeat_monitor,
        struct RgbLedSequence *const    rgb_led_sequence,
        struct ErrorTable *             error_table,
        struct Clock *const             clock,
        struct GateDrive *const         gate_drive);

/**
 * Deallocate the memory used by the given world
 * @param world The world to deallocate
 */
void App_InvWorld_Destroy(struct InvWorld *world);

/**
 * Get the CAN TX interface for the given world
 * @param world The world to get CAN TX interface for
 * @return The CAN TX interface for the given world
 */
struct InvCanTxInterface *App_InvWorld_GetCanTx(const struct InvWorld *world);

/**
 * Get the CAN RX interface for the given world
 * @param world The world to get CAN RX interface for
 * @return The CAN RX interface for the given world
 */
struct InvCanRxInterface *App_InvWorld_GetCanRx(const struct InvWorld *world);

/**
 * Get the heartbeat monitor for the given world
 * @param world The world to get heartbeat monitor for
 * @return The heartbeat monitor for the given world
 */
struct HeartbeatMonitor *
    App_InvWorld_GetHeartbeatMonitor(const struct InvWorld *world);

/**
 * Get the RGB LED sequence for the given world
 * @param world The world to get RGB LED sequence for
 * @return The RGB LED sequence for the given world
 */
struct RgbLedSequence *
    App_InvWorld_GetRgbLedSequence(const struct InvWorld *world);

/**
 * Get the error table for the given world
 * @param world The world to get error table for
 * @return The error table for the given world
 */
struct ErrorTable *App_InvWorld_GetErrorTable(const struct InvWorld *world);

/**
 * Get the clock for the given world
 * @param world The world to get clock for
 * @return The clock for the given world
 */
struct Clock *App_InvWorld_GetClock(const struct InvWorld *world);

struct GateDrive *App_InvWorld_GetGateDrive(const struct InvWorld *world);
