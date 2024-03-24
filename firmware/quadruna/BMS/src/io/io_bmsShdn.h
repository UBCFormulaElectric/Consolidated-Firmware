#pragma once

#include <stdbool.h>
#include "app_utils.h"

#ifdef TARGET_EMBEDDED
#include "hw_gpio.h"

typedef struct
{
    const int        bms_num_nodes;
    const Gpio       ts_ilck_ok_gpio;
    const Gpio       hvd_ok_gpio;
} BmsShdnConfig;
#else
EMPTY_STRUCT(BmsShdnConfig);
#endif

void io_bmsShdn_init(const BmsShdnConfig *bms_shdn_config);

int io_bms_num_shdn_nodes();

bool io_get_TS_ILCK_OK();

bool io_get_HVD_OK();