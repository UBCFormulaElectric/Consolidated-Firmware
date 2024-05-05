#pragma once

/**
 * Set the flow rate_lpm of the pump.
 * @param rate_lpm Given rate_lpm between 0 and 2000L/m
 */
void app_pumpControl_setFlowRate(float rate_lpm);

/**
 * Get the current flow rate of the pump.
 * @return The current flow rate.
 */
float app_pumpControl_getFlowRate(void);
