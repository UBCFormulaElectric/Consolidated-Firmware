#pragma once

#include "controls/App_ControlLoop.h"

double calculatePiOutputs(
    struct ControllerValues *pi_values,
    double                   ref,
    double                   actual,
    double                   limit,
    double                   comp);
