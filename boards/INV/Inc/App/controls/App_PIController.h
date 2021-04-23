#pragma once

#include "controls/App_ControlLoop.h"

double calculatePiOutputs(
    ControllerValues *pi_values,
    double            ref,
    double            actual,
    double            limit,
    double            comp);
