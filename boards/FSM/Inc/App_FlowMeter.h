#pragma once

#define TIM2_FREQUENCY 72000000

struct FlowMeter;

/**
 * Allocate and init flow meter, get flow rate
 * @param get_flow_rate
 * @return
 */
struct FlowMeter *App_FlowMeter_Create(float (*get_flow_rate)(void));


void dummy ( struct FlowMeter * ptr);
