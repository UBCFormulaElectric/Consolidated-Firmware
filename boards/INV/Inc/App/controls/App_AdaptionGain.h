#pragma once

#include "controls/App_ControlLoop.h"

ControllerValues
    adaptionGain(const ControllerValues *const values, const double is_ref);
