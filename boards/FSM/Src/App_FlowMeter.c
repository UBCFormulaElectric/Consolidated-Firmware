#include "App_FlowMeter.h"
#include "stdlib.h"
#include "SharedAssert.h"
#include <stm32f3xx_hal.h>
// Currently configured to TIM2; Channel 3

struct FlowMeter
{
    float (*get_flow_rate)(void);
};

struct FlowMeter *App_FlowMeter_Create(float (*get_flow_rate)(void))
{
    shared_assert(get_flow_rate != NULL);

    // Initialize FlowMeter
    static struct FlowMeter FlowMeter;

    // Initialize FlowMeter Ptr
    struct FlowMeter *const flowmeter = &FlowMeter;

    flowmeter->get_flow_rate = get_flow_rate;

    return flowmeter;
}

void dummy(struct FlowMeter *ptr)
{
    const float frequency = ptr->get_flow_rate();

    UNUSED(frequency);
}
