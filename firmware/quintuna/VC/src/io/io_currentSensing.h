#pragma once
#include <stdbool.h>

extern "C"
{
#include "app_utils.h"
}

#include "hw_gpio.h"
#include "hw_adc.h"

class CurrentSensing
{
    bool  hasAccumulatorFault();
    bool  hasBatteryFault();
    float getAccumulatorCurrent();
    float getBatteryCurrent();
};
