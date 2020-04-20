#include <stddef.h>
#include <stdlib.h>
#include <assert.h>

#include "App_PdmWorld.h"

struct PdmWorld
{
    struct PdmCanTxInterface *can_tx_interface;
    struct PdmCanRxInterface *can_rx_interface;
    struct VoltageMonitor *   vbat_voltage_monitor;
    struct VoltageMonitor *   _24v_aux_voltage_monitor;
    struct VoltageMonitor *   _24v_acc_voltage_monitor;
    struct HeartbeatMonitor * heartbeat_monitor;
};

struct PdmWorld *App_PdmWorld_Create(
    struct PdmCanTxInterface *const can_tx_interface,
    struct PdmCanRxInterface *const can_rx_interface,
    struct VoltageMonitor *const    vbat_voltage_monitor,
    struct VoltageMonitor *const    _24v_aux_voltage_monitor,
    struct VoltageMonitor *const    _24v_acc_voltage_monitor,
    struct HeartbeatMonitor *const  heartbeat_monitor)
{
    struct PdmWorld *world = (struct PdmWorld *)malloc(sizeof(struct PdmWorld));
    assert(world != NULL);

    world->can_tx_interface         = can_tx_interface;
    world->can_rx_interface         = can_rx_interface;
    world->vbat_voltage_monitor     = vbat_voltage_monitor;
    world->_24v_aux_voltage_monitor = _24v_aux_voltage_monitor;
    world->_24v_acc_voltage_monitor = _24v_acc_voltage_monitor;
    world->heartbeat_monitor        = heartbeat_monitor;

    return world;
}

void App_PdmWorld_Destroy(struct PdmWorld *const world)
{
    free(world);
}

struct PdmCanTxInterface *
    App_PdmWorld_GetCanTx(const struct PdmWorld *const world)
{
    return world->can_tx_interface;
}

struct PdmCanRxInterface *
    App_PdmWorld_GetCanRx(const struct PdmWorld *const world)
{
    return world->can_rx_interface;
}

struct VoltageMonitor *
    App_PdmWorld_GetVbatVoltageMonitor(const struct PdmWorld *const world)
{
    return world->vbat_voltage_monitor;
}

struct VoltageMonitor *
    App_PdmWorld_Get24vAuxVoltageMonitor(const struct PdmWorld *const world)
{
    return world->_24v_aux_voltage_monitor;
}

struct VoltageMonitor *
    App_PdmWorld_Get24vAccVoltageMonitor(const struct PdmWorld *const world)
{
    return world->_24v_acc_voltage_monitor;
}

struct HeartbeatMonitor *
    App_PdmWorld_GetHeartbeatMonitor(const struct PdmWorld *const world)
{
    return world->heartbeat_monitor;
}
