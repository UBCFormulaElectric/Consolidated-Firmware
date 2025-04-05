#pragma once

#include <stdbool.h>

/**
 * Initialize the flow meter by configuring PWM input 
 */
void  io_coolant_init();

/**
 * Update the flow meter frequency by reading the timer value.
 */
void  io_coolant_inputCaptureCallback();

/**
 * Check if the flow meter is active. If it is not, set the frequency to 0.
 */
void  io_coolant_checkIfFlowMeterActive();

/**
 * Get the flow rate in liters per minute.
 * @return The flow rate in liters per minute.
 * @note The flow rate is calculated based on the frequency of the PWM signal
 */
float io_coolant_getFlowRate();
