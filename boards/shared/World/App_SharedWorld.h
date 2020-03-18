#pragma once

#include "auto_generated/App_CanTx.h"

struct World;
struct CanTxInterface;

/**
 * Allocate and initialize a world in the application layer
 * @param can_tx_interface: The CAN TX interface to register to the world
 * @return A pointer to the created world, whose ownership is given to the
 * caller
 */
struct World *App_SharedWorld_Create(struct CanTxInterface *can_tx_interface);

/**
 * Get the CAN TX interface for the given world
 * @param world: The world to get CAN TX interface for
 * @return The CAN TX interface for the given world
 */
struct CanTxInterface *App_SharedWorld_GetCanTx(struct World *world);
