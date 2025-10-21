#pragma once

#include <stdbool.h>

#define BMS_SHDN_NODE_COUNT 3

bool io_bmsShdn_msd_shdn_sns_pin_get(void);

bool io_bmsShdn_hv_p_intlck_sns_pin_get(void);

bool io_bmsShdn_hv_n_intlck_sns_pin_get(void);