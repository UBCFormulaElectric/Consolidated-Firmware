#pragma once

#include <stdbool.h>

// The datasheet (https://www.adafruit.com/product/828) for the flow meter
// indicates a working flow rate between 1L/min to 30L/min
#define MIN_FLOW_RATE_L_PER_MIN 1.0f
#define MAX_FLOW_RATE_L_PER_MIN 30.0f

#define FLOW_METER_TIME_TO_FAULT (1000U)
#define FLOW_METER_TIME_TO_CLEAR (1000U)

void app_coolant_init(void);
void app_coolant_destroy(void);
void app_coolant_broadcast(void);
