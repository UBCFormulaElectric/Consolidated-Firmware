#pragma once

#include <cstdint>
#include "app_canUtils.hpp"

using namespace app::can_utils;

namespace app::tv::algo
{

struct Inputs
{
    float apps           = 0.0f;
    float steering       = 0.0f;
    SwitchState  tv_switch     = SwitchState::SWITCH_OFF;
    SwitchState  regen_switch  = SwitchState::SWITCH_OFF;
    SwitchState  launch_switch = SwitchState::SWITCH_OFF;
};

void run(DriveMode mode, const Inputs &inputs);
} // namespace app::tv::algo
