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
struct FlowMeter *App_FlowMeter_Create(
    struct FsmCanTxInterface *can_tx,
    float (*get_flow_rate)(void));

/**
 * Read the flow rate from a flow meter
 * @param flow_meter : The flow meter to read the flow rate from
 * @return The flow rate of the flow meter (L/min)
 */
float App_FlowMeter_ReadFlowRate(struct FlowMeter *flow_meter);

/**
 * Get the flow rate from a flow meter
 * @param primary_flow_meter: The primary flow meter to read the flow rate from
 */
void App_FlowMeter_TickPrimary(struct FlowMeter *primary_flow_meter);

/**
 * Get the flow rate from a flow meter
 * @param secondary_flow_meter : The secondary flow meter to read the flow rate
 * from
 */
void App_FlowMeter_TickSecondary(struct FlowMeter *secondary_flow_meter);
