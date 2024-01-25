#include <stddef.h>
#include <stdlib.h>
#include <assert.h>

#include "App_DcmWorld.h"
#include "configs/App_WaitSignalDuration.h"

struct DcmWorld
{
    struct HeartbeatMonitor * heartbeat_monitor;
    struct BrakeLight *       brake_light;
    struct Buzzer *           buzzer;
    struct Imu *              imu;
    struct WaitSignal *       buzzer_wait_signal;
    struct Clock *            clock;
};

struct DcmWorld * App_DcmWorld_Create(
    struct HeartbeatMonitor * const heartbeat_monitor,
    struct BrakeLight * const       brake_light,
    struct Buzzer * const           buzzer,
    struct Imu * const              imu,
    struct Clock * const            clock,
    bool (*is_buzzer_on)(struct DcmWorld *),
    void (*buzzer_complete_callback)(struct DcmWorld *))
{
    struct DcmWorld * world = malloc(sizeof(struct DcmWorld));
    assert(world != NULL);

    world->heartbeat_monitor = heartbeat_monitor;
    world->brake_light       = brake_light;
    world->buzzer            = buzzer;
    world->imu               = imu;
    world->clock             = clock;

    struct WaitSignalCallback buzzer_callback = { .function         = buzzer_complete_callback,
                                                  .wait_duration_ms = BUZZER_ON_DURATION_MS };
    world->buzzer_wait_signal                 = App_SharedWaitSignal_Create(0U, is_buzzer_on, world, buzzer_callback);

    return world;
}

void App_DcmWorld_Destroy(struct DcmWorld * world)
{
    free(world->buzzer_wait_signal);
    free(world);
}

struct HeartbeatMonitor * App_DcmWorld_GetHeartbeatMonitor(const struct DcmWorld * const world)
{
    return world->heartbeat_monitor;
}

struct BrakeLight * App_DcmWorld_GetBrakeLight(const struct DcmWorld * const world)
{
    return world->brake_light;
}

struct Buzzer * App_DcmWorld_GetBuzzer(const struct DcmWorld * const world)
{
    return world->buzzer;
}

struct Imu * App_DcmWorld_GetImu(const struct DcmWorld * const world)
{
    return world->imu;
}

void App_DcmWorld_UpdateWaitSignal(const struct DcmWorld * const world, uint32_t current_ms)
{
    App_SharedWaitSignal_Update(world->buzzer_wait_signal, current_ms);
}

struct Clock * App_DcmWorld_GetClock(const struct DcmWorld * const world)
{
    return world->clock;
}
