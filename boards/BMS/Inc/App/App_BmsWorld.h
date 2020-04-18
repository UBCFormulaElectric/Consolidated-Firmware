#pragma once

#include "App_CanTx.h"
#include "App_CanRx.h"
#include "App_Imd.h"
#include "App_SharedHeartbeatMonitor.h"

struct BmsWorld;

/**
 * Allocate and initialize a world in the application layer
 * @param can_tx_interface The CAN TX interface to register to the world
 * @param can_rx_interface The CAN RX interface to register to the world
 * @param imd The IMD to register to the world
 * @param heartbeat_monitor The heartbeat monitor to register to the world
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
    struct HeartbeatMonitor * heartbeat_monitor);

/**
 * Destroy the given BMS world, freeing the memory associated with it
 * @param world The BMS world to destroy
 */
void App_BmsWorld_Destroy(struct BmsWorld *world);

struct BmsCanTxInterface *App_BmsWorld_GetCanTx(const struct BmsWorld *world);
struct BmsCanRxInterface *App_BmsWorld_GetCanRx(const struct BmsWorld *world);
struct Imd *              App_BmsWorld_GetImd(const struct BmsWorld *world);
struct HeartbeatMonitor *
    App_BmsWorld_GetHeartbeatMonitor(const struct BmsWorld *world);
