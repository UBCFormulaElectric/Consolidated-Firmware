#include <stddef.h>
#include <stdlib.h>
#include <assert.h>

#include "App_DcmWorld.h"

struct DcmWorld
{
    struct DcmCanTxInterface *can_tx_interface;
    struct DcmCanRxInterface *can_rx_interface;
    struct HeartbeatMonitor * heartbeat_monitor;
    struct RgbLedSequence *   rgb_led_sequence;
    struct BrakeLight *       brake_light;
    struct Buzzer *           buzzer;
};

struct DcmWorld *App_DcmWorld_Create(
    struct DcmCanTxInterface *const can_tx_interface,
    struct DcmCanRxInterface *const can_rx_interface,
    struct HeartbeatMonitor *const  heartbeat_monitor,
    struct RgbLedSequence *const    rgb_led_sequence,
    struct BrakeLight *const        brake_light,
    struct Buzzer *const            buzzer)
{
    struct DcmWorld *world = (struct DcmWorld *)malloc(sizeof(struct DcmWorld));
    assert(world != NULL);

    world->can_tx_interface  = can_tx_interface;
    world->can_rx_interface  = can_rx_interface;
    world->heartbeat_monitor = heartbeat_monitor;
    world->rgb_led_sequence  = rgb_led_sequence;
    world->brake_light       = brake_light;
    world->buzzer            = buzzer;

    return world;
}

void App_DcmWorld_Destroy(struct DcmWorld *world)
{
    free(world);
}

struct DcmCanTxInterface *
    App_DcmWorld_GetCanTx(const struct DcmWorld *const world)
{
    return world->can_tx_interface;
}

struct DcmCanRxInterface *
    App_DcmWorld_GetCanRx(const struct DcmWorld *const world)
{
    return world->can_rx_interface;
}

struct HeartbeatMonitor *
    App_DcmWorld_GetHeartbeatMonitor(const struct DcmWorld *const world)
{
    return world->heartbeat_monitor;
}

struct RgbLedSequence *
    App_DcmWorld_GetRgbLedSequence(const struct DcmWorld *const world)
{
    return world->rgb_led_sequence;
}

struct BrakeLight *
    App_DcmWorld_GetBrakeLight(const struct DcmWorld *const world)
{
    return world->brake_light;
}

struct Buzzer *App_DcmWorld_GetBuzzer(const struct DcmWorld *const world)
{
    return world->buzzer;
}
