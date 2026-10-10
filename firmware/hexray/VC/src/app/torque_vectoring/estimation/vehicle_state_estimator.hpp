#pragma once

#ifdef TARGET_EMBEDDED
#include "app_canUtils.hpp"
#include "app_sbgEllipse.hpp"
#endif

#include "state_estimation/app_kalman_filter.hpp"
#include "torque_vectoring/torque_vectoring.hpp"
#include "torque_vectoring/shared_datatypes/wheel_set.hpp"

namespace app::tv::estimation
{
struct Measurements
{
    // sensor measurements
    // body accelerations
    const float ax_mps2;
    const float ay_mps2;
    const float yaw_rate_radps;

    // specific to update step
    const shared_datatypes::wheel_set<float> omegas; // wheel speeds
    const float                              delta;
    const float                              gps_vx_mps;
    const float                              gps_vy_mps;
#ifdef TARGET_EMBEDDED
    const app::can_utils::VcEkfStatus sbg_ekf_mode;
#endif
};

namespace VehicleStateEstimator
{
    // Rows of the measurement vectors (separate from the state indices above).
    constexpr std::size_t NUM_STATES = 2;
    constexpr std::size_t NUM_INPUTS = 3;
    constexpr std::size_t GPS_DIM    = 2;
    constexpr std::size_t WHEEL_DIM  = 4;
    constexpr std::size_t ZSPEED_DIM = 2;

    using Filter     = app::state_estimation::ekf<tv_real, NUM_STATES, NUM_INPUTS, WHEEL_DIM, GPS_DIM, ZSPEED_DIM>;
    using Covariance = Filter::N_N;

    [[nodiscard]] shared_datatypes::VehicleState<float> estimate(const Measurements &state);
    [[nodiscard]] const Covariance                     &covariance();
    void                                                reset_filter();
    // if you want to reset, just reconstruct the object
}; // namespace VehicleStateEstimator
} // namespace app::tv::estimation
