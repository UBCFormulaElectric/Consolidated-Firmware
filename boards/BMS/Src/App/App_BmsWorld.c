#include <stdlib.h>
#include <assert.h>

#include "App_BmsWorld.h"

struct BmsWorld
{
    struct BmsCanTxInterface *can_tx_interface;
    struct BmsCanRxInterface *can_rx_interface;
    struct Imd *              imd;
    struct HeartbeatMonitor * heartbeat_monitor;
    struct RgbLedSequence *   rgb_led_sequence;
    struct Charger *          charger;
    struct OkStatus *         bms_ok;
    struct OkStatus *         imd_ok;
    struct OkStatus *         bspd_ok;
    struct Accumulator *      accumulator;
    struct Airs *             airs;
    struct PrechargeRelay *   precharge_relay;
    struct TractiveSystem *   ts;
    struct ErrorTable *       error_table;
    struct Clock *            clock;
};

struct BmsWorld *App_BmsWorld_Create(
    struct BmsCanTxInterface *const can_tx_interface,
    struct BmsCanRxInterface *const can_rx_interface,
    struct Imd *const               imd,
    struct HeartbeatMonitor *const  heartbeat_monitor,
    struct RgbLedSequence *const    rgb_led_sequence,
    struct Charger *const           charger,
    struct OkStatus *const          bms_ok,
    struct OkStatus *const          imd_ok,
    struct OkStatus *const          bspd_ok,
    struct Accumulator *const       accumulator,
    struct Airs *const              airs,
    struct PrechargeRelay *const    precharge_relay,
    struct TractiveSystem *const    tractive_system,
    struct ErrorTable *const        error_table,
    struct Clock *const             clock)
{
    struct BmsWorld *world = (struct BmsWorld *)malloc(sizeof(struct BmsWorld));
    assert(world != NULL);

    world->can_tx_interface  = can_tx_interface;
    world->can_rx_interface  = can_rx_interface;
    world->imd               = imd;
    world->heartbeat_monitor = heartbeat_monitor;
    world->rgb_led_sequence  = rgb_led_sequence;
    world->charger           = charger;
    world->bms_ok            = bms_ok;
    world->imd_ok            = imd_ok;
    world->bspd_ok           = bspd_ok;
    world->accumulator       = accumulator;
    world->airs              = airs;
    world->precharge_relay   = precharge_relay;
    world->ts                = tractive_system;
    world->error_table       = error_table;
    world->clock             = clock;

    return world;
}

void App_BmsWorld_Destroy(struct BmsWorld *world)
{
    free(world);
}

struct BmsCanTxInterface *App_BmsWorld_GetCanTx(const struct BmsWorld *const world)
{
    return world->can_tx_interface;
}

struct BmsCanRxInterface *App_BmsWorld_GetCanRx(const struct BmsWorld *const world)
{
    return world->can_rx_interface;
}

struct Imd *App_BmsWorld_GetImd(const struct BmsWorld *const world)
{
    return world->imd;
}

struct HeartbeatMonitor *App_BmsWorld_GetHeartbeatMonitor(const struct BmsWorld *const world)
{
    return world->heartbeat_monitor;
}

struct RgbLedSequence *App_BmsWorld_GetRgbLedSequence(const struct BmsWorld *const world)
{
    return world->rgb_led_sequence;
}

struct Charger *App_BmsWorld_GetCharger(const struct BmsWorld *const world)
{
    return world->charger;
}

struct OkStatus *App_BmsWorld_GetBmsOkStatus(const struct BmsWorld *const world)
{
    return world->bms_ok;
}

struct OkStatus *App_BmsWorld_GetImdOkStatus(const struct BmsWorld *const world)
{
    return world->imd_ok;
}

struct OkStatus *App_BmsWorld_GetBspdOkStatus(const struct BmsWorld *const world)
{
    return world->bspd_ok;
}

struct Accumulator *App_BmsWorld_GetAccumulator(const struct BmsWorld *const world)
{
    return world->accumulator;
}

struct Airs *App_BmsWorld_GetAirs(const struct BmsWorld *const world)
{
    return world->airs;
}

struct PrechargeRelay *App_BmsWorld_GetPrechargeRelay(const struct BmsWorld *const world)
{
    return world->precharge_relay;
}

struct TractiveSystem *App_BmsWorld_GetTractiveSystem(const struct BmsWorld *const world)
{
    return world->ts;
}

struct ErrorTable *App_BmsWorld_GetErrorTable(const struct BmsWorld *const world)
{
    return world->error_table;
}

struct Clock *App_BmsWorld_GetClock(const struct BmsWorld *const world)
{
    return world->clock;
}
