#pragma once

#include "App_CanTx.h"
#include "App_CanRx.h"

struct DcmWorld;

/**
 * Allocate and initialize a world in the application layer
 * @param can_tx_interface The CAN TX interface to register to the world. Note
 *                         that this function does not take ownership of this
 *                         interface, and so this interface must be kept alive
 *                         for the lifetime of the created DCM world
 * @param can_rx_interface The CAN RX interface to register to the world. Note
 *                         that this function does not take ownership of this
 *                         interface, and so this interface must be kept alive
 *                         for the lifetime of the created DCM world
 * @return A pointer to the created world, whose ownership is given to the
 * caller
 */
struct DcmWorld *App_DcmWorld_Create(
    struct DcmCanTxInterface *can_tx_interface,
    struct DcmCanRxInterface *can_rx_interface);

/**
 * Destroy the given DCM world, freeing the memory associated with it
 * @param world The DCM world to destroy
 */
void App_DcmWorld_Destroy(struct DcmWorld *world);

/**
 * Get the CAN TX interface for the given world
 * @param world: The world to get CAN TX interface for
 * @return The CAN TX interface for the given world
 */
struct DcmCanTxInterface *App_DcmWorld_GetCanTx(const struct DcmWorld *world);

/**
 * Get the CAN RX interface for the given world
 * @param world: The world to get CAN RX interface for
 * @return The CAN RX interface for the given world
 */
struct DcmCanRxInterface *App_DcmWorld_GetCanRx(const struct DcmWorld *world);
