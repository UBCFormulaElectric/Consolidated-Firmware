#pragma once

#include "App_CanTx.h"
#include "App_CanRx.h"
#include "App_SharedHeartbeatMonitor.h"
#include "App_FlowMeter.h"

struct FsmWorld;

/**
 * Allocate and initialize a world in the application layer
 * @param can_tx_interface The CAN TX interface to register to the world
 * @param can_rx_interface The CAN RX interface to register to the world
 * @param heartbeat_monitor The heartbeat monitor to register to the world
 * @param primary_flow_meter The primary flow meter to register to the world
 * @param secondary_flow_meter The secondary flow meter to register to the world
 * @note This function does __not__ take ownership of any of the interfaces
 *       passed into it, which means the every interface must be kept alive for
 *       the lifetime of the created world
 * @return A pointer to the created world, whose ownership is given to the
 * caller
 */
struct FsmWorld *App_FsmWorld_Create(
    struct FsmCanTxInterface *can_tx_interface,
    struct FsmCanRxInterface *can_rx_interface,
    struct HeartbeatMonitor * heartbeat_monitor,
    struct FlowMeter *        primary_flow_meter,
    struct FlowMeter *        secondary_flow_meter);

/**
 * Destroy the given FSM world, freeing the memory associated with it
 * @param world The FSM world to destroy
 */
void App_FsmWorld_Destroy(struct FsmWorld *world);

struct FsmCanTxInterface *App_FsmWorld_GetCanTx(const struct FsmWorld *world);
struct FsmCanRxInterface *App_FsmWorld_GetCanRx(const struct FsmWorld *world);
struct HeartbeatMonitor *
    App_FsmWorld_GetHeartbeatMonitor(const struct FsmWorld *world);
struct FlowMeter *
    App_FsmWorld_GetPrimaryFlowMeter(const struct FsmWorld *world);
struct FlowMeter *
    App_FsmWorld_GetSecondaryFlowMeter(const struct FsmWorld *world);
