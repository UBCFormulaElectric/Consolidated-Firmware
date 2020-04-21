#include <assert.h>
#include <stddef.h>

#define MAX_NUM_OF_FLOWMETERS 2

struct FlowMeter
{
    float (*get_flow_rate)(void);
    float flow_rate;
};

struct FlowMeter *App_FlowMeter_Create(float (*get_flow_rate)(void))
{
    assert(get_flow_rate != NULL);

    static struct FlowMeter flow_meters[MAX_NUM_OF_FLOWMETERS];
    static size_t           alloc_index = 0;

    assert(alloc_index < MAX_NUM_OF_FLOWMETERS);

    struct FlowMeter *const flow_meter = &flow_meters[alloc_index++];
    flow_meter->get_flow_rate          = get_flow_rate;

    return flow_meter;
}

float App_FlowMeter_GetFlowRate(const struct FlowMeter *const flow_meter)
{
    return flow_meter->flow_rate;
}

void App_FlowMeter_Tick(struct FlowMeter *const flow_meter)
{
    flow_meter->flow_rate = flow_meter->get_flow_rate();
}
