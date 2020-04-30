#pragma once

#include "App_CanTx.h"
#include "App_CanRx.h"
#include "App_InRangeCheck.h"
#include "App_SharedHeartbeatMonitor.h"
#include "App_SharedRgbLedSequence.h"

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
    struct InRangeCheck *     vbat_voltage_check,
    struct InRangeCheck *     _24v_aux_voltage_check,
    struct InRangeCheck *     _24v_acc_voltage_check,
    struct InRangeCheck *     aux1_current_check,
    struct InRangeCheck *     aux2_current_check,
    struct InRangeCheck *     left_inverter_check,
    struct InRangeCheck *     right_inverter_check,
    struct InRangeCheck *     energy_meter_current_check,
    struct InRangeCheck *     can_current_check,
    struct InRangeCheck *     air_shutdown_current_check,
    struct HeartbeatMonitor * heartbeat_monitor,
    struct RgbLedSequence *   rgb_led_sequence);

/**
 * Destroy the given PDM world, freeing the memory associated with it
 * @param world The PDM world to destroy
 */
void App_PdmWorld_Destroy(struct PdmWorld *world);

struct PdmCanTxInterface *App_PdmWorld_GetCanTx(const struct PdmWorld *world);
struct PdmCanRxInterface *App_PdmWorld_GetCanRx(const struct PdmWorld *world);
struct InRangeCheck *
    App_PdmWorld_GetVbatInRangeCheck(const struct PdmWorld *world);
struct InRangeCheck *
    App_PdmWorld_Get24vAuxInRangeCheck(const struct PdmWorld *world);
struct InRangeCheck *
    App_PdmWorld_Get24vAccInRangeCheck(const struct PdmWorld *world);
struct InRangeCheck *
    App_PdmWorld_GetAux1CurrentCheck(const struct PdmWorld *world);
struct InRangeCheck *
    App_PdmWorld_GetAux2CurrentCheck(const struct PdmWorld *world);
struct InRangeCheck *
    App_PdmWorld_GetLeftInverterCurrentCheck(const struct PdmWorld *world);
struct InRangeCheck *
    App_PdmWorld_GetRightInverterCurrentCheck(const struct PdmWorld *world);
struct InRangeCheck *
    App_PdmWorld_GetEnergyMeterCurrentCheck(const struct PdmWorld *world);
struct InRangeCheck *
    App_PdmWorld_GetCanCurrentCheck(const struct PdmWorld *world);
struct InRangeCheck *
    App_PdmWorld_GetAirShutdownCurrentCheck(const struct PdmWorld *world);
struct HeartbeatMonitor *
    App_PdmWorld_GetHeartbeatMonitor(const struct PdmWorld *world);
struct RgbLedSequence *
    App_PdmWorld_GetRgbLedSequence(const struct PdmWorld *world);
