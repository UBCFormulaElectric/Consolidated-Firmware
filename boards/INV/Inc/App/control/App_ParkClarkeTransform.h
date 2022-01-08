#pragma once

#include "control/App_ControlLoop.h"

struct PhaseValues parkClarkeTransform(
    const struct DqsValues *const dqs_voltages,
    float                         theta);
