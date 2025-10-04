#pragma once

/*
* Initialize the flow meter by configuring PWM input
*/
void io_coolant_init();

/*
* Check if the flow meter is active. If not, set frequency to 0. 
*/
void io_coolant_checkIfFlowMeterActive();

/*
* return the flow rate in liters per minute.
* NOTE: flow rate is calculated based on frequency of the PWM signal
*/
float io_coolant_getFlowRate();