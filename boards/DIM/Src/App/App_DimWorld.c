#include <stddef.h>
#include <stdlib.h>
#include <assert.h>

#include "App_DimWorld.h"

struct DimWorld
{
    struct DimCanTxInterface *can_tx_interface;
    struct DimCanRxInterface *can_rx_interface;
    struct SevenSegDisplays * seven_seg_displays;
    struct HeartbeatMonitor * heartbeat_monitor;
    struct RegenPaddle *      regen_paddle;
    struct RgbLedSequence *   rgb_led_sequence;
};

struct DimWorld *App_DimWorld_Create(
    struct DimCanTxInterface *const can_tx_interface,
    struct DimCanRxInterface *const can_rx_interface,
    struct SevenSegDisplays *const  seven_seg_displays,
    struct HeartbeatMonitor *const  heartbeat_monitor,
    struct RegenPaddle *            regen_paddle,
    struct RgbLedSequence *const    rgb_led_sequence)
{
    struct DimWorld *world = (struct DimWorld *)malloc(sizeof(struct DimWorld));
    assert(world != NULL);

    world->can_tx_interface   = can_tx_interface;
    world->can_rx_interface   = can_rx_interface;
    world->seven_seg_displays = seven_seg_displays;
    world->heartbeat_monitor  = heartbeat_monitor;
    world->regen_paddle       = regen_paddle;
    world->rgb_led_sequence   = rgb_led_sequence;

    return world;
}

void App_DimWorld_Destroy(struct DimWorld *world)
{
    free(world);
}

struct DimCanTxInterface *
    App_DimWorld_GetCanTx(const struct DimWorld *const world)
{
    return world->can_tx_interface;
}

struct DimCanRxInterface *
    App_DimWorld_GetCanRx(const struct DimWorld *const world)
{
    return world->can_rx_interface;
}

struct SevenSegDisplays *
    App_DimWorld_GetSevenSegDisplays(const struct DimWorld *const world)
{
    return world->seven_seg_displays;
}

struct HeartbeatMonitor *
    App_DimWorld_GetHeartbeatMonitor(const struct DimWorld *const world)
{
    return world->heartbeat_monitor;
}

struct RegenPaddle *
    App_DimWorld_GetRegenPaddle(const struct DimWorld *const world)
{
    return world->regen_paddle;
}

struct RgbLedSequence *
    App_DimWorld_GetRgbLedSequence(const struct DimWorld *const world)
{
    return world->rgb_led_sequence;
}
