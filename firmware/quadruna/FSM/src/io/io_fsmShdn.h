#pragma once

#include <stdbool.h>
#include "app_utils.h"

#ifdef TARGET_EMBEDDED
#include "hw_gpio.h"

typedef struct
{
    const int        fsm_num_nodes;
    const Gpio       fsm_shdn_ok_gpio;
} FsmShdnConfig;
#else
EMPTY_STRUCT(FsmShdnConfig);
#endif

void io_fsmShdn_init(const FsmShdnConfig *fsm_shdn_config);

int io_fsm_num_shdn_nodes();

bool io_get_FSM_SHDN_OK();