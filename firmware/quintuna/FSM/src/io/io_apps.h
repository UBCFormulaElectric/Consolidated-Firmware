#pragma once

#include "hw_adc.h"

#ifdef TARGET_EMBEDDED

typedef struct
{
    const AdcChannel *papps;
    const AdcChannel *sapps;
} AppsConfig;

#else
#include "app_utils.h"
EMPTY_STRUCT(AppsConfig)
#endif


