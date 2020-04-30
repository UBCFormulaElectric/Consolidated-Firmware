#pragma once

#include "App_CanTx.h"
#include "App_CanRx.h"
#include "App_SevenSegDisplays.h"
#include "App_SharedHeartbeatMonitor.h"
#include "App_RegenPaddle.h"
#include "App_SharedRgbLedSequence.h"

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
    struct RgbLedSequence *   rgb_led_sequence);

/**
 * Destroy the given DIM world, freeing the memory associated with it
 * @param world The DIM world to destroy
 */
void App_DimWorld_Destroy(struct DimWorld *world);

struct DimCanTxInterface *App_DimWorld_GetCanTx(const struct DimWorld *world);
struct DimCanRxInterface *App_DimWorld_GetCanRx(const struct DimWorld *world);
struct SevenSegDisplays *
    App_DimWorld_GetSevenSegDisplays(const struct DimWorld *world);
struct HeartbeatMonitor *
                    App_DimWorld_GetHeartbeatMonitor(const struct DimWorld *world);
struct RegenPaddle *App_DimWorld_GetRegenPaddle(const struct DimWorld *world);
struct RgbLedSequence *
    App_DimWorld_GetRgbLedSequence(const struct DimWorld *world);
