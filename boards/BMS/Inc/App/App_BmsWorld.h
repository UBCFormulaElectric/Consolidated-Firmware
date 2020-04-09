#pragma once

#include "App_CanTx.h"
#include "App_CanRx.h"
#include "App_Imd.h"

struct BmsWorld;

/**
 * Allocate and initialize a world in the application layer
 * @param can_tx_interface The CAN TX interface to register to the world. Note
 *                         that this function does not take ownership of this
 *                         interface, and so this interface must be kept alive
 *                         for the lifetime of the created BMS world
 * @param can_rx_interface The CAN RX interface to register to the world. Note
 *                         that this function does not take ownership of this
 *                         interface, and so this interface must be kept alive
 *                         for the lifetime of the created BMS world
 * @param imd A wrapper around the IMD object
 * @return A pointer to the created world, whose ownership is given to the
 * caller
 */
struct BmsWorld *App_BmsWorld_Create(
    struct BMSCanTxInterface *can_tx_interface,
    struct BMSCanRxInterface *can_rx_interface,
    struct Imd *              imd);

/**
 * Destroy the given BMS world, freeing the memory associated with it
 * @param world The BMS world to destroy
 */
void App_BmsWorld_Destroy(struct BmsWorld *world);

/**
 * Get the CAN TX interface for the given world
 * @param world: The world to get CAN TX interface for
 * @return The CAN TX interface for the given world
 */
struct BMSCanTxInterface *App_BmsWorld_GetCanTx(const struct BmsWorld *world);

/**
 * Get the CAN RX interface for the given world
 * @param world: The world to get CAN RX interface for
 * @return The CAN RX interface for the given world
 */
struct BMSCanRxInterface *App_BmsWorld_GetCanRx(const struct BmsWorld *world);

/**
 * Get the IMD from the given world
 * @param world The world to get the IMD from
 * @return The IMD from the world
 */
struct Imd *App_BmsWorld_GetImd(const struct BmsWorld *world);
