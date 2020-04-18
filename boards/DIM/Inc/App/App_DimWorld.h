#pragma once

#include "App_CanTx.h"
#include "App_CanRx.h"
#include "App_SevenSegDisplays.h"
#include "App_SharedHeartbeatMonitor.h"

struct DimWorld;

/**
 * Allocate and initialize a world in the application layer
 * @param can_tx_interface The CAN TX interface to register to the world
 * @param can_rx_interface The CAN RX interface to register to the world
 * @param seven_seg_displays The group of three 7-segment displays to register
 *                           to the world
 * @note This function does __not__ take ownership of any of the interfaces
 *       passed into it, which means the every interface must be kept alive for
 *       the lifetime of the created DIM world
 * @return A pointer to the created world, whose ownership is given to the
 * caller
 */
struct DimWorld *App_DimWorld_Create(
    struct DimCanTxInterface *can_tx_interface,
    struct DimCanRxInterface *can_rx_interface,
    struct SevenSegDisplays * seven_seg_displays,
    struct HeartbeatMonitor * heartbeat_monitor);

/**
 * Destroy the given DIM world, freeing the memory associated with it
 * @param world The DIM world to destroy
 */
void App_DimWorld_Destroy(struct DimWorld *world);

/**
 * Get the CAN TX interface for the given world
 * @param world: The world to get CAN TX interface for
 * @return The CAN TX interface for the given world
 */
struct DimCanTxInterface *App_DimWorld_GetCanTx(const struct DimWorld *world);

/**
 * Get the CAN RX interface for the given world
 * @param world: The world to get CAN RX interface for
 * @return The CAN RX interface for the given world
 */
struct DimCanRxInterface *App_DimWorld_GetCanRx(const struct DimWorld *world);

/**
 * Get the group of three 7-segment displays for the given world
 * @param world: The world to get group of three 7-segment displays for
 * @return The group of three 7-segment displays for the given world
 */
struct SevenSegDisplays *
    App_DimWorld_GetSevenSegDisplays(const struct DimWorld *world);

struct HeartbeatMonitor *
    App_DimWorld_GetHeartbeatMonitor(const struct DimWorld *world);
