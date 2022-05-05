#pragma once

#include "App_CanTx.h"
#include "App_CanRx.h"
#include "App_SevenSegDisplays.h"
#include "App_SharedHeartbeatMonitor.h"
#include "App_RegenPaddle.h"
#include "App_SharedRgbLedSequence.h"
#include "App_RotarySwitch.h"
#include "App_Led.h"
#include "App_BinarySwitch.h"
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
    struct RegenPaddle *      regen_paddle,
    struct RgbLedSequence *   rgb_led_sequence,
    struct RotarySwitch *     drive_mode_switch,
    struct Led *              imd_led,
    struct Led *              bspd_led,
    struct BinarySwitch *     start_switch,
    struct BinarySwitch *     traction_control_switch,
    struct BinarySwitch *     torque_vectoring_switch,
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
 * Get the regen paddle for the given world
 * @param world The world to get regen paddle for
 * @return The regen paddle for the given world
 */
struct RegenPaddle *App_DimWorld_GetRegenPaddle(const struct DimWorld *world);

/**
 * Get the RGB LED sequence for the given world
 * @param world The world to get RGB LED sequence for
 * @return The RGB LED sequence for the given world
 */
struct RgbLedSequence *App_DimWorld_GetRgbLedSequence(const struct DimWorld *world);

/**
 * Get the drive mode switch for the given world
 * @param world The world to get drive mode switch for
 * @return The drive mode switch for the given world
 */
struct RotarySwitch *App_DimWorld_GetDriveModeSwitch(const struct DimWorld *world);
/**
 * Get the start switch for the given world
 * @param world The world to get start switch for
 * @return The start switch for the given world
 */
struct BinarySwitch *App_DimWorld_GetStartSwitch(const struct DimWorld *world);

/**
 * Get the traction control switch for the given world
 * @param world The world to get traction control switch for
 * @return The traction control switch for the given world
 */
struct BinarySwitch *App_DimWorld_GetTractionControlSwitch(const struct DimWorld *world);

/**
 * Get the torque vectoring switch for the given world
 * @param world The world to get torque vectoring switch for
 * @return The torque vectoring switch for the given world
 */
struct BinarySwitch *App_DimWorld_GetTorqueVectoringSwitch(const struct DimWorld *world);

/**
 * Get the IMD LED for the given world
 * @param world The world to get IMD LED for
 * @return The IMD LED for the given world
 */
struct Led *App_DimWorld_GetImdLed(const struct DimWorld *world);

/**
 * Get the BSPD LED for the given world
 * @param world The world to get BSPD LED for
 * @return The BSPD LED for the given world
 */
struct Led *App_DimWorld_GetBspdLed(const struct DimWorld *world);

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
