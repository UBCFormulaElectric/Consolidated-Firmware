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
    struct RotarySwitch *     drive_mode_switch;
    struct Led *              imd_led;
    struct Led *              bspd_led;
    struct BinarySwitch *     start_switch;
    struct BinarySwitch *     traction_control_switch;
    struct BinarySwitch *     torque_vectoring_switch;
    struct ErrorTable *       error_table;
    struct RgbLed *           bms_status_led;
    struct RgbLed *           dcm_status_led;
    struct RgbLed *           dim_status_led;
    struct RgbLed *           fsm_status_led;
    struct RgbLed *           pdm_status_led;
    struct Clock *            clock;
};

struct DimWorld *App_DimWorld_Create(
    struct DimCanTxInterface *const can_tx_interface,
    struct DimCanRxInterface *const can_rx_interface,
    struct SevenSegDisplays *const  seven_seg_displays,
    struct HeartbeatMonitor *const  heartbeat_monitor,
    struct RegenPaddle *const       regen_paddle,
    struct RgbLedSequence *const    rgb_led_sequence,
    struct RotarySwitch *const      drive_mode_switch,
    struct Led *const               imd_led,
    struct Led *const               bspd_led,
    struct BinarySwitch *const      start_switch,
    struct BinarySwitch *const      traction_control_switch,
    struct BinarySwitch *const      torque_vectoring_switch,
    struct ErrorTable *const        error_table,
    struct RgbLed *const            bms_status_led,
    struct RgbLed *const            dcm_status_led,
    struct RgbLed *const            dim_status_led,
    struct RgbLed *const            fsm_status_led,
    struct RgbLed *const            pdm_status_led,
    struct Clock *const             clock)
{
    struct DimWorld *world = (struct DimWorld *)malloc(sizeof(struct DimWorld));
    assert(world != NULL);

    world->can_tx_interface        = can_tx_interface;
    world->can_rx_interface        = can_rx_interface;
    world->seven_seg_displays      = seven_seg_displays;
    world->heartbeat_monitor       = heartbeat_monitor;
    world->regen_paddle            = regen_paddle;
    world->rgb_led_sequence        = rgb_led_sequence;
    world->drive_mode_switch       = drive_mode_switch;
    world->imd_led                 = imd_led;
    world->bspd_led                = bspd_led;
    world->start_switch            = start_switch;
    world->traction_control_switch = traction_control_switch;
    world->torque_vectoring_switch = torque_vectoring_switch;
    world->error_table             = error_table;
    world->bms_status_led          = bms_status_led;
    world->dcm_status_led          = dcm_status_led;
    world->dim_status_led          = dim_status_led;
    world->fsm_status_led          = fsm_status_led;
    world->pdm_status_led          = pdm_status_led;
    world->clock                   = clock;

    return world;
}

void App_DimWorld_Destroy(struct DimWorld *world)
{
    free(world);
}

struct DimCanTxInterface *App_DimWorld_GetCanTx(const struct DimWorld *const world)
{
    return world->can_tx_interface;
}

struct DimCanRxInterface *App_DimWorld_GetCanRx(const struct DimWorld *const world)
{
    return world->can_rx_interface;
}

struct SevenSegDisplays *App_DimWorld_GetSevenSegDisplays(const struct DimWorld *const world)
{
    return world->seven_seg_displays;
}

struct HeartbeatMonitor *App_DimWorld_GetHeartbeatMonitor(const struct DimWorld *const world)
{
    return world->heartbeat_monitor;
}

struct RegenPaddle *App_DimWorld_GetRegenPaddle(const struct DimWorld *const world)
{
    return world->regen_paddle;
}

struct RgbLedSequence *App_DimWorld_GetRgbLedSequence(const struct DimWorld *const world)
{
    return world->rgb_led_sequence;
}

struct RotarySwitch *App_DimWorld_GetDriveModeSwitch(const struct DimWorld *const world)
{
    return world->drive_mode_switch;
}

struct Led *App_DimWorld_GetImdLed(const struct DimWorld *const world)
{
    return world->imd_led;
}

struct Led *App_DimWorld_GetBspdLed(const struct DimWorld *world)
{
    return world->bspd_led;
}

struct BinarySwitch *App_DimWorld_GetStartSwitch(const struct DimWorld *const world)
{
    return world->start_switch;
}

struct BinarySwitch *App_DimWorld_GetTractionControlSwitch(const struct DimWorld *const world)
{
    return world->traction_control_switch;
}

struct BinarySwitch *App_DimWorld_GetTorqueVectoringSwitch(const struct DimWorld *const world)
{
    return world->torque_vectoring_switch;
}

struct ErrorTable *App_DimWorld_GetErrorTable(const struct DimWorld *const world)
{
    return world->error_table;
}

struct RgbLed *App_DimWorld_GetBmsStatusLed(const struct DimWorld *world)
{
    return world->bms_status_led;
}

struct RgbLed *App_DimWorld_GetDcmStatusLed(const struct DimWorld *world)
{
    return world->dcm_status_led;
}

struct RgbLed *App_DimWorld_GetDimStatusLed(const struct DimWorld *world)
{
    return world->dim_status_led;
}

struct RgbLed *App_DimWorld_GetFsmStatusLed(const struct DimWorld *world)
{
    return world->fsm_status_led;
}

struct RgbLed *App_DimWorld_GetPdmStatusLed(const struct DimWorld *world)
{
    return world->pdm_status_led;
}

struct Clock *App_DimWorld_GetClock(const struct DimWorld *world)
{
    return world->clock;
}
