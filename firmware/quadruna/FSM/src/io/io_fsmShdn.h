#pragma once

#include <stdbool.h>
#include "app_utils.h"

#ifdef TARGET_EMBEDDED
#include "hw_gpio.h"

typedef struct
{
    const Gpio fsm_shdn_ok_gpio;
} FsmShdnConfig;
#else
EMPTY_STRUCT(FsmShdnConfig);
#endif

#define FsmShdnNodeCount 1

void io_fsmShdn_init(const FsmShdnConfig *fsm_shdn_config);

bool io_get_FSM_SHDN_OK(void);
