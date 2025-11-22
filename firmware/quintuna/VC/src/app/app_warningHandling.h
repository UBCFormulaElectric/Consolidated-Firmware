#pragma once
#include "stdbool.h"
#include <stdint.h>

// bspd
void app_softwareBspd_init(void);
bool app_warningHandlin_checkSoftwareBspd(float papps_pedal_percentage);
// board warnings
bool app_warningHandlin_boardWarningCheck(void);
