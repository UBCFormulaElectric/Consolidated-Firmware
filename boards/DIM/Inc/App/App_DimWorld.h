#pragma once

#include "App_CanTx.h"
#include "App_CanRx.h"
#include "App_SevenSegDisplays.h"
#include "App_SharedHeartbeatMonitor.h"
#include "App_SharedRgbLedSequence.h"
#include "App_RotarySwitch.h"
#include "App_SharedErrorTable.h"
#include "App_SharedRgbLed.h"
#include "App_SharedClock.h"

struct DimWorld;

/**
 * Allocate and initialize a world in the application layer by registering
 * interfaces to it
 * @note This function does __not__ take ownership of any of the interfaces
 *       passed into it, which means the every interface must be kept alive for
 *       the lifetime of the created DIM world
 * @return A pointer to the created world, whose ownership is given to the
 * caller
 */
struct DimWorld *App_DimWorld_Create(
    struct DimCanTxInterface *can_tx_interface,
    struct DimCanRxInterface *can_rx_interface,
    struct SevenSegDisplays * seven_seg_displays,
    struct HeartbeatMonitor * heartbeat_monitor,
    struct RgbLedSequence *   rgb_led_sequence,
    struct RotarySwitch *     drive_mode_switch,
    struct ErrorTable *       error_table,
    struct RgbLed *           bms_status_led,
    struct RgbLed *           dcm_status_led,
    struct RgbLed *           dim_status_led,
    struct RgbLed *           fsm_status_led,
    struct RgbLed *           pdm_status_led,
    struct Clock *            clock);

/**
 * Deallocate the memory used by the given world
 * @param world The world to deallocate
 */
void App_DimWorld_Destroy(struct DimWorld *world);

/**
 * Get the CAN TX interface for the given world
 * @param world The world to get CAN TX interface for
 * @return The CAN TX interface for the given world
 */
struct DimCanTxInterface *App_DimWorld_GetCanTx(const struct DimWorld *world);

/**
 * Get the CAN RX interface for the given world
 * @param world The world to get CAN RX interface for
 * @return The CAN RX interface for the given world
 */
struct DimCanRxInterface *App_DimWorld_GetCanRx(const struct DimWorld *world);

/**
 * Get the 7-segment displays for the given world
 * @param world The world to get 7-segment displays for
 * @return The 7-segment displays for the given world
 */
struct SevenSegDisplays *App_DimWorld_GetSevenSegDisplays(const struct DimWorld *world);

/**
 * Get the heartbeat monitor for the given world
 * @param world The world to get heartbeat monitor for
 * @return The heartbeat monitor for the given world
 */
struct HeartbeatMonitor *App_DimWorld_GetHeartbeatMonitor(const struct DimWorld *world);

/**
 * Get the RGB LED sequence for the given world
 * @param world The world to get RGB LED sequence for
 * @return The RGB LED sequence for the given world
 */
struct RgbLedSequence *App_DimWorld_GetRgbLedSequence(const struct DimWorld *world);

/**
 * Get the error table for the given world
 * @param world The world to get error table for
 * @return The error table for the given world
 */
struct ErrorTable *App_DimWorld_GetErrorTable(const struct DimWorld *world);

/**
 * Get the BMS status LED for the given world
 * @param world The world to get BMS status LED for
 * @return The BMS status LED for the given world
 */
struct RgbLed *App_DimWorld_GetBmsStatusLed(const struct DimWorld *world);

/**
 * Get the DCM status LED for the given world
 * @param world The world to get DCM status LED for
 * @return The DCM status LED for the given world
 */
struct RgbLed *App_DimWorld_GetDcmStatusLed(const struct DimWorld *world);

/**
 * Get the DIM status LED for the given world
 * @param world The world to get DIM status LED for
 * @return The DIM status LED for the given world
 */
struct RgbLed *App_DimWorld_GetDimStatusLed(const struct DimWorld *world);

/**
 * Get the FSM status LED for the given world
 * @param world The world to get FSM status LED for
 * @return The FSM status LED for the given world
 */
struct RgbLed *App_DimWorld_GetFsmStatusLed(const struct DimWorld *world);

/**
 * Get the PDM status LED for the given world
 * @param world The world to get PDM status LED for
 * @return The PDM status LED for the given world
 */
struct RgbLed *App_DimWorld_GetPdmStatusLed(const struct DimWorld *world);

/**
 * Get the clock for the given world
 * @param world The world to get clock for
 * @return The clock for the given world
 */
struct Clock *App_DimWorld_GetClock(const struct DimWorld *world);
