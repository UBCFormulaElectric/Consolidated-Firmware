#pragma once

#include <stdlib.h>
#include <main.h>
#include "SharedAssert.h"

struct FlowMeter;

/**
 * Function used to create the flow meter struct
 * @param get_flow_rate is a function pointer to a function that
 * acquires the flow meter flow rate
 * @return
 */
struct FlowMeter *App_FlowMeter_Create(float (*get_flow_rate)(void));

/**
 * Function reads the flow rate from a liquid flow meter
 * @param Pointer to the flow meter struct
 */
float App_FlowMeter_ReadFlowRate(struct FlowMeter *flow_meter);
