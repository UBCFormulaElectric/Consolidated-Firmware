#include <stdlib.h>
#include <assert.h>

#include "App_BmsWorld.h"

struct BmsWorld
{
    struct Imd *             imd;
    struct HeartbeatMonitor *heartbeat_monitor;
    struct RgbLedSequence *  rgb_led_sequence;
    struct Charger *         charger;
    struct OkStatus *        bms_ok;
    struct OkStatus *        imd_ok;
    struct OkStatus *        bspd_ok;
    struct Accumulator *     accumulator;
    struct Airs *            airs;
    struct PrechargeRelay *  precharge_relay;
    struct TractiveSystem *  ts;
    struct Clock *           clock;
    struct Eeprom *          eeprom;
};

struct BmsWorld *App_BmsWorld_Create(
    struct Imd *const              imd,
    struct HeartbeatMonitor *const heartbeat_monitor,
    struct RgbLedSequence *const   rgb_led_sequence,
    struct Charger *const          charger,
    struct OkStatus *const         bms_ok,
    struct OkStatus *const         imd_ok,
    struct OkStatus *const         bspd_ok,
    struct Accumulator *const      accumulator,
    struct Airs *const             airs,
    struct PrechargeRelay *const   precharge_relay,
    struct TractiveSystem *const   tractive_system,
    struct Clock *const            clock,
    struct Eeprom *const           eeprom)
{
    struct BmsWorld *world = (struct BmsWorld *)malloc(sizeof(struct BmsWorld));
    assert(world != NULL);

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
    world->clock             = clock;
    world->eeprom            = eeprom;

    return world;
}

void App_BmsWorld_Destroy(struct BmsWorld *world)
{
    free(world);
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

struct Clock *App_BmsWorld_GetClock(const struct BmsWorld *const world)
{
    return world->clock;
}

struct Eeprom *App_BmsWorld_GetEeprom(const struct BmsWorld *const world)
{
    return world->eeprom;
}
