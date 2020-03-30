#include "App_FlowMeter.h"

#define MAX_NUM_OF_FLOWMETERS 2

struct FlowMeter
{
    float (*get_flow_rate)(void);
};

struct FlowMeter *App_FlowMeter_Create(float (*get_flow_rate)(void))
{
    shared_assert(get_flow_rate != NULL);

    static struct FlowMeter flow_meters[MAX_NUM_OF_FLOWMETERS];
    static size_t           alloc_index = 0;

    shared_assert(alloc_index < MAX_NUM_OF_FLOWMETERS);

    struct FlowMeter *const flow_meter = &flow_meters[alloc_index++];
    flow_meter->get_flow_rate          = get_flow_rate;

    return flow_meter;
}

float App_FlowMeter_ReadFlowRate(struct FlowMeter *flow_meter)
{
    return flow_meter->get_flow_rate();
}
