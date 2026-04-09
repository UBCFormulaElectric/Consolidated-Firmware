#pragma once

#include "state_estimation/app_kalman_filter.hpp"
#include "torque_vectoring/estimation/dynamics_estimation.hpp"
#include "torque_vectoring/shared_datatypes/datatypes.hpp"

namespace app::tv::estimation
{
struct Measurements
{
    // sensor measurements
    const float ax;
    const float ay;
    const float yaw_rate;

    // driver controls
    const float delta;
    const float apps;

    const datatypes::datatypes::wheel_set<float> omegas;
};

namespace VehicleStateEstimator
{
    using Filter     = app::state_estimation::ekf<float, 4, 4, 2>;
    using Covariance = Filter::N_N;
    [[nodiscard]] shared_datatypes::datatypes::VehicleState estimate(const Measurements &state);
    [[nodiscard]] const Covariance                         &covariance();
    void                                                    reset_filter();
    // if you want to reset, just reconstruct the object
}; // namespace VehicleStateEstimator
} // namespace app::tv::estimation
