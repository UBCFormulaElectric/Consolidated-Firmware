#pragma once
#include <stdbool.h>

#ifdef TARGET_EMBEDDED
#include "hw_adcs.h"

typedef struct
{
    const AdcChannel *src;
} Suspension;
#else
#include "app_utils.h"
EMPTY_STRUCT(Suspension);
#endif

float io_suspension_getTravel(const Suspension *suspension);
bool  io_suspension_OCSC(const Suspension *suspension);

extern const Suspension right;
extern const Suspension left;
