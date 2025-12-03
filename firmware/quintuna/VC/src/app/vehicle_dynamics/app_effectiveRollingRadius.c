#include "app_effectiveRollingRadius.h"
#include "app_vehicleDynamicsConstants.h"
#include <math.h>

float app_effectiveRollingRadius_compute(const float v_x_body, const float v_y_body, const float steer_angle_deg, const float frequency_hz)
{
    // Check for invalid frequency to avoid division by zero
    if (frequency_hz <= 0.0f)
    {
        return 0.0f;
    }

    // Convert steering angle from degrees to radians
    const float steer_angle_rad = DEG_TO_RAD(steer_angle_deg);

    // Calculate wheel velocity in x-direction (wheel frame)
    // V_x,i^wheel = V_x,i^body cos(δ_i) + V_y,i^body sin(δ_i)
    const float v_x_wheel = v_x_body * cosf(steer_angle_rad) + v_y_body * sinf(steer_angle_rad);

    // Calculate angular velocity from frequency: ω = 2πf
    const float omega_rads = 2.0f * M_PI_F * frequency_hz;

    // Calculate effective rolling radius: re = Vx / ω
    // Add small epsilon to avoid division by zero (though we already check frequency)
    const float effective_radius = v_x_wheel / (omega_rads + SMALL_EPSILON);

    return effective_radius;
}


