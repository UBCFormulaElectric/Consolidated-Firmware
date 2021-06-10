#pragma once

#include "controls/App_ControlLoop.h"

struct ControllerValues adaptionGain(
    const struct ControllerValues *const values,
    const double                         is_ref);
