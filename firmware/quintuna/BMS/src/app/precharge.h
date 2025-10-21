#pragma once

#include <stdbool.h>
#include "app/units.h"

#define NUM_OF_INVERTERS (4U)
#define PRECHARGE_RESISTOR_OHMS (1.5e3f + 1.5e3f)
#define INV_CAPACITANCE_F (75e-6f)
#define PRECHARGE_CAPACITANCE_F (INV_CAPACITANCE_F * NUM_OF_INVERTERS)
#define PRECHARGE_RC_MS (S_TO_MS(PRECHARGE_RESISTOR_OHMS * PRECHARGE_CAPACITANCE_F))

// 2.7RC corresponds to time to reach ~93% charged
#define PRECHARGE_COMPLETION_MS ((float)PRECHARGE_RC_MS * 2.7f)
#define PRECHARGE_COMPLETION_LOWER_BOUND ((uint32_t)(PRECHARGE_COMPLETION_MS * 0.5f))
#define PRECHARGE_COMPLETION_UPPER_BOUND ((uint32_t)(PRECHARGE_COMPLETION_MS * 3.0f))

// When TS reaches this threshold of ACC, AIR+ engages, and precharge is disabled, the voltage must be at least 90%
// EV.6.6.1
#define PRECHARGE_ACC_V_THRESHOLD (0.90f)
#define MAX_PRECHARGE_ATTEMPTS 3U

typedef enum
{
    PRECHARGE_STATE_RUNNING,
    PRECHARGE_STATE_SUCCESS,
    PRECHARGE_STATE_FAILED,
    PRECHARGE_STATE_FAILED_CRITICAL,
    PRECHARGE_STATE_COOLDOWN,
} PrechargeState;

void           app_precharge_init(void);
void           app_precharge_restart(void);
PrechargeState app_precharge_poll(bool precharge_for_charging);
bool           app_precharge_limitExceeded(void);
