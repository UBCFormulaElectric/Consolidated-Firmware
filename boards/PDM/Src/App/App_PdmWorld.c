#include <stddef.h>
#include <stdlib.h>
#include <assert.h>

#include "App_PdmWorld.h"

struct PdmWorld
{
    struct PdmCanTxInterface *can_tx;
    struct PdmCanRxInterface *can_rx;
    struct HeartbeatMonitor * heartbeat_monitor;
    struct RgbLedSequence    *rgb_led_sequence;
    struct LowVoltageBattery *low_voltage_battery;
    struct Efuse             *efuse1;
    struct Efuse             *efuse2;
    struct Efuse             *efuse3;
    struct Efuse             *efuse4;
    bool                     *efuse1_current_range_check;
    bool                     *efuse2_current_range_check;
    bool                     *efuse3_current_range_check;
    bool                     *efuse4_current_range_check;
    struct RailMonitoring    *rail_monitor;
    bool                     *rail_range_check;
    struct Clock *            clock;
};

struct PdmWorld *App_PdmWorld_Create(
    struct PdmCanTxInterface *const can_tx_interface,
    struct PdmCanRxInterface *const can_rx_interface,
    struct HeartbeatMonitor *const  heartbeat_monitor,
    struct RgbLedSequence *const    rgb_led_sequence,
    struct LowVoltageBattery *const low_voltage_battery,
    struct Efuse             *const efuse1,
    struct Efuse             *const efuse2,
    struct Efuse             *const efuse3,
    struct Efuse             *const efuse4,
    bool                            efuse1_current_range_check,
    bool                            efuse2_current_range_check,
    bool                            efuse3_current_range_check,
    bool                            efuse4_current_range_check,
    struct RailMonitoring    *const rail_monitor,
    bool                     *rail_range_check;
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
    world->efuse                           = efuse;
    world->rail_monitor                          = rail_monitor;
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

struct Efuse *App_PdmWorld_GetEfuse1(const struct PdmWorld *const world)
{
    return world->efuse1;
}

struct Efuse *App_PdmWorld_GetEfuse2(const struct PdmWorld *const world)
{
    return world->efuse2;
}

struct Efuse *App_PdmWorld_GetEfuse3(const struct PdmWorld *const world)
{
    return world->efuse3;
}

struct Efuse *App_PdmWorld_GetEfuse4(const struct PdmWorld *const world)
{
    return world->efuse4;
}

struct RailMonitoring *App_PdmWorld_GetRailMonitoring(const struct PdmWorld *const world)
{
    return world->rail_monitor;
}

struct Clock *App_PdmWorld_GetClock(const struct PdmWorld *const world)
{
    return world->clock;
}
