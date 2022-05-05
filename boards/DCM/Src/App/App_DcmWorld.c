#include <stddef.h>
#include <stdlib.h>
#include <assert.h>

#include "App_DcmWorld.h"
#include "configs/App_WaitSignalDuration.h"

struct DcmWorld
{
    struct DcmCanTxInterface *can_tx_interface;
    struct DcmCanRxInterface *can_rx_interface;
    struct HeartbeatMonitor * heartbeat_monitor;
    struct RgbLedSequence *   rgb_led_sequence;
    struct BrakeLight *       brake_light;
    struct Buzzer *           buzzer;
    struct Imu *              imu;
    struct ErrorTable *       error_table;
    struct WaitSignal *       buzzer_wait_signal;
    struct Clock *            clock;
    struct InverterSwitches * inverter_switches;
};

struct DcmWorld *App_DcmWorld_Create(
    struct DcmCanTxInterface *const can_tx_interface,
    struct DcmCanRxInterface *const can_rx_interface,
    struct HeartbeatMonitor *const  heartbeat_monitor,
    struct RgbLedSequence *const    rgb_led_sequence,
    struct BrakeLight *const        brake_light,
    struct Buzzer *const            buzzer,
    struct Imu *const               imu,
    struct ErrorTable *const        error_table,
    struct Clock *const             clock,
    struct InverterSwitches *const  inverter_switches,
    bool (*is_buzzer_on)(struct DcmWorld *),
    void (*buzzer_complete_callback)(struct DcmWorld *))
{
    struct DcmWorld *world = malloc(sizeof(struct DcmWorld));
    assert(world != NULL);

    world->can_tx_interface  = can_tx_interface;
    world->can_rx_interface  = can_rx_interface;
    world->heartbeat_monitor = heartbeat_monitor;
    world->rgb_led_sequence  = rgb_led_sequence;
    world->brake_light       = brake_light;
    world->buzzer            = buzzer;
    world->imu               = imu;
    world->error_table       = error_table;
    world->clock             = clock;
    world->inverter_switches = inverter_switches;

    struct WaitSignalCallback buzzer_callback = { .function         = buzzer_complete_callback,
                                                  .wait_duration_ms = BUZZER_ON_DURATION_MS };
    world->buzzer_wait_signal                 = App_SharedWaitSignal_Create(0U, is_buzzer_on, world, buzzer_callback);

    return world;
}

void App_DcmWorld_Destroy(struct DcmWorld *world)
{
    free(world->buzzer_wait_signal);
    free(world);
}

struct DcmCanTxInterface *App_DcmWorld_GetCanTx(const struct DcmWorld *const world)
{
    return world->can_tx_interface;
}

struct DcmCanRxInterface *App_DcmWorld_GetCanRx(const struct DcmWorld *const world)
{
    return world->can_rx_interface;
}

struct HeartbeatMonitor *App_DcmWorld_GetHeartbeatMonitor(const struct DcmWorld *const world)
{
    return world->heartbeat_monitor;
}

struct RgbLedSequence *App_DcmWorld_GetRgbLedSequence(const struct DcmWorld *const world)
{
    return world->rgb_led_sequence;
}

struct BrakeLight *App_DcmWorld_GetBrakeLight(const struct DcmWorld *const world)
{
    return world->brake_light;
}

struct Buzzer *App_DcmWorld_GetBuzzer(const struct DcmWorld *const world)
{
    return world->buzzer;
}

struct Imu *App_DcmWorld_GetImu(const struct DcmWorld *const world)
{
    return world->imu;
}

struct ErrorTable *App_DcmWorld_GetErrorTable(const struct DcmWorld *const world)
{
    return world->error_table;
}

void App_DcmWorld_UpdateWaitSignal(const struct DcmWorld *const world, uint32_t current_ms)
{
    App_SharedWaitSignal_Update(world->buzzer_wait_signal, current_ms);
}

struct Clock *App_DcmWorld_GetClock(const struct DcmWorld *const world)
{
    return world->clock;
}

struct InverterSwitches *App_DcmWorld_GetInverterSwitches(const struct DcmWorld *world)
{
    return world->inverter_switches;
}
