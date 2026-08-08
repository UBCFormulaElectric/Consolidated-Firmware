#pragma once
#include <stdint.h>
namespace CurrentLimiter
{

static constexpr uint8_t N_WHEELS = 4;

struct WheelInput
{
    float speed_rpm;      // Speed command from slip solver
    float temp_C;         // Motor temperature (from inverter feedback)
    float current_meas;   // Actual measured line current (from inverter feedback)
    float dc_bus_voltage; // Measured DC bus voltage
};

struct WheelOutput
{
    float speed_cmd_rpm;       // AMK_TargetVelocity — unchanged from input
    float torque_limit_pos_Nm; // AMK_TorqueLimitPositiv (ID82)
    float torque_limit_neg_Nm; // AMK_TorqueLimitNegativ (ID83)
    float predicted_current;   // For telemetry/logging
};

struct Config
{
    float I_max_BMS_discharge; // Discharging current limit from BMS
    float I_max_BMS_charge;    // Charging current limit from BMS
    float Kp_feedback;         // Proportional gain for feedback trim
    float motor_max_torque;    // Max torque used by default
    bool  regen_on = true;     // Regen Flag
};

// Call once at startup
void init(const Config &cfg);

// Computes torque limits for all 4 wheels preserving yaw moment ratios. Call every cycle
void update(const WheelInput inputs[N_WHEELS], WheelOutput outputs[N_WHEELS], float bms_dc_current);

// Returns the alpha scaling factor from the last update() call.
// Useful for telemetry — alpha < 1.0 means limiting is active.
float get_last_alpha_charge();
float get_last_alpha_discharge();

} // namespace CurrentLimiter
