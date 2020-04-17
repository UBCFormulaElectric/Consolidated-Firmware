#pragma once

struct FlowMeter;

/**
 * Create the flow meter to read flow rate from
 * @param can_tx : A CanTx interface for the FSM
 * @param get_flow_rate : A function pointer to a function that
 * acquires the flow rate from the flow meter
 * @return A pointer to the created flow meter, whose ownership is given to the
 * caller
 */
struct FlowMeter *App_FlowMeter_Create(float (*get_flow_rate)(void));

/**
 * Read the flow rate from a flow meter
 * @param flow_meter : The flow meter to read the flow rate from
 * @return The flow rate of the flow meter (L/min)
 */
float App_FlowMeter_ReadFlowRate(struct FlowMeter *flow_meter);

/**
 * Update flow rate for the flow meter
 * @param flow_meter: The flow meter to update the flow rate for
 */
void App_FlowMeter_Tick(struct FlowMeter *flow_meter);
