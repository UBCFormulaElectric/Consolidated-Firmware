#include <stdlib.h>
#include <assert.h>

#include "App_BmsWorld.h"
#include "configs/App_WaitSignalDuration.h"

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
    struct BinaryStatus *     air_negative;
    struct BinaryStatus *     air_positive;
    struct PreChargeSequence *pre_charge_sequence;
    struct Clock *            clock;
    struct WaitSignal *       wait_after_init_signal;
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
    struct BinaryStatus *const      air_negative,
    struct BinaryStatus *const      air_positive,
    struct PreChargeSequence *const pre_charge_sequence,
    struct Clock *const             clock,

    bool (*is_in_init_state)(struct BmsWorld *),
    void (*wait_after_init_callback)(struct BmsWorld *))
{
    struct BmsWorld *world = (struct BmsWorld *)malloc(sizeof(struct BmsWorld));
    assert(world != NULL);

    world->can_tx_interface    = can_tx_interface;
    world->can_rx_interface    = can_rx_interface;
    world->imd                 = imd;
    world->heartbeat_monitor   = heartbeat_monitor;
    world->rgb_led_sequence    = rgb_led_sequence;
    world->charger             = charger;
    world->bms_ok              = bms_ok;
    world->imd_ok              = imd_ok;
    world->bspd_ok             = bspd_ok;
    world->accumulator         = accumulator;
    world->air_negative        = air_negative;
    world->air_positive        = air_positive;
    world->pre_charge_sequence = pre_charge_sequence;
    world->clock               = clock;

    struct WaitSignalCallback _wait_after_init_callback = {
        .function         = wait_after_init_callback,
        .wait_duration_ms = WAIT_AFTER_INIT_DURATION_MS
    };
    world->wait_after_init_signal = App_SharedWaitSignal_Create(
        0U, is_in_init_state, world, _wait_after_init_callback);

    return world;
}

void App_BmsWorld_Destroy(struct BmsWorld *world)
{
    free(world->wait_after_init_signal);
    free(world);
}

struct BmsCanTxInterface *
    App_BmsWorld_GetCanTx(const struct BmsWorld *const world)
{
    return world->can_tx_interface;
}

struct BmsCanRxInterface *
    App_BmsWorld_GetCanRx(const struct BmsWorld *const world)
{
    return world->can_rx_interface;
}

struct Imd *App_BmsWorld_GetImd(const struct BmsWorld *const world)
{
    return world->imd;
}

struct HeartbeatMonitor *
    App_BmsWorld_GetHeartbeatMonitor(const struct BmsWorld *const world)
{
    return world->heartbeat_monitor;
}

struct RgbLedSequence *
    App_BmsWorld_GetRgbLedSequence(const struct BmsWorld *const world)
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

struct OkStatus *
    App_BmsWorld_GetBspdOkStatus(const struct BmsWorld *const world)
{
    return world->bspd_ok;
}

struct Accumulator *
    App_BmsWorld_GetAccumulator(const struct BmsWorld *const world)
{
    return world->accumulator;
}

struct BinaryStatus *
    App_BmsWorld_GetAirNegative(const struct BmsWorld *const world)
{
    return world->air_negative;
}

struct BinaryStatus *
    App_BmsWorld_GetAirPositive(const struct BmsWorld *const world)
{
    return world->air_positive;
}

struct PreChargeSequence *
    App_BmsWorld_GetPreChargeSequence(const struct BmsWorld *const world)
{
    return world->pre_charge_sequence;
}

struct Clock *App_BmsWorld_GetClock(const struct BmsWorld *const world)
{
    return world->clock;
}

void App_BmsWorld_UpdateWaitSignal(
    const struct BmsWorld *const world,
    uint32_t                     current_ms)
{
    App_SharedWaitSignal_Update(world->wait_after_init_signal, current_ms);
}
