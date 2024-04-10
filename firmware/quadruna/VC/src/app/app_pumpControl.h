#pragma once

/**
 * Set the flow rate of the pump.
 * @param rate Given rate between 0 and 2000L/H
 */
void app_pumpControl_setFlowRate(float rate);

/**
 * Get the current flow rate of the pump.
 * @return The current flow rate.
 */
float app_pumpControl_getFlowRate();