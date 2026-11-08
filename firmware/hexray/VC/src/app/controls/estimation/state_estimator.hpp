#pragma once

#ifdef TARGET_EMBEDDED
#include "app_canUtils.hpp"
#endif

#include "state_estimation/app_kalman_filter.hpp"
#include "controls/controls.hpp"
#include "controls/datatypes/wheel_set.hpp"

namespace app::controls::estimation
{
struct Measurements
{
    // sensor measurements
    // body accelerations
    const float ax_mps2;
    const float ay_mps2;
    const float r_radps;

    // specific to update step
    const wheel_set<float> wheel_omega_radps; // wheel (not motor) speeds
    const float            steering_wheel_angle_rad;
    const float            apps;  // accelerator pedal, 0..1
    const float            brake; // brake pedal, 0..1
    const float            gps_vx_mps;
    const float            gps_vy_mps;
#ifdef TARGET_EMBEDDED
    const app::can_utils::VcEkfStatus sbg_ekf_status;
#else
    bool        enable_gps;
    const float discharge_limit_W;
    const float charge_limit_W;
#endif
};

namespace state_estimator
{
    // Rows of the measurement vectors (separate from the state indices above).
    constexpr std::size_t NUM_STATES = 2;
    constexpr std::size_t NUM_INPUTS = 3;
    constexpr std::size_t GPS_DIM    = 2;
    constexpr std::size_t WHEEL_DIM  = 4;
    constexpr std::size_t ZSPEED_DIM = 2;

    using Filter     = app::state_estimation::ekf<float, NUM_STATES, NUM_INPUTS, WHEEL_DIM, GPS_DIM, ZSPEED_DIM>;
    using Covariance = Filter::N_N;

    [[nodiscard]] VehicleState      estimate(const Measurements &state);
    [[nodiscard]] const Covariance &covariance();
    void                            resetFilter();
    // if you want to reset, just reconstruct the object
}; // namespace state_estimator
} // namespace app::controls::estimation
