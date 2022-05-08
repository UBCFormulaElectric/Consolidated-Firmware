#pragma once

#include <stdbool.h>
#include "App_FsmWorld.h"

/**
 * Check if the primary flow rate is below its minimum threshold
 * @param world The world to get the primary flow rate in-range check
 * @return true if the primary flow rate (L/min) is below its minimum flow rate
 * threshold, else false
 */
bool App_FlowMetersSignals_IsPrimaryFlowRateBelowThreshold(struct FsmWorld *world);

/**
 * Check if the primary flow rate is in range
 * @param world The world to get the primary flow rate in-range check
 * @return true if the primary flow rate (L/min) is in range, else false
 */
bool App_FlowMetersSignals_IsPrimaryFlowRateInRange(struct FsmWorld *world);

/**
 * Check if the secondary flow rate is below its minimum threshold
 * @param world The world to get the secondary flow rate in-range check
 * @return true if the secondary flow rate (L/min) is in range, else false
 */
bool App_FlowMetersSignals_IsSecondaryFlowRateBelowThreshold(struct FsmWorld *world);

/**
 * Check if the secondary flow rate is in range
 * @param world The world to get the secondary flow rate in-range check
 * @return true if the secondary flow rate (L/min) is in range, else false
 */
bool App_FlowMetersSignals_IsSecondaryFlowRateInRange(struct FsmWorld *world);

/**
 * The callback function for the primary flow rate signal in the given world
 * @param world The world to get the CAN tx interface from
 */
void App_FlowMetersSignals_PrimaryFlowRateBelowThresholdCallback(struct FsmWorld *const world);

/**
 * The callback function for the secondary flow rate signal in the given world
 * @param world The world to get the CAN tx interface from
 */
void App_FlowMetersSignals_SecondaryFlowRateBelowThresholdCallback(struct FsmWorld *const world);
