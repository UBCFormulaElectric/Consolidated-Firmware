#include <stddef.h>
#include <stdlib.h>
#include <assert.h>

#include "App_PdmWorld.h"

struct PdmWorld
{
    struct PdmCanTxInterface *can_tx_interface;
    struct PdmCanRxInterface *can_rx_interface;
    struct InRangeCheck *     vbat_voltage_check;
    struct InRangeCheck *     _24v_aux_voltage_check;
    struct InRangeCheck *     _24v_acc_voltage_check;
    struct InRangeCheck *     aux1_current_check;
    struct InRangeCheck *     aux2_current_check;
    struct InRangeCheck *     left_inverter_check;
    struct InRangeCheck *     right_inverter_check;
    struct InRangeCheck *     energy_meter_current_check;
    struct InRangeCheck *     can_current_check;
    struct InRangeCheck *     air_shutdown_current_check;
    struct HeartbeatMonitor * heartbeat_monitor;
    struct RgbLedSequence *   rgb_led_sequence;
};

struct PdmWorld *App_PdmWorld_Create(
    struct PdmCanTxInterface *const can_tx_interface,
    struct PdmCanRxInterface *const can_rx_interface,
    struct InRangeCheck *     const vbat_voltage_check,
    struct InRangeCheck *     const _24v_aux_voltage_check,
    struct InRangeCheck *     const _24v_acc_voltage_check,
    struct InRangeCheck *     const aux1_current_check,
    struct InRangeCheck *     const aux2_current_check,
    struct InRangeCheck *     const left_inverter_check,
    struct InRangeCheck *     const right_inverter_check,
    struct InRangeCheck *     const energy_meter_current_check,
    struct InRangeCheck *     const can_current_check,
    struct InRangeCheck *     const air_shutdown_current_check,
    struct HeartbeatMonitor * const heartbeat_monitor,
    struct RgbLedSequence *const    rgb_led_sequence)
{
    struct PdmWorld *world = (struct PdmWorld *)malloc(sizeof(struct PdmWorld));
    assert(world != NULL);

    world->can_tx_interface           = can_tx_interface;
    world->can_rx_interface           = can_rx_interface;
    world->vbat_voltage_check         = vbat_voltage_check;
    world->_24v_aux_voltage_check     = _24v_aux_voltage_check;
    world->_24v_acc_voltage_check     = _24v_acc_voltage_check;
    world->aux1_current_check         = aux1_current_check;
    world->aux2_current_check         = aux2_current_check;
    world->left_inverter_check        = left_inverter_check;
    world->right_inverter_check       = right_inverter_check;
    world->energy_meter_current_check = energy_meter_current_check;
    world->can_current_check          = can_current_check;
    world->air_shutdown_current_check = air_shutdown_current_check;
    world->heartbeat_monitor          = heartbeat_monitor;
    world->rgb_led_sequence         = rgb_led_sequence;

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

struct InRangeCheck *
    App_PdmWorld_GetVbatInRangeCheck(const struct PdmWorld *const world)
{
    return world->vbat_voltage_check;
}

struct InRangeCheck *
    App_PdmWorld_Get24vAuxInRangeCheck(const struct PdmWorld *const world)
{
    return world->_24v_aux_voltage_check;
}

struct InRangeCheck *
    App_PdmWorld_Get24vAccInRangeCheck(const struct PdmWorld *const world)
{
    return world->_24v_acc_voltage_check;
}

struct InRangeCheck *
    App_PdmWorld_GetAux1CurrentCheck(const struct PdmWorld *const world)
{
    return world->aux1_current_check;
}

struct InRangeCheck *
    App_PdmWorld_GetAux2CurrentCheck(const struct PdmWorld *const world)
{
    return world->aux2_current_check;
}

struct InRangeCheck *
    App_PdmWorld_GetLeftInverterCurrentCheck(const struct PdmWorld *const world)
{
    return world->left_inverter_check;
}

struct InRangeCheck *App_PdmWorld_GetRightInverterCurrentCheck(
    const struct PdmWorld *const world)
{
    return world->right_inverter_check;
}

struct InRangeCheck *
    App_PdmWorld_GetEnergyMeterCurrentCheck(const struct PdmWorld *const world)
{
    return world->energy_meter_current_check;
}

struct InRangeCheck *
    App_PdmWorld_GetCanCurrentCheck(const struct PdmWorld *const world)
{
    return world->can_current_check;
}

struct InRangeCheck *
    App_PdmWorld_GetAirShutdownCurrentCheck(const struct PdmWorld *const world)
{
    return world->air_shutdown_current_check;
}

struct HeartbeatMonitor *
    App_PdmWorld_GetHeartbeatMonitor(const struct PdmWorld *const world)
{
    return world->heartbeat_monitor;
}

struct RgbLedSequence *
    App_PdmWorld_GetRgbLedSequence(const struct PdmWorld *const world)
{
    return world->rgb_led_sequence;
}
