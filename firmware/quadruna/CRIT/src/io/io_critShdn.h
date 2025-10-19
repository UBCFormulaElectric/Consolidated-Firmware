#pragma once

#include <stdbool.h>

#ifdef TARGET_EMBEDDED
#include "hw_gpio.h"

typedef struct
{
    const Gpio *cockpit_estop_gpio;
    const Gpio *inertia_sen_ok_gpio;
} CritShdnConfig;
#else
#include "utils.h"
EMPTY_STRUCT(CritShdnConfig);
#endif

#define CRIT_SHDN_NODE_COUNT 2

void io_critShdn_init(const CritShdnConfig *crit_shdn_config);

bool io_critShdn_COCKPIT_ESTOP_OK_get(void);

bool io_critShdn_get_INERTIA_SEN_OK_get(void);
