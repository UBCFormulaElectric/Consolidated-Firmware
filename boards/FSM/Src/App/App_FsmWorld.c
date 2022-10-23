#include <stddef.h>
#include <stdlib.h>
#include <assert.h>
#include <linkedlist.h>

#include "App_FsmWorld.h"
#include "configs/App_SignalCallbackDurations.h"

struct SignalNode
{
    struct Signal *    signal;
    struct SignalNode *next;
};

struct FsmWorld
{
    struct FsmCanTxInterface *can_tx_interface;
    struct FsmCanRxInterface *can_rx_interface;
    struct HeartbeatMonitor * heartbeat_monitor;
    struct Clock *            clock;
    struct AcceleratorPedals *papps_and_sapps;
    struct Brake *            brake;
    struct Coolant *          coolant;
    struct Steering *         steering;
    struct Wheels *           wheels;
    struct RgbLedSequence *   rgb_led_sequence;
};

struct FsmWorld *App_FsmWorld_Create(
    struct FsmCanTxInterface *const can_tx_interface,
    struct FsmCanRxInterface *const can_rx_interface,
    struct HeartbeatMonitor *const  heartbeat_monitor,
    struct Clock *const             clock,

    struct AcceleratorPedals *const papps_and_sapps,
    struct Brake *const             brake,
    struct Coolant *const           coolant,
    struct Steering *const          steering,
    struct Wheels *const            wheels,

    struct RgbLedSequence *const rgb_led_sequence,

    bool (*const is_flow_rate_below_threshold)(struct FsmWorld *),
    bool (*const is_flow_rate_in_range)(struct FsmWorld *),
    void (*const flow_rate_below_threshold_callback)(struct FsmWorld *))
{
    struct FsmWorld *world = (struct FsmWorld *)malloc(sizeof(struct FsmWorld));
    assert(world != NULL);

    world->can_tx_interface  = can_tx_interface;
    world->can_rx_interface  = can_rx_interface;
    world->heartbeat_monitor = heartbeat_monitor;
    world->clock             = clock;

    world->papps_and_sapps  = papps_and_sapps;
    world->brake            = brake;
    world->coolant          = coolant;
    world->steering         = steering;
    world->wheels           = wheels;
    world->rgb_led_sequence = rgb_led_sequence;

    // Signals
    struct SignalCallback papps_callback = {
        .entry_condition_high_duration_ms = PAPPS_ENTRY_HIGH_MS,
        .exit_condition_high_duration_ms  = PAPPS_EXIT_HIGH_MS,
        .function                         = papps_alarm_callback,
    };
    struct Signal *papps_alarm_signal =
        App_SharedSignal_Create(0, is_papps_alarm_active, is_papps_and_sapps_alarm_inactive, world, papps_callback);
    App_RegisterSignal(world, papps_alarm_signal);

    struct SignalCallback sapps_callback = {
        .entry_condition_high_duration_ms = SAPPS_ENTRY_HIGH_MS,
        .exit_condition_high_duration_ms  = SAPPS_EXIT_HIGH_MS,
        .function                         = sapps_alarm_callback,
    };
    struct Signal *sapps_alarm_signal =
        App_SharedSignal_Create(0, is_sapps_alarm_active, is_papps_and_sapps_alarm_inactive, world, sapps_callback);
    App_RegisterSignal(world, sapps_alarm_signal);

    struct SignalCallback apps_callback = {
        .entry_condition_high_duration_ms = APPS_ENTRY_HIGH_MS,
        .exit_condition_high_duration_ms  = APPS_EXIT_HIGH_MS,
        .function                         = apps_disagreement_callback,
    };
    struct Signal *apps_disagreement_signal =
        App_SharedSignal_Create(0, has_apps_disagreement, has_apps_agreement, world, apps_callback);
    App_RegisterSignal(world, apps_disagreement_signal);

    struct SignalCallback apps_and_brake_callback = {
        .entry_condition_high_duration_ms = APPS_AND_BRAKE_ENTRY_HIGH_MS,
        .exit_condition_high_duration_ms  = APPS_AND_BRAKE_EXIT_HIGH_MS,
        .function                         = apps_and_brake_plausibility_failure_callback,
    };
    struct Signal *apps_and_brake_plausibility_check_signal = App_SharedSignal_Create(
        0, has_apps_and_brake_plausibility_failure, is_apps_and_brake_plausibility_ok, world, apps_and_brake_callback);
    App_RegisterSignal(world, apps_and_brake_plausibility_check_signal);

    struct SignalCallback flow_rate_callback = { .entry_condition_high_duration_ms = FLOW_METER_ENTRY_HIGH_MS,
                                                 .exit_condition_high_duration_ms  = FLOW_METER_EXIT_HIGH_MS,
                                                 .function = flow_rate_below_threshold_callback };
    struct Signal *       flow_rate_signal =
        App_SharedSignal_Create(0, is_flow_rate_below_threshold, is_flow_rate_in_range, world, flow_rate_callback);
    App_RegisterSignal(world, flow_rate_signal);

    return world;
}

void App_FsmWorld_Destroy(struct FsmWorld *world){
    free(world);
}

struct FsmCanTxInterface *App_FsmWorld_GetCanTx(const struct FsmWorld *const world)
{
    return world->can_tx_interface;
}

struct FsmCanRxInterface *App_FsmWorld_GetCanRx(const struct FsmWorld *const world)
{
    return world->can_rx_interface;
}

struct HeartbeatMonitor *App_FsmWorld_GetHeartbeatMonitor(const struct FsmWorld *const world)
{
    return world->heartbeat_monitor;
}

struct AcceleratorPedals *App_FsmWorld_GetPappsAndSapps(const struct FsmWorld *const world)
{
    return world->papps_and_sapps;
}

struct Brake *App_FsmWorld_GetBrake(const struct FsmWorld *const world)
{
    return world->brake;
}

struct Coolant *App_FsmWorld_GetCoolant(const struct FsmWorld *const world)
{
    return world->coolant;
}

struct Steering *App_FsmWorld_GetSteering(const struct FsmWorld *const world)
{
    return world->steering;
}

struct Wheels *App_FsmWorld_GetWheels(const struct FsmWorld *const world)
{
    return world->wheels;
}

struct RgbLedSequence *App_FsmWorld_GetRgbLedSequence(const struct FsmWorld *const world)
{
    return world->rgb_led_sequence;
}

struct Clock *App_FsmWorld_GetClock(const struct FsmWorld *const world)
{
    return world->clock;
}
