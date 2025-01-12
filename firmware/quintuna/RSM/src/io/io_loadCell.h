#pragma once
#include <stdio.h>
#include <stdbool.h>
#include "hw_adcs.h"

typedef struct
{
    const AdcChannel *src;

} LoadCell;

bool  is_ocsc(const LoadCell *loadCell);
float loadCell_getMechanicalLoad(const LoadCell *loadCell);

extern const LoadCell sensor3;
