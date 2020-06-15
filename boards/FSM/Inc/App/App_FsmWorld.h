#pragma once

#include "App_CanTx.h"
#include "App_CanRx.h"
#include "App_InRangeCheck.h"
#include "App_SharedHeartbeatMonitor.h"
#include "App_SharedRgbLedSequence.h"
#include "App_SharedBinaryStatus.h"
#include "App_Brake.h"
#include "App_SharedSignal.h"
#include "App_SharedClock.h"
#include "App_AcceleratorPedal.h"

struct FsmWorld;

// TODO: add these fxn pointers to the javadoc
// bool (*has_apps_disagreement)(struct FsmWorld *),
// void (*has_apps_disagreement_callback)(struct FsmWorld *),
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
    struct Brake *            brake,
    struct RgbLedSequence *   rgb_led_sequence,
    struct Clock *            clock,

    bool (*has_apps_disagreement)(struct FsmWorld *),
    void (*has_apps_disagreement_callback)(struct FsmWorld *),
    struct AcceleratorPedal *papps,
    bool (*is_papps_alaram_active)(struct FsmWorld *),
    void (*papps_alarm_callback)(struct FsmWorld *),
    struct AcceleratorPedal *sapps,
    bool (*is_sapps_alaram_active)(struct FsmWorld *),
    void (*sapps_alarm_callback)(struct FsmWorld *));

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
 * Get the brake for the given world
 * @param world The world to get the brake for
 * @return The brake for the given world
 */
struct Brake *App_FsmWorld_GetBrake(const struct FsmWorld *world);

/**
 * Get the RGB LED sequence for the given world
 * @param world The world to get RGB LED sequence for
 * @return The RGB LED sequence for the given world
 */
struct RgbLedSequence *
    App_FsmWorld_GetRgbLedSequence(const struct FsmWorld *world);

/**
 * Update the registered signals in the given world
 * @note This function should be called periodically. And since the time
 *       resolution of the signal library is in milliseconds, it would make
 *       sense to call this function at 1kHz.
 * @param world The world to update registered signals for
 * @param current_time_ms The current time, in milliseconds
 */
void App_FsmWorld_UpdateSignals(
    const struct FsmWorld *world,
    uint32_t               current_time_ms);

/**
 * Get the clock for the given world
 * @param world The world to get clock for
 * @return The clock for the given world
 */
struct Clock *App_FsmWorld_GetClock(const struct FsmWorld *world);

/**
 * Get the primary APPS for the given world
 * @param world The world to get primary APPS for
 * @return The primary APPS for the given world
 */
struct AcceleratorPedal *App_FsmWorld_GetPapps(const struct FsmWorld *world);

/**
 * Get the secondary APPS for the given world
 * @param world The world to get secondary APPS for
 * @return The secondary APPS for the given world
 */
struct AcceleratorPedal *App_FsmWorld_GetSapps(const struct FsmWorld *world);
