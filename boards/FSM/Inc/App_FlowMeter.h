#pragma once

#include <stdlib.h>
#include "SharedAssert.h"

#define TIM2_FREQUENCY 36000000

struct FlowMeter;

/**
 * Allocate and init flow metenujabesr, get flow rate
 * @param get_flow_rate
 * @return
 */
struct FlowMeter *App_FlowMeter_Create(float (*get_flow_rate)(void));

/**
 * Read Flow Rate from Flow Meter
 *
 */
float App_FlowMeter_GetFlowRate(struct FlowMeter *flow_meter);
