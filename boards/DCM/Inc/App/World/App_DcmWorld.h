#pragma once

#include "auto_generated/App_CanTx.h"
#include "auto_generated/App_CanRx.h"

struct DcmWorld;

/**
 * Allocate and initialize a world in the application layer
 * @param can_tx_interface: The CAN TX interface to register to the world
 * @return A pointer to the created world, whose ownership is given to the
 * caller
 */
struct DcmWorld *App_DcmWorld_Create(
    struct DCMCanTxInterface *can_tx_interface,
    struct DCMCanRxInterface *can_rx_interface);

/**
 * Get the CAN TX interface for the given world
 * @param world: The world to get CAN TX interface for
 * @return The CAN TX interface for the given world
 */
struct DCMCanTxInterface *App_DcmWorld_GetCanTx(struct DcmWorld *world);

/**
 * Get the CAN RX interface for the given world
 * @param world: The world to get CAN RX interface for
 * @return The CAN RX interface for the given world
 */
struct DcmCanRxInterface *App_SharedWorld_GetCanRx(struct World *world);
