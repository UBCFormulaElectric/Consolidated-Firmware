#pragma once

#include "App_CanTx.h"
#include "App_CanRx.h"
#include "App_InRangeCheck.h"
#include "App_SharedHeartbeatMonitor.h"
#include "App_SharedRgbLedSequence.h"
#include "App_LowVoltageBattery.h"
#include "App_SharedClock.h"

struct PdmWorld;

/**
 * Allocate and initialize a world in the application layer by registering
 * interfaces to it
 * @note This function does __not__ take ownership of any of the interfaces
 *       passed into it, which means the every interface must be kept alive for
 *       the lifetime of the created world
 * @return A pointer to the created world, whose ownership is given to the
 *         caller
 */
struct PdmWorld *App_PdmWorld_Create(
    struct PdmCanTxInterface *can_tx_interface,
    struct PdmCanRxInterface *can_rx_interface,
    struct InRangeCheck *     vbat_voltage_in_range_check,
    struct InRangeCheck *     _24v_aux_voltage_in_range_check,
    struct InRangeCheck *     _24v_acc_voltage_in_range_check,
    struct InRangeCheck *     aux1_current_in_range_check,
    struct InRangeCheck *     aux2_current_in_range_check,
    struct InRangeCheck *     left_inverter_current_in_range_check,
    struct InRangeCheck *     right_inverter_current_in_range_check,
    struct InRangeCheck *     energy_meter_current_in_range_check,
    struct InRangeCheck *     can_current_in_range_check,
    struct InRangeCheck *     air_shutdown_current_in_range_check,
    struct HeartbeatMonitor * heartbeat_monitor,
    struct RgbLedSequence *   rgb_led_sequence,
    struct LowVoltageBattery *low_voltage_battery,
    struct Clock *            clock);

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
 * Get the VBAT voltage in-range check for the given world
 * @param world The world to get VBAT voltage in-range check for
 * @return The VBAT voltage in-range check for the given world
 */
struct InRangeCheck *App_PdmWorld_GetVbatVoltageInRangeCheck(const struct PdmWorld *world);

/**
 * Get the 24V AUX voltage in-range check for the given world
 * @param world The world to get 24V AUX voltage in-range check for
 * @return The 24V AUX voltage in-range check for the given world
 */
struct InRangeCheck *App_PdmWorld_Get24vAuxVoltageInRangeCheck(const struct PdmWorld *world);

/**
 * Get the 24V ACC voltage in-range check for the given world
 * @param world The world to get 24V ACC voltage in-range check for
 * @return The 24V ACC voltage in-range check for the given world
 */
struct InRangeCheck *App_PdmWorld_Get24vAccVoltageInRangeCheck(const struct PdmWorld *world);

/**
 * Get the auxiliary 1 current in-range check for the given world
 * @param world The world to get auxiliary 1 current in-range check for
 * @return The auxiliary 1 current in-range check for the given world
 */
struct InRangeCheck *App_PdmWorld_GetAux1CurrentInRangeCheck(const struct PdmWorld *world);

/**
 * Get the auxiliary 2 current in-range check for the given world
 * @param world The world to get auxiliary 2 current in-range check for
 * @return The auxiliary 2 current in-range check for the given world
 */
struct InRangeCheck *App_PdmWorld_GetAux2CurrentInRangeCheck(const struct PdmWorld *world);

/**
 * Get the left inverter current in-range check for the given world
 * @param world The world to get left inverter current in-range check for
 * @return The left inverter current in-range check for the given world
 */
struct InRangeCheck *App_PdmWorld_GetLeftInverterCurrentInRangeCheck(const struct PdmWorld *world);
/**
 * Get the right inverter current in-range check for the given world
 * @param world The world to get right inverter current in-range check for
 * @return The right inverter current in-range check for the given world
 */
struct InRangeCheck *App_PdmWorld_GetRightInverterCurrentInRangeCheck(const struct PdmWorld *world);
/**
 * Get the energy meter current in-range check for the given world
 * @param world The world to get energy meter current in-range check for
 * @return The energy meter current in-range check for the given world
 */
struct InRangeCheck *App_PdmWorld_GetEnergyMeterCurrentInRangeCheck(const struct PdmWorld *world);
/**
 * Get the CAN current in-range check for the given world
 * @param world The world to get CAN current in-range check for
 * @return The CAN current in-range check for the given world
 */
struct InRangeCheck *App_PdmWorld_GetCanCurrentInRangeCheck(const struct PdmWorld *world);

/**
 * Get the AIR shutdown current in-range check for the given world
 * @param world The world to get AIR shutdown current in-range check for
 * @return The AIR shutdown current in-range check for the given world
 */
struct InRangeCheck *App_PdmWorld_GetAirShutdownCurrentInRangeCheck(const struct PdmWorld *world);

/**
 * Get the heartbeat monitor for the given world
 * @param world The world to get heartbeat monitor for
 * @return The heartbeat monitor for the given world
 */
struct HeartbeatMonitor *App_PdmWorld_GetHeartbeatMonitor(const struct PdmWorld *world);

/**
 * Get the RGB LED sequence for the given world
 * @param world The world to get RGB LED sequence for
 * @return The RGB LED sequence for the given world
 */
struct RgbLedSequence *App_PdmWorld_GetRgbLedSequence(const struct PdmWorld *world);

/**
 * Get the low voltage battery for the given world
 * @param world The world to get low voltage battery for
 * @return The low voltage battery for the given world
 */
struct LowVoltageBattery *App_PdmWorld_GetLowVoltageBattery(const struct PdmWorld *world);

/**
 * Get the clock for the given world
 * @param world The world to get clock for
 * @return The clock for the given world
 */
struct Clock *App_PdmWorld_GetClock(const struct PdmWorld *world);
