#include "App_CanTx.h"
#include <assert.h>

#define MAX_NUM_OF_FLOWMETERS 2

struct FlowMeter
{
    float (*get_flow_rate)(void);
    struct FsmCanTxInterface *can_tx;
};

struct FlowMeter *App_FlowMeter_Create(
    struct FsmCanTxInterface *const can_tx,
    float (*get_flow_rate)(void))
{
    assert(get_flow_rate != NULL);

    static struct FlowMeter flow_meters[MAX_NUM_OF_FLOWMETERS];
    static size_t           alloc_index = 0;

    assert(alloc_index < MAX_NUM_OF_FLOWMETERS);

    struct FlowMeter *const flow_meter = &flow_meters[alloc_index++];
    flow_meter->get_flow_rate          = get_flow_rate;
    flow_meter->can_tx                 = can_tx;

    return flow_meter;
}

float App_FlowMeter_ReadFlowRate(struct FlowMeter *flow_meter)
{
    return flow_meter->get_flow_rate();
}

void App_FlowMeter_TickPrimary(struct FlowMeter *primary_flow_meter)
{
    const float primary_flow_rate = primary_flow_meter->get_flow_rate();
    App_CanTx_SetPeriodicSignal_PRIMARY_FLOW_METER_FLOW_RATE(
        primary_flow_meter->can_tx, primary_flow_rate);
}

void App_FlowMeter_TickSecondary(struct FlowMeter *secondary_flow_meter)
{
    const float secondary_flow_rate = secondary_flow_meter->get_flow_rate();
    App_CanTx_SetPeriodicSignal_SECONDARY_FLOW_METER_FLOW_RATE(
        secondary_flow_meter->can_tx, secondary_flow_rate);
}
