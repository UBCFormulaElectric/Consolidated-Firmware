#include <math.h>
#include <assert.h>
#include <stddef.h>
#include <stdlib.h>

struct FlowMeter
{
    float (*get_flow_rate)(void);
    float flow_rate;
};

struct FlowMeter *App_FlowMeter_Create(float (*get_flow_rate)(void))
{
    assert(get_flow_rate != NULL);

    struct FlowMeter *flow_meter =
        (struct FlowMeter *)malloc(sizeof(struct FlowMeter));
    assert(flow_meter != NULL);

    flow_meter->get_flow_rate = get_flow_rate;
    flow_meter->flow_rate     = NAN;

    return flow_meter;
}

void App_FlowMeter_Destroy(struct FlowMeter *const flow_meter)
{
    free(flow_meter);
}

float App_FlowMeter_GetFlowRate(const struct FlowMeter *const flow_meter)
{
    return flow_meter->flow_rate;
}

void App_FlowMeter_Tick(struct FlowMeter *const flow_meter)
{
    flow_meter->flow_rate = flow_meter->get_flow_rate();
}
