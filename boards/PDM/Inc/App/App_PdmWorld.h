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
 * Destroy the given PDM world, freeing the memory associated with it
 * @param world The PDM world to destroy
 */
void App_PdmWorld_Destroy(struct PdmWorld *world);

struct PdmCanTxInterface *App_PdmWorld_GetCanTx(const struct PdmWorld *world);
struct PdmCanRxInterface *App_PdmWorld_GetCanRx(const struct PdmWorld *world);
struct VoltageMonitor *
    App_PdmWorld_GetVbatVoltageMonitor(const struct PdmWorld *world);
struct VoltageMonitor *
    App_PdmWorld_Get24vAuxVoltageMonitor(const struct PdmWorld *world);
struct VoltageMonitor *
    App_PdmWorld_Get24vAccVoltageMonitor(const struct PdmWorld *world);
struct HeartbeatMonitor *
    App_PdmWorld_GetHeartbeatMonitor(const struct PdmWorld *world);
struct RgbLedSequence *
    App_PdmWorld_GetRgbLedSequence(const struct PdmWorld *world);
