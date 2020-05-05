#pragma once

struct FlowMeter;

/**
 * Create the flow meter to read flow rate from
 * @param get_flow_rate : A function pointer to a function that
 * acquires the flow rate from the flow meter
 * @return A pointer to the created flow meter, whose ownership is given to the
 * caller
 */
struct FlowMeter *App_FlowMeter_Create(float (*get_flow_rate)(void));

/**
 * Destroy the given flow meter object, freeing any memory that it uses
 * @param flow_meter The flow meter to destroy
 */
void App_FlowMeter_Destroy(struct FlowMeter *flow_meter);

/**
 * Get the flow rate from a flow meter
 * @param flow_meter: The flow meter to get the flow rate from
 * @return The flow rate of the flow meter (L/min)
 */
float App_FlowMeter_GetFlowRate(const struct FlowMeter *flow_meter);
