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

#define BMS_SHDN_NODE_COUNT 2

void io_bmsShdn_init(const BmsShdnConfig *bms_shdn_config);

bool io_bmsShdn_TS_ILCK_OK_get(void);

bool io_bmsShdn_HVD_OK_get(void);
