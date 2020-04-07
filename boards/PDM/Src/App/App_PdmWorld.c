#include <stddef.h>
#include <stdlib.h>
#include "App_SharedAssert.h"

#include "App_PdmWorld.h"

struct PdmWorld
{
    struct PDMCanTxInterface *can_tx_interface;
    struct PDMCanRxInterface *can_rx_interface;
    struct VoltageMonitor *   vbat_voltage_monitor;
    struct VoltageMonitor *   _24v_aux_voltage_monitor;
    struct VoltageMonitor *   _24v_acc_voltage_monitor;
};

struct PdmWorld *App_PdmWorld_Create(
    struct PDMCanTxInterface *const can_tx_interface,
    struct PDMCanRxInterface *const can_rx_interface,
    struct VoltageMonitor *const    vbat_voltage_monitor,
    struct VoltageMonitor *const    _24v_aux_voltage_monitor,
    struct VoltageMonitor *const    _24v_acc_voltage_monitor)
{
    struct PdmWorld *world = (struct PdmWorld *)malloc(sizeof(struct PdmWorld));
    shared_assert(world != NULL);

    world->can_tx_interface         = can_tx_interface;
    world->can_rx_interface         = can_rx_interface;
    world->vbat_voltage_monitor     = vbat_voltage_monitor;
    world->_24v_aux_voltage_monitor = _24v_aux_voltage_monitor;
    world->_24v_acc_voltage_monitor = _24v_acc_voltage_monitor;

    return world;
}

void App_PdmWorld_Destroy(struct PdmWorld *world)
{
    shared_assert(world != NULL);
    free(world);
}

struct PDMCanTxInterface *
    App_PdmWorld_GetCanTx(const struct PdmWorld *const world)
{
    shared_assert(world != NULL);
    return world->can_tx_interface;
}

struct PDMCanRxInterface *
    App_PdmWorld_GetCanRx(const struct PdmWorld *const world)
{
    shared_assert(world != NULL);
    return world->can_rx_interface;
}
