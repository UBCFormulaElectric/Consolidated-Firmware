#include <stddef.h>
#include <stdlib.h>
#include <assert.h>

#include "App_PdmWorld.h"

struct PdmWorld
{
    struct PdmCanTxInterface *can_tx_interface;
    struct PdmCanRxInterface *can_rx_interface;
    struct InRangeCheck *     vbat_voltage_in_range_check;
    struct InRangeCheck *     _24v_aux_voltage_in_range_check;
    struct InRangeCheck *     _24v_acc_voltage_in_range_check;
    struct InRangeCheck *     aux1_current_in_range_check;
    struct InRangeCheck *     aux2_current_in_range_check;
    struct InRangeCheck *     left_inverter_current_in_range_check;
    struct InRangeCheck *     right_inverter_current_in_range_check;
    struct InRangeCheck *     energy_meter_current_in_range_check;
    struct InRangeCheck *     can_current_in_range_check;
    struct InRangeCheck *     air_shutdown_current_in_range_check;
    struct HeartbeatMonitor * heartbeat_monitor;
    struct RgbLedSequence *   rgb_led_sequence;
    struct LowVoltageBattery *low_voltage_battery;
    struct Clock *            clock;
};

struct PdmWorld *App_PdmWorld_Create(
    struct PdmCanTxInterface *const can_tx_interface,
    struct PdmCanRxInterface *const can_rx_interface,
    struct InRangeCheck *const      vbat_voltage_in_range_check,
    struct InRangeCheck *const      _24v_aux_voltage_in_range_check,
    struct InRangeCheck *const      _24v_acc_voltage_in_range_check,
    struct InRangeCheck *const      aux1_current_in_range_check,
    struct InRangeCheck *const      aux2_current_in_range_check,
    struct InRangeCheck *const      left_inverter_current_in_range_check,
    struct InRangeCheck *const      right_inverter_current_in_range_check,
    struct InRangeCheck *const      energy_meter_current_in_range_check,
    struct InRangeCheck *const      can_current_in_range_check,
    struct InRangeCheck *const      air_shutdown_current_in_range_check,
    struct HeartbeatMonitor *const  heartbeat_monitor,
    struct RgbLedSequence *const    rgb_led_sequence,
    struct LowVoltageBattery *const low_voltage_battery,
    struct Clock *const             clock)
{
    struct PdmWorld *world = (struct PdmWorld *)malloc(sizeof(struct PdmWorld));
    assert(world != NULL);

    world->can_tx_interface                      = can_tx_interface;
    world->can_rx_interface                      = can_rx_interface;
    world->vbat_voltage_in_range_check           = vbat_voltage_in_range_check;
    world->_24v_aux_voltage_in_range_check       = _24v_aux_voltage_in_range_check;
    world->_24v_acc_voltage_in_range_check       = _24v_acc_voltage_in_range_check;
    world->aux1_current_in_range_check           = aux1_current_in_range_check;
    world->aux2_current_in_range_check           = aux2_current_in_range_check;
    world->left_inverter_current_in_range_check  = left_inverter_current_in_range_check;
    world->right_inverter_current_in_range_check = right_inverter_current_in_range_check;
    world->energy_meter_current_in_range_check   = energy_meter_current_in_range_check;
    world->can_current_in_range_check            = can_current_in_range_check;
    world->air_shutdown_current_in_range_check   = air_shutdown_current_in_range_check;
    world->heartbeat_monitor                     = heartbeat_monitor;
    world->rgb_led_sequence                      = rgb_led_sequence;
    world->low_voltage_battery                   = low_voltage_battery;
    world->clock                                 = clock;

    return world;
}

void App_PdmWorld_Destroy(struct PdmWorld *const world)
{
    free(world);
}

struct PdmCanTxInterface *App_PdmWorld_GetCanTx(const struct PdmWorld *const world)
{
    return world->can_tx_interface;
}

struct PdmCanRxInterface *App_PdmWorld_GetCanRx(const struct PdmWorld *const world)
{
    return world->can_rx_interface;
}

struct InRangeCheck *App_PdmWorld_GetVbatVoltageInRangeCheck(const struct PdmWorld *const world)
{
    return world->vbat_voltage_in_range_check;
}

struct InRangeCheck *App_PdmWorld_Get24vAuxVoltageInRangeCheck(const struct PdmWorld *const world)
{
    return world->_24v_aux_voltage_in_range_check;
}

struct InRangeCheck *App_PdmWorld_Get24vAccVoltageInRangeCheck(const struct PdmWorld *const world)
{
    return world->_24v_acc_voltage_in_range_check;
}

struct InRangeCheck *App_PdmWorld_GetAux1CurrentInRangeCheck(const struct PdmWorld *const world)
{
    return world->aux1_current_in_range_check;
}

struct InRangeCheck *App_PdmWorld_GetAux2CurrentInRangeCheck(const struct PdmWorld *const world)
{
    return world->aux2_current_in_range_check;
}

struct InRangeCheck *App_PdmWorld_GetLeftInverterCurrentInRangeCheck(const struct PdmWorld *const world)
{
    return world->left_inverter_current_in_range_check;
}

struct InRangeCheck *App_PdmWorld_GetRightInverterCurrentInRangeCheck(const struct PdmWorld *const world)
{
    return world->right_inverter_current_in_range_check;
}

struct InRangeCheck *App_PdmWorld_GetEnergyMeterCurrentInRangeCheck(const struct PdmWorld *const world)
{
    return world->energy_meter_current_in_range_check;
}

struct InRangeCheck *App_PdmWorld_GetCanCurrentInRangeCheck(const struct PdmWorld *const world)
{
    return world->can_current_in_range_check;
}

struct InRangeCheck *App_PdmWorld_GetAirShutdownCurrentInRangeCheck(const struct PdmWorld *const world)
{
    return world->air_shutdown_current_in_range_check;
}

struct HeartbeatMonitor *App_PdmWorld_GetHeartbeatMonitor(const struct PdmWorld *const world)
{
    return world->heartbeat_monitor;
}

struct RgbLedSequence *App_PdmWorld_GetRgbLedSequence(const struct PdmWorld *const world)
{
    return world->rgb_led_sequence;
}

struct LowVoltageBattery *App_PdmWorld_GetLowVoltageBattery(const struct PdmWorld *const world)
{
    return world->low_voltage_battery;
}

struct Clock *App_PdmWorld_GetClock(const struct PdmWorld *const world)
{
    return world->clock;
}
