#pragma once
#include <stdbool.h>

#ifdef TARGET_EMBEDDED
#include "hw_adc.h"

typedef struct
{
    const AdcChannel *src;
} Suspension;
#else
#include "app_utils.h"
EMPTY_STRUCT(Suspension);
#endif

float io_suspension_getTravel(const Suspension *suspension);
// bool  io_suspension_OCSC(const Suspension *suspension);
bool io_suspension_rl_OCSC(void);
bool io_suspension_rr_OCSC(void);

extern const Suspension right;
extern const Suspension left;
