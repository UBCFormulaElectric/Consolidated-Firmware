#include <stddef.h>
#include <stdlib.h>
#include <assert.h>
#include <linkedlist.h>

#include "App_FsmWorld.h"

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
    struct InRangeCheck *     brake_pressure_in_range_check;
    struct BinaryStatus *     brake_actuation_status;
    struct RgbLedSequence *   rgb_led_sequence;
    struct SignalNode *       signals_head;
    struct Clock *            clock;
    struct AcceleratorPedal * papps;
    struct AcceleratorPedal * sapps;
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
    struct InRangeCheck *const      brake_pressure_in_range_check,
    struct BinaryStatus *const      brake_actuation_status,
    struct RgbLedSequence *const    rgb_led_sequence,
    struct Clock *const             clock,
    struct AcceleratorPedal *const  papps,
    bool (*is_papps_alaram_active)(struct FsmWorld *),
    void (*papps_alarm_callback)(struct FsmWorld *),
    struct AcceleratorPedal *const sapps,
    bool (*is_sapps_alaram_active)(struct FsmWorld *),
    void (*sapps_alarm_callback)(struct FsmWorld *))
{
    struct FsmWorld *world = (struct FsmWorld *)malloc(sizeof(struct FsmWorld));
    assert(world != NULL);

    world->can_tx_interface                 = can_tx_interface;
    world->can_rx_interface                 = can_rx_interface;
    world->heartbeat_monitor                = heartbeat_monitor;
    world->primary_flow_rate_in_range_check = primary_flow_rate_in_range_check;
    world->secondary_flow_rate_in_range_check =
        secondary_flow_rate_in_range_check;
    world->left_wheel_speed_in_range_check  = left_wheel_speed_in_range_check;
    world->right_wheel_speed_in_range_check = right_wheel_speed_in_range_check;
    world->steering_angle_in_range_check    = steering_angle_in_range_check;
    world->brake_pressure_in_range_check    = brake_pressure_in_range_check;
    world->brake_actuation_status           = brake_actuation_status;
    world->rgb_led_sequence                 = rgb_led_sequence;
    world->signals_head                     = NULL;
    world->clock                            = clock;
    world->papps                            = papps;
    world->sapps                            = sapps;

    struct SignalCallback papps_callback = {
        .high_duration_ms = 10,
        .function         = papps_alarm_callback,
    };
    struct Signal *papps_signal = App_SharedSignal_Create(
        0, is_papps_alaram_active, world, papps_callback);
    App_RegisterSignal(world, papps_signal);

    struct SignalCallback sapps_callback = {
        .high_duration_ms = 10,
        .function         = sapps_alarm_callback,
    };
    struct Signal *sapps_signal = App_SharedSignal_Create(
        0, is_sapps_alaram_active, world, sapps_callback);
    App_RegisterSignal(world, sapps_signal);

    return world;
}

void App_FsmWorld_Destroy(struct FsmWorld *world)
{
    struct SignalNode *node, *tmp;

    SL_FOREACH_SAFE(world->signals_head, node, tmp)
    {
        if (world->signals_head == NULL)
        {
            break;
        }

        free(node->signal);
        SL_DELETE(world->signals_head, node);
        free(node);
    }

    free(world);
}

struct FsmCanTxInterface *
    App_FsmWorld_GetCanTx(const struct FsmWorld *const world)
{
    return world->can_tx_interface;
}

struct FsmCanRxInterface *
    App_FsmWorld_GetCanRx(const struct FsmWorld *const world)
{
    return world->can_rx_interface;
}

struct HeartbeatMonitor *
    App_FsmWorld_GetHeartbeatMonitor(const struct FsmWorld *const world)
{
    return world->heartbeat_monitor;
}

struct InRangeCheck *App_FsmWorld_GetPrimaryFlowRateInRangeCheck(
    const struct FsmWorld *const world)
{
    return world->primary_flow_rate_in_range_check;
}

struct InRangeCheck *App_FsmWorld_GetSecondaryFlowRateInRangeCheck(
    const struct FsmWorld *const world)
{
    return world->secondary_flow_rate_in_range_check;
}

struct InRangeCheck *App_FsmWorld_GetLeftWheelSpeedInRangeCheck(
    const struct FsmWorld *const world)
{
    return world->left_wheel_speed_in_range_check;
}

struct InRangeCheck *App_FsmWorld_GetRightWheelSpeedInRangeCheck(
    const struct FsmWorld *const world)
{
    return world->right_wheel_speed_in_range_check;
}

struct InRangeCheck *App_FsmWorld_GetSteeringAngleInRangeCheck(
    const struct FsmWorld *const world)
{
    return world->steering_angle_in_range_check;
}

struct InRangeCheck *App_FsmWorld_GetBrakePressureInRangeCheck(
    const struct FsmWorld *const world)
{
    return world->brake_pressure_in_range_check;
}

struct BinaryStatus *
    App_FsmWorld_GetBrakeActuationStatus(const struct FsmWorld *const world)
{
    return world->brake_actuation_status;
}

struct RgbLedSequence *
    App_FsmWorld_GetRgbLedSequence(const struct FsmWorld *const world)
{
    return world->rgb_led_sequence;
}

void App_FsmWorld_UpdateSignals(
    const struct FsmWorld *world,
    uint32_t               current_time_ms)
{
    struct SignalNode  node;
    struct SignalNode *node_ptr = &node;

    SL_FOREACH(world->signals_head, node_ptr)
    {
        App_SharedSignal_Update(node_ptr->signal, current_time_ms);
    }
}

struct Clock *App_FsmWorld_GetClock(const struct FsmWorld *const world)
{
    return world->clock;
}

struct AcceleratorPedal *
    App_FsmWorld_GetPapps(const struct FsmWorld *const world)
{
    return world->papps;
}

struct AcceleratorPedal *
    App_FsmWorld_GetSapps(const struct FsmWorld *const world)
{
    return world->sapps;
}
