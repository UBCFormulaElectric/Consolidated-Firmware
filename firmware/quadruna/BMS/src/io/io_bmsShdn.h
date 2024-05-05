#pragma once

#include <stdbool.h>
#include "app_utils.h"

#ifdef TARGET_EMBEDDED
#include "hw_gpio.h"

typedef struct
{
    const Gpio ts_ilck_ok_gpio;
    const Gpio hvd_ok_gpio;
} BmsShdnConfig;
#else
EMPTY_STRUCT(BmsShdnConfig);
#endif

#define BmsShdnNodeCount 2

void io_bmsShdn_init(const BmsShdnConfig *bms_shdn_config);

bool io_get_TS_ILCK_OK(void);

bool io_get_HVD_OK(void);
