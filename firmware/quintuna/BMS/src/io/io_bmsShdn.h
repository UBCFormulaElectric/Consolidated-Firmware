#pragma once

#include <stdbool.h>

#define BMS_SHDN_NODE_COUNT 2

bool io_bmsShdn_shdn_en_pin_get(void);

bool io_bmsShdn_hv_p_intlck_sns_pin_get(void);
