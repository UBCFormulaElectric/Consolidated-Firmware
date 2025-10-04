#pragma once 

#define MIN_FLOW_RATE_L_PER_MIN 1.0f
#define MAX_FLOW_RATE_L_PER_MIN 30.0f

#define FLOW_METER_TIME_TO_FAULT 1000U
#define FLOW_METER_TIME_TO_CLEAR 1000U

void app_coolant_init();
void app_coolant_broadcase();