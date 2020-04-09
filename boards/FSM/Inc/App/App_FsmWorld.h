#pragma once

#include "App_CanTx.h"
#include "App_CanRx.h"

struct FsmWorld;

/**
 * Allocate and initialize a world in the application layer
 * @param can_tx_interface The CAN TX interface to register to the world. Note
 *                         that this function does not take ownership of this
 *                         interface, and so this interface must be kept alive
 *                         for the lifetime of the created FSM world
 * @param can_rx_interface The CAN RX interface to register to the world. Note
 *                         that this function does not take ownership of this
 *                         interface, and so this interface must be kept alive
 *                         for the lifetime of the created FSM world
 * @return A pointer to the created world, whose ownership is given to the
 * caller
 */
struct FsmWorld *App_FsmWorld_Create(
    struct FSMCanTxInterface *can_tx_interface,
    struct FSMCanRxInterface *can_rx_interface);

/**
 * Destroy the given FSM world, freeing the memory associated with it
 * @param world The FSM world to destroy
 */
void App_FsmWorld_Destroy(struct FsmWorld *world);

/**
 * Get the CAN TX interface for the given world
 * @param world: The world to get CAN TX interface for
 * @return The CAN TX interface for the given world
 */
struct FSMCanTxInterface *App_FsmWorld_GetCanTx(const struct FsmWorld *world);

/**
 * Get the CAN RX interface for the given world
 * @param world: The world to get CAN RX interface for
 * @return The CAN RX interface for the given world
 */
struct FSMCanRxInterface *App_FsmWorld_GetCanRx(const struct FsmWorld *world);
