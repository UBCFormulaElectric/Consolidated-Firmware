#include "App_FlowMeter.h"

struct FlowMeter
{
    float (*get_flow_rate)(void);
};

struct FlowMeter *App_FlowMeter_Create(float (*get_flow_rate)(void))
{
    shared_assert(get_flow_rate != NULL);

    static struct FlowMeter FlowMeter;
    struct FlowMeter *const flowmeter = &FlowMeter;

    // Initialize struct content
    flowmeter->get_flow_rate = get_flow_rate;

    return flowmeter;
}

float App_FlowMeter_ReadFlowRate(struct FlowMeter *flow_meter)
{
    return flm->get_flow_rate();
}
