#pragma once

#include <stdbool.h>
#include "app_utils.h"

#ifdef TARGET_EMBEDDED
#include "hw_gpio.h"

typedef struct
{
    const Gpio shdn_sen_ok_gpio;
    const Gpio inertia_sen_ok_gpio;
} CritShdnConfig;
#else
EMPTY_STRUCT(CritShdnConfig);
#endif

#define CritShdnNodeCount 2

void io_critShdn_init(const CritShdnConfig *crit_shdn_config);

bool io_get_SHDN_SEN_OK(void);

bool io_get_INERTIA_SEN_OK(void);
