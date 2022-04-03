#pragma once

#include "control/App_ControlLoop.h"

void calculatePiOutputs(
    struct ControllerValues *pi_values,
    float                    ref,
    float                    actual,
    float                    limit,
    float                    comp);
