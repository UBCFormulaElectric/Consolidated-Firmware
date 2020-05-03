#include <assert.h>
#include <stddef.h>
#include <stdlib.h>

struct FlowMeter
{
    float (*get_flow_rate)(void);
};

struct FlowMeter *App_FlowMeter_Create(float (*get_flow_rate)(void))
{
    assert(get_flow_rate != NULL);

    struct FlowMeter *flow_meter =
        (struct FlowMeter *)malloc(sizeof(struct FlowMeter));
    assert(flow_meter != NULL);

    flow_meter->get_flow_rate = get_flow_rate;

    return flow_meter;
}

void App_FlowMeter_Destroy(struct FlowMeter *const flow_meter)
{
    free(flow_meter);
}

float App_FlowMeter_GetFlowRate(const struct FlowMeter *const flow_meter)
{
    return flow_meter->get_flow_rate();
}
