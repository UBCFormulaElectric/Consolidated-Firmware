#pragma once

#include "controls/App_ControlLoop.h"

struct ControllerValues adaptionGain(
    const struct ControllerValues *const values,
    const float                          is_ref);
