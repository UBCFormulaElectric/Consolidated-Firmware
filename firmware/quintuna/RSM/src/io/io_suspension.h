#pragma once
#include <stdbool.h>

#ifdef TARGET_EMBEDDED
#include "hw_adc.h"


#else
#include "app_utils.h"

#endif

float io_suspension_getrlTravel(void);
float io_suspension_getrrTravel(void);
bool io_suspension_rl_OCSC(void);
bool io_suspension_rr_OCSC(void);
float travelFromVoltage(float voltage);

