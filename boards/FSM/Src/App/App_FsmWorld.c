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
    struct InRangeCheck *     primary_flow_rate_in_range_check;
    struct InRangeCheck *     secondary_flow_rate_in_range_check;
    struct InRangeCheck *     left_wheel_speed_in_range_check;
    struct InRangeCheck *     right_wheel_speed_in_range_check;
    struct InRangeCheck *     steering_angle_in_range_check;
    struct Brake *            brake;
    struct RgbLedSequence *   rgb_led_sequence;
    struct SignalNode *       signals_head;
    struct Clock *            clock;
    struct AcceleratorPedals *papps_and_sapps;
};

/**
 * Register a signal to the given world
 * @param world The world to register a signal for
 * @param signal The signal to register to the given world
 */
static void App_RegisterSignal(struct FsmWorld *world, struct Signal *signal)
{
    struct SignalNode *item = malloc(sizeof(struct SignalNode));
    assert(item != NULL);
    item->signal = signal;
    item->next   = NULL;

    SL_APPEND(world->signals_head, item);
}

struct FsmWorld *App_FsmWorld_Create(
    struct FsmCanTxInterface *const can_tx_interface,
    struct FsmCanRxInterface *const can_rx_interface,
    struct HeartbeatMonitor *const  heartbeat_monitor,
    struct InRangeCheck *const      primary_flow_rate_in_range_check,
    struct InRangeCheck *const      secondary_flow_rate_in_range_check,
    struct InRangeCheck *const      left_wheel_speed_in_range_check,
    struct InRangeCheck *const      right_wheel_speed_in_range_check,
    struct InRangeCheck *const      steering_angle_in_range_check,
    struct Brake *const             brake,
    struct RgbLedSequence *const    rgb_led_sequence,
    struct Clock *const             clock,
    struct AcceleratorPedals *const papps_and_sapps,

    bool (*const has_apps_and_brake_plausibility_failure)(struct FsmWorld *),
    bool (*const is_apps_and_brake_plausibility_ok)(struct FsmWorld *),
    void (*const apps_and_brake_plausibility_failure_callback)(struct FsmWorld *),
    bool (*const has_apps_disagreement)(struct FsmWorld *),
    bool (*const has_apps_agreement)(struct FsmWorld *),
    void (*const apps_disagreement_callback)(struct FsmWorld *),
    bool (*const is_papps_alarm_active)(struct FsmWorld *),
    void (*const papps_alarm_callback)(struct FsmWorld *),
    bool (*const is_sapps_alarm_active)(struct FsmWorld *),
    void (*const sapps_alarm_callback)(struct FsmWorld *),
    bool (*const is_papps_and_sapps_alarm_inactive)(struct FsmWorld *),

    bool (*const is_primary_flow_rate_below_threshold)(struct FsmWorld *),
    bool (*const is_primary_flow_rate_in_range)(struct FsmWorld *),
    void (*const primary_flow_rate_below_threshold_callback)(struct FsmWorld *),
    bool (*const is_secondary_flow_rate_below_threshold)(struct FsmWorld *),
    bool (*const is_secondary_flow_rate_in_range)(struct FsmWorld *),
    void (*const secondary_flow_rate_below_threshold_callback)(struct FsmWorld *))
{
    struct FsmWorld *world = (struct FsmWorld *)malloc(sizeof(struct FsmWorld));
    assert(world != NULL);

    world->can_tx_interface                   = can_tx_interface;
    world->can_rx_interface                   = can_rx_interface;
    world->heartbeat_monitor                  = heartbeat_monitor;
    world->primary_flow_rate_in_range_check   = primary_flow_rate_in_range_check;
    world->secondary_flow_rate_in_range_check = secondary_flow_rate_in_range_check;
    world->left_wheel_speed_in_range_check    = left_wheel_speed_in_range_check;
    world->right_wheel_speed_in_range_check   = right_wheel_speed_in_range_check;
    world->steering_angle_in_range_check      = steering_angle_in_range_check;
    world->brake                              = brake;
    world->rgb_led_sequence                   = rgb_led_sequence;
    world->signals_head                       = NULL;
    world->clock                              = clock;
    world->papps_and_sapps                    = papps_and_sapps;

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

    struct SignalCallback primary_flow_rate_callback = { .entry_condition_high_duration_ms = FLOW_METER_ENTRY_HIGH_MS,
                                                         .exit_condition_high_duration_ms  = FLOW_METER_EXIT_HIGH_MS,
                                                         .function = primary_flow_rate_below_threshold_callback };
    struct Signal *       primary_flow_rate_signal   = App_SharedSignal_Create(
        0, is_primary_flow_rate_below_threshold, is_primary_flow_rate_in_range, world, primary_flow_rate_callback);
    App_RegisterSignal(world, primary_flow_rate_signal);

    struct SignalCallback secondary_flow_rate_callback = { .entry_condition_high_duration_ms = FLOW_METER_ENTRY_HIGH_MS,
                                                           .exit_condition_high_duration_ms  = FLOW_METER_EXIT_HIGH_MS,
                                                           .function = secondary_flow_rate_below_threshold_callback };
    struct Signal *       secondary_flow_rate_signal   = App_SharedSignal_Create(
        0, is_secondary_flow_rate_below_threshold, is_secondary_flow_rate_in_range, world,
        secondary_flow_rate_callback);
    App_RegisterSignal(world, secondary_flow_rate_signal);

    return world;
}

void App_FsmWorld_Destroy(struct FsmWorld *world)
{
    struct SignalNode *node, *tmp;

    SL_FOREACH_SAFE(world->signals_head, node, tmp)
    {
        SL_DELETE(world->signals_head, node);
        free(node->signal);
        free(node);

        if (world->signals_head == NULL)
        {
            break;
        }
    }

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

struct InRangeCheck *App_FsmWorld_GetPrimaryFlowRateInRangeCheck(const struct FsmWorld *const world)
{
    return world->primary_flow_rate_in_range_check;
}

struct InRangeCheck *App_FsmWorld_GetSecondaryFlowRateInRangeCheck(const struct FsmWorld *const world)
{
    return world->secondary_flow_rate_in_range_check;
}

struct InRangeCheck *App_FsmWorld_GetLeftWheelSpeedInRangeCheck(const struct FsmWorld *const world)
{
    return world->left_wheel_speed_in_range_check;
}

struct InRangeCheck *App_FsmWorld_GetRightWheelSpeedInRangeCheck(const struct FsmWorld *const world)
{
    return world->right_wheel_speed_in_range_check;
}

struct InRangeCheck *App_FsmWorld_GetSteeringAngleInRangeCheck(const struct FsmWorld *const world)
{
    return world->steering_angle_in_range_check;
}

struct Brake *App_FsmWorld_GetBrake(const struct FsmWorld *const world)
{
    return world->brake;
}

struct RgbLedSequence *App_FsmWorld_GetRgbLedSequence(const struct FsmWorld *const world)
{
    return world->rgb_led_sequence;
}

struct AcceleratorPedals *App_FsmWorld_GetPappsAndSapps(const struct FsmWorld *const world)
{
    return world->papps_and_sapps;
}

void App_FsmWorld_UpdateSignals(const struct FsmWorld *world, uint32_t current_time_ms)
{
    struct SignalNode *node_ptr;

    SL_FOREACH(world->signals_head, node_ptr) { App_SharedSignal_Update(node_ptr->signal, current_time_ms); }
}

struct Clock *App_FsmWorld_GetClock(const struct FsmWorld *const world)
{
    return world->clock;
}
