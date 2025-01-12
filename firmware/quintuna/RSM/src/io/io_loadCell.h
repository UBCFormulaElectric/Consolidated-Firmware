#pragma once
#include <stdio.h>
#include <stdbool.h>
// #include "hw_adcs.h"

typedef struct
{
    // Adc *src;
    char _unused;
} LoadCell;

bool  is_ocsc(LoadCell *loadCell);
float loadCell_getMechanicalLoad(LoadCell *loadCell);
float voltageToMechancialLoad(LoadCell *loadCell, float voltage);

extern const LoadCell sensor3;
extern const LoadCell sensor4;
