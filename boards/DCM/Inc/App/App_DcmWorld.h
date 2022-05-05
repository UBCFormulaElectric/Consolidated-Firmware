#pragma once

#include "App_CanTx.h"
#include "App_CanRx.h"
#include "App_SharedHeartbeatMonitor.h"
#include "App_SharedRgbLedSequence.h"
#include "App_BrakeLight.h"
#include "App_Buzzer.h"
#include "App_BuzzerSignals.h"
#include "App_Imu.h"
#include "App_SharedErrorTable.h"
#include "App_SharedClock.h"
#include "App_SharedWaitSignal.h"
#include "App_InverterSwitches.h"

struct DcmWorld;

/**
 * Allocate and initialize a world in the application layer by registering
 * interfaces to it
 * @note This function does __not__ take ownership of any of the interfaces
 *       passed into it, which means the every interface must be kept alive for
 *       the lifetime of the created world
 * @return A pointer to the created world, whose ownership is given to the
 * caller
 */
struct DcmWorld *App_DcmWorld_Create(
    struct DcmCanTxInterface *can_tx_interface,
    struct DcmCanRxInterface *can_rx_interface,
    struct HeartbeatMonitor * heartbeat_monitor,
    struct RgbLedSequence *   rgb_led_sequence,
    struct BrakeLight *       brake_light,
    struct Buzzer *           buzzer,
    struct Imu *              imu,
    struct ErrorTable *       error_table,
    struct Clock *            clock,
    struct InverterSwitches * inverter_switches,
    bool (*is_buzzer_on)(struct DcmWorld *),
    void (*buzzer_callback)(struct DcmWorld *));

/**
 * Deallocate the memory used by the given world
 * @param world The world to deallocate
 */
void App_DcmWorld_Destroy(struct DcmWorld *world);

/**
 * Get the CAN TX interface for the given world
 * @param world The world to get CAN TX interface for
 * @return The CAN TX interface for the given world
 */
struct DcmCanTxInterface *App_DcmWorld_GetCanTx(const struct DcmWorld *world);

/**
 * Get the CAN RX interface for the given world
 * @param world The world to get CAN RX interface for
 * @return The CAN RX interface for the given world
 */
struct DcmCanRxInterface *App_DcmWorld_GetCanRx(const struct DcmWorld *world);

/**
 * Get the heartbeat monitor for the given world
 * @param world The world to get heartbeat monitor for
 * @return The heartbeat monitor for the given world
 */
struct HeartbeatMonitor *App_DcmWorld_GetHeartbeatMonitor(const struct DcmWorld *world);

/**
 * Get the RGB LED sequence for the given world
 * @param world The world to get RGB LED sequence for
 * @return The RGB LED sequence for the given world
 */
struct RgbLedSequence *App_DcmWorld_GetRgbLedSequence(const struct DcmWorld *world);

/**
 * Get the brake light for the given world
 * @param world The world to get brake light for
 * @return The brake light for the given world
 */
struct BrakeLight *App_DcmWorld_GetBrakeLight(const struct DcmWorld *world);

/**
 * Get the buzzer for the given world
 * @param world The world to get buzzer for
 * @return The buzzer for the given world
 */
struct Buzzer *App_DcmWorld_GetBuzzer(const struct DcmWorld *world);

/**
 * Get the Imu for the given world
 * @param world The world to get clock for
 * @return The Imu for the given world
 */
struct Imu *App_DcmWorld_GetImu(const struct DcmWorld *world);

/**
 * Get the error table for the given world
 * @param world The world to get error table for
 * @return The error table for the given world
 */
struct ErrorTable *App_DcmWorld_GetErrorTable(const struct DcmWorld *world);

/**
 * Update the registered wait signals in the given world
 * @note This function should be called periodically. And since the time
 *       resolution of the wait signal library is in milliseconds, it would make
 *       sense to call this function at 1kHz.
 * @param world The world to update the registered wait signals
 * @param current_ms The current time in milliseconds
 */
void App_DcmWorld_UpdateWaitSignal(const struct DcmWorld *world, uint32_t current_ms);

/**
 * Get the clock for the given world
 * @param world The world to get clock for
 * @return The clock for the given world
 */
struct Clock *App_DcmWorld_GetClock(const struct DcmWorld *world);

/**
 * Get the inverter switches for the given world
 * @param world The world to get inverter switches from
 * @return The inverter switches for the given world
 */
struct InverterSwitches *App_DcmWorld_GetInverterSwitches(const struct DcmWorld *world);
