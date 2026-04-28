#pragma once

#include <cstdint>
#include "app_canUtils.hpp"

using namespace app::can_utils;

namespace app::tv::algo
{

struct SensorStatus
{
    bool imu_ok      = false;
    bool steering_ok = false;
    bool gps_ok      = false;

    bool allSensorsOk() { return imu_ok && steering_ok && gps_ok; }
};

struct Inputs
{
    float       apps      = 0.0f;
    float       steering  = 0.0f;
    SwitchState regen_sw  = SwitchState::SWITCH_OFF;
    SwitchState launch_sw = SwitchState::SWITCH_OFF;
    // Various data
};

void send_torque(float fl, float fr, float rl, float rr);
void send_speed(float fl, float fr, float rl, float rr);
void run(float apps);
} // namespace app::tv::algo
