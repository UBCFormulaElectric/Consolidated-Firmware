#pragma once

#include "App_CanTx.h"
#include "App_CanRx.h"
#include "App_SharedHeartbeatMonitor.h"

struct DcmWorld;

/**
 * Allocate and initialize a world in the application layer by registering
 * interfaces to it
 * @note This function does __not__ take ownership of any of the interfaces
 *       passed into it, which means the every interface must be kept alive for
 *       the lifetime of the created world
 * @return A pointer to the created world, whose ownership is given to the
 * caller
 */
struct DcmWorld *App_DcmWorld_Create(
    struct DcmCanTxInterface *can_tx_interface,
    struct DcmCanRxInterface *can_rx_interface,
    struct HeartbeatMonitor * heartbeat_monitor);

/**
 * Destroy the given DCM world, freeing the memory associated with it
 * @param world The DCM world to destroy
 */
void App_DcmWorld_Destroy(struct DcmWorld *world);

struct DcmCanTxInterface *App_DcmWorld_GetCanTx(const struct DcmWorld *world);
struct DcmCanRxInterface *App_DcmWorld_GetCanRx(const struct DcmWorld *world);
struct HeartbeatMonitor *
    App_DcmWorld_GetHeartbeatMonitor(const struct DcmWorld *world);
