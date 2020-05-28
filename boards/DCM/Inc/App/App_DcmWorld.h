#pragma once

#include "App_CanTx.h"
#include "App_CanRx.h"
#include "App_SharedHeartbeatMonitor.h"
#include "App_SharedRgbLedSequence.h"
#include "App_BrakeLight.h"
#include "App_Buzzer.h"

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
    struct Buzzer *           buzzer);

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
struct HeartbeatMonitor *
    App_DcmWorld_GetHeartbeatMonitor(const struct DcmWorld *world);

/**
 * Get the RGB LED sequence for the given world
 * @param world The world to get RGB LED sequence for
 * @return The RGB LED sequence for the given world
 */
struct RgbLedSequence *
    App_DcmWorld_GetRgbLedSequence(const struct DcmWorld *world);

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
