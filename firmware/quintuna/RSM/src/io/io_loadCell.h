#pragma once
#include <stdio.h>
#include <stdbool.h>
#include "hw_adcs.h"

typedef struct
{
    const AdcChannel *src;

} LoadCell;

bool  io_loadCell_OCSC(const LoadCell *loadCell);
float io_loadCell_getMechanicalLoad(const LoadCell *loadCell);

extern const LoadCell sensor3;
