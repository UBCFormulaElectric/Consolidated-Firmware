#pragma once

#include <stdbool.h>

typedef enum {
    PRECHARGE_STATE_RUNNING,
    PRECHARGE_STATE_SUCCESS,
    PRECHARGE_STATE_FAILED,
    PRECHARGE_STATE_FAILED_CRITICAL,
} PrechargeState;

void app_precharge_init(void);
void app_precharge_restart(void);
PrechargeState app_precharge_poll(bool precharge_for_charging);
bool app_precharge_limitExceeded(void);
