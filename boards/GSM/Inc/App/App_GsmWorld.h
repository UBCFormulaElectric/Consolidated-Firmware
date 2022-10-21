#pragma once

#include "App_CanTx.h"
#include "App_CanRx.h"
#include "App_SharedClock.h"

struct GsmWorld;

/**
 * Allocate and initialize a world in the application layer by registering
 * interfaces to it
 * @note This function does __not__ take ownership of any of the interfaces
 *       passed into it, which means the every interface must be kept alive for
 *       the lifetime of the created world
 * @return A pointer to the created world, whose ownership is given to the
 * caller
 */
struct GsmWorld *
    App_GsmWorld_Create(struct GsmCanTxInterface *can_tx_interface, struct GsmCanRxInterface *can_rx_interface);

/**
 * Deallocate the memory used by the given world
 * @param world The world to deallocate
 */
void App_GsmWorld_Destroy(struct GsmWorld *world);

/**
 * Get the CAN TX interface for the given world
 * @param world The world to get CAN TX interface for
 * @return The CAN TX interface for the given world
 */
struct GsmCanTxInterface *App_GsmWorld_GetCanTx(const struct GsmWorld *world);

/**
 * Get the CAN RX interface for the given world
 * @param world The world to get CAN RX interface for
 * @return The CAN RX interface for the given world
 */
struct GsmCanRxInterface *App_GsmWorld_GetCanRx(const struct GsmWorld *world);
