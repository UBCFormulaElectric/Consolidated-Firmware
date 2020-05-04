#pragma once

#include "App_CanTx.h"
#include "App_CanRx.h"
#include "App_VoltageMonitor.h"
#include "App_SharedHeartbeatMonitor.h"
#include "App_SharedRgbLedSequence.h"

struct PdmWorld;

/**
 * Allocate and initialize a world in the application layer
 * @param can_tx_interface The CAN TX interface to register to the world
 * @param can_rx_interface The CAN RX interface to register to the world
 * @param _24v_aux_voltage_monitor The 24V AUX voltage monitor to register to
 *                                 the world
 * @param _24v_acc_voltage_monitor The 24V ACC voltage monitor to register to
 *                                 the world
 * @param heartbeat_monitor The heartbeat monitor to register to the world
 * @note This function does __not__ take ownership of any of the interfaces
 *       passed into it, which means the every interface must be kept alive for
 *       the lifetime of the created world
 * @return A pointer to the created world, whose ownership is given to the
 *         caller
 */
struct PdmWorld *App_PdmWorld_Create(
    struct PdmCanTxInterface *can_tx_interface,
    struct PdmCanRxInterface *can_rx_interface,
    struct VoltageMonitor *   vbat_voltage_monitor,
    struct VoltageMonitor *   _24v_aux_voltage_monitor,
    struct VoltageMonitor *   _24v_acc_voltage_monitor,
    struct HeartbeatMonitor * heartbeat_monitor,
    struct RgbLedSequence *   rgb_led_sequence);

/**
 * Deallocate the memory used by the given world
 * @param world The world to deallocate
 */
void App_PdmWorld_Destroy(struct PdmWorld *world);

/**
 * Get the CAN TX interface for the given world
 * @param world The world to get CAN TX interface for
 * @return The CAN TX interface for the given world
 */
struct PdmCanTxInterface *App_PdmWorld_GetCanTx(const struct PdmWorld *world);

/**
 * Get the CAN RX interface for the given world
 * @param world The world to get CAN RX interface for
 * @return The CAN RX interface for the given world
 */
struct PdmCanRxInterface *App_PdmWorld_GetCanRx(const struct PdmWorld *world);

/**
 * Get the VBAT voltage monitor for the given world
 * @param world The world to get VBAT voltage monitor for
 * @return The CAN VBAT voltage monitor for the given world
 */
struct VoltageMonitor *
    App_PdmWorld_GetVbatVoltageMonitor(const struct PdmWorld *world);

/**
 * Get the 24V AUX voltage monitor for the given world
 * @param world The world to get 24V AUX voltage monitor for
 * @return The CAN 24V AUX voltage monitor for the given world
 */
struct VoltageMonitor *
    App_PdmWorld_Get24vAuxVoltageMonitor(const struct PdmWorld *world);

/**
 * Get the 24V ACC voltage monitor for the given world
 * @param world The world to get 24V ACC voltage monitor for
 * @return The CAN 24V ACC voltage monitor for the given world
 */
struct VoltageMonitor *
    App_PdmWorld_Get24vAccVoltageMonitor(const struct PdmWorld *world);

/**
 * Get the heartbeat monitor for the given world
 * @param world The world to get heartbeat monitor for
 * @return The heartbeat monitor for the given world
 */
struct HeartbeatMonitor *
    App_PdmWorld_GetHeartbeatMonitor(const struct PdmWorld *world);

/**
 * Get the RGB LED sequence for the given world
 * @param world The world to get RGB LED sequence for
 * @return The RGB LED sequence for the given world
 */
struct RgbLedSequence *
    App_PdmWorld_GetRgbLedSequence(const struct PdmWorld *world);
