#pragma once

#include "auto_generated/App_CanTx.h"
#include "auto_generated/App_CanRx.h"
#include "App_VoltageMonitor.h"

struct PdmWorld;

/**
 * Allocate and initialize a world in the application layer
 * @param can_tx_interface The CAN TX interface to register to the world. Note
 *                         that this function does not take ownership of this
 *                         interface, and so this interface must be kept alive
 *                         for the lifetime of the created PDM world
 * @param can_rx_interface The CAN RX interface to register to the world. Note
 *                         that this function does not take ownership of this
 *                         interface, and so this interface must be kept alive
 *                         for the lifetime of the created PDM world
 * @param TODO: voltage monitors
 * @return A pointer to the created world, whose ownership is given to the
 * caller
 */
struct PdmWorld *App_PdmWorld_Create(
    struct PDMCanTxInterface *can_tx_interface,
    struct PDMCanRxInterface *can_rx_interface,
    struct VoltageMonitor *   vbat_volage_monitor,
    struct VoltageMonitor *   _24v_aux_voltage_monitor,
    struct VoltageMonitor *   _24v_acc_voltage_monitor);

/**
 * Destroy the given PDM world, freeing the memory associated with it
 * @param world The PDM world to destroy
 */
void App_PdmWorld_Destroy(struct PdmWorld *world);

/**
 * Get the CAN TX interface for the given world
 * @param world: The world to get CAN TX interface for
 * @return The CAN TX interface for the given world
 */
struct PDMCanTxInterface *App_PdmWorld_GetCanTx(const struct PdmWorld *world);

/**
 * Get the CAN RX interface for the given world
 * @param world: The world to get CAN RX interface for
 * @return The CAN RX interface for the given world
 */
struct PDMCanRxInterface *App_PdmWorld_GetCanRx(const struct PdmWorld *world);
