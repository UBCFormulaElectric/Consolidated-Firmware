#pragma once

#include <stdbool.h>
#include "app_utils.h"

#ifdef TARGET_EMBEDDED
#include "hw_gpio.h"

typedef struct
{
    const int  crit_num_nodes;
    const Gpio shdn_sen_ok_gpio;
    const Gpio inertia_sen_ok_gpio;
} CritShdnConfig;
#else
EMPTY_STRUCT(CritShdnConfig);
#endif

void io_critShdn_init(const CritShdnConfig *crit_shdn_config);

int io_crit_num_shdn_nodes();

bool io_get_SHDN_SEN_OK();

bool io_get_INERTIA_SEN_OK();