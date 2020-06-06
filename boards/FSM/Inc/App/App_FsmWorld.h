#pragma once

#include "App_CanTx.h"
#include "App_CanRx.h"
#include "App_InRangeCheck.h"
#include "App_SharedHeartbeatMonitor.h"
#include "App_SharedRgbLedSequence.h"
#include "App_SharedBinaryStatus.h"

struct FsmWorld;

/**
 * Allocate and initialize a world in the application layer by registering
 * interfaces to it
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
    struct InRangeCheck *     primary_flow_rate_in_range_check,
    struct InRangeCheck *     secondary_flow_rate_in_range_check,
    struct InRangeCheck *     left_wheel_speed_in_range_check,
    struct InRangeCheck *     right_wheel_speed_in_range_check,
    struct InRangeCheck *     steering_angle_in_range_check,
    struct InRangeCheck *     brake_pressure_in_range_check,
    struct BinaryStatus *     brake_actuation_status,
    struct BinaryStatus *     brake_oc_sc_status,
    struct RgbLedSequence *   rgb_led_sequence);

/**
 * Deallocate the memory used by the given world
 * @param world The world to deallocate
 */
void App_FsmWorld_Destroy(struct FsmWorld *world);

/**
 * Get the CAN TX interface for the given world
 * @param world The world to get CAN TX interface for
 * @return The CAN TX interface for the given world
 */
struct FsmCanTxInterface *App_FsmWorld_GetCanTx(const struct FsmWorld *world);

/**
 * Get the CAN RX interface for the given world
 * @param world The world to get CAN RX interface for
 * @return The CAN RX interface for the given world
 */
struct FsmCanRxInterface *App_FsmWorld_GetCanRx(const struct FsmWorld *world);

/**
 * Get the heartbeat monitor for the given world
 * @param world The world to get heartbeat monitor for
 * @return The heartbeat monitor for the given world
 */
struct HeartbeatMonitor *
    App_FsmWorld_GetHeartbeatMonitor(const struct FsmWorld *world);

/**
 * Get the primary flow rate in-range check for the given world
 * @param world The world to get primary flow rate in-range check for
 * @return The primary flow rate in-range check for the given world
 */
struct InRangeCheck *
    App_FsmWorld_GetPrimaryFlowRateInRangeCheck(const struct FsmWorld *world);

/**
 * Get the secondary flow rate in-range check for the given world
 * @param world The world to get secondary flow rate in-range check for
 * @return The secondary flow rate in-range check for the given world
 */
struct InRangeCheck *
    App_FsmWorld_GetSecondaryFlowRateInRangeCheck(const struct FsmWorld *world);

/**
 * Get the left wheel speed in-range check for the given world
 * @param world The world to get the left wheel speed in-range check for
 * @return The left wheel speed in-range check for the given world
 */
struct InRangeCheck *
    App_FsmWorld_GetLeftWheelSpeedInRangeCheck(const struct FsmWorld *world);

/**
 * Get the right wheel speed in-range check for the given world
 * @param world The world to get the right wheel speed in-range check for
 * @return The right wheel speed in-range check for the given world
 */
struct InRangeCheck *
    App_FsmWorld_GetRightWheelSpeedInRangeCheck(const struct FsmWorld *world);

/**
 * Get the steering angle in-range check for the given world
 * @param world The world to get the steering angle in-range check for
 * @return The steering angle in-range check for the given world
 */
struct InRangeCheck *
    App_FsmWorld_GetSteeringAngleInRangeCheck(const struct FsmWorld *world);

/**
 * Get the brake pressure in-range check for the given world
 * @param world The world to get the brake pressure in-range check for
 * @return The brake pressure in-range check for the given world
 */
struct InRangeCheck *
    App_FsmWorld_GetBrakePressureInRangeCheck(const struct FsmWorld *world);

/**
 * Get the brake actuation status for the given world
 * @param world The world to get the brake actuation status for
 * @return The brake actuation status for the given world
 */
struct BinaryStatus *
    App_FsmWorld_GetBrakeActuationStatus(const struct FsmWorld *world);

/**
 * Get the brake open/short circuit status for the given world
 * @param world The world to get the open/short circuit status for
 * @return The open/short circuit status for the given world
 */
struct BinaryStatus *App_FsmWorld_GetBrakeOpenOrShortCircuitedStatus(
    const struct FsmWorld *world);

/**
 * Get the RGB LED sequence for the given world
 * @param world The world to get RGB LED sequence for
 * @return The RGB LED sequence for the given world
 */
struct RgbLedSequence *
    App_FsmWorld_GetRgbLedSequence(const struct FsmWorld *world);
