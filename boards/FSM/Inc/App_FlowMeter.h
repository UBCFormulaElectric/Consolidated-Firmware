#pragma once

#include <stdlib.h>
#include <main.h>
#include "SharedAssert.h"

struct FlowMeter;

/**
 * Create the flow meter struct to read flow rate from
 * @param get_flow_rate A function pointer to a function that
 * acquires the flow meter flow rate
 * @return A pointer to the created FlowMeter struct
 */
struct FlowMeter *App_FlowMeter_Create(float (*get_flow_rate)(void));

/**
 * Read the flow rate from a liquid flow meter
 * @param flow_meter The flow meter to read flow rate from
 * @return The coolant pump flow rate (L/min)
 */
float App_FlowMeter_ReadFlowRate(struct FlowMeter *flow_meter);
