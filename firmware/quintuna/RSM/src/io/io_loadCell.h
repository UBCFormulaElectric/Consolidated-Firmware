#pragma once
#include <stdbool.h>

#ifdef TARGET_EMBEDDED
#include "hw_adcs.h"
typedef struct
{
    const AdcChannel *src;
} LoadCell;
#else
#include "app_utils.h"
EMPTY_STRUCT(LoadCell);
#endif

bool  io_loadCell_OCSC(const LoadCell *loadCell);
float io_loadCell_getMechanicalLoad(const LoadCell *loadCell);

extern const LoadCell sensor3;
