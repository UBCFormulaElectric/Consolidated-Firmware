#pragma once

#include <Eigen/Dense>

#include "state_estimation/app_kalman_filter.hpp"
#include "torque_vectoring/estimation/dynamics_estimation.hpp"
#include "torque_vectoring/shared_datatypes/datatypes.hpp"

namespace app::tv::estimation
{
class VehicleStateEstimator
{
  public:
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

    using Filter     = app::state_estimation::ekf<float, 4, 4, 2>;
    using Covariance = Filter::N_N;

    // constexpr VehicleStateEstimator() : filter_(createFilter()) {}
    constexpr VehicleStateEstimator() : filter_(createFilter()) {}

    [[nodiscard]] shared_datatypes::datatypes::VehicleState estimate(const Measurements &state) const;
    [[nodiscard]] const Covariance                         &covariance() const;
    // if you want to reset, just reconstruct the object

  private:
    [[nodiscard]] static constexpr Filter createFilter();
    const Filter                          filter_;
};
} // namespace app::tv::estimation
