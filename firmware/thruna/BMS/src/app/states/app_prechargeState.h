#pragma once

#include "app_stateMachine.h"
#include "states/app_initState.h"
#include "states/app_driveState.h"
#include "states/app_chargeState.h"
#include "states/app_faultState.h"
#include "states/app_allStates.h"
#include "app_units.h"

#define NUM_OF_INVERTERS (2U)
#define PRECHARGE_RESISTOR_OHMS (500U)
#define INV_CAPACITANCE_F (0.280e-3f)
#define PRECHARGE_CAPACITANCE_F (INV_CAPACITANCE_F * NUM_OF_INVERTERS)
#define PRECHARGE_RC_MS (S_TO_MS(PRECHARGE_RESISTOR_OHMS * PRECHARGE_CAPACITANCE_F))

// 2.7RC corresponds to time to reach ~93% charged
#define PRECHARGE_COMPLETION_MS ((float)PRECHARGE_RC_MS * 2.7f)
#define PRECHARGE_COMPLETION_LOWER_BOUND ((uint32_t)(PRECHARGE_COMPLETION_MS * 0.5f))
#define PRECHARGE_COMPLETION_UPPER_BOUND ((uint32_t)(PRECHARGE_COMPLETION_MS * 3.0f))

/**
 * Get a pointer to the Pre-Charge State.
 * @return A pointer to the Pre-Charge State. THIS SHOULD NOT BE MODIFIED.
 */
const State *app_prechargeState_get(void);
