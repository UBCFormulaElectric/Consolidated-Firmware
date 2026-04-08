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
    struct Outputs
    {
        datatypes::datatypes::VehicleState vehicle_state{};
        float                              yaw_moment_nm = 0.0f;
    };

    using Filter     = app::state_estimation::ekf<float, 4, 4, 2>;
    using Covariance = Filter::N_N;

    VehicleStateEstimator();

    [[nodiscard]] Outputs           estimate(const shared_datatypes::datatypes::VehicleState &state);
    [[nodiscard]] const Outputs    &outputs() const;
    [[nodiscard]] const Covariance &covariance() const;
    void                            reset();

  private:
    [[nodiscard]] static Filter createFilter();

    Filter          filter_;
    vehicleDynamics dynamics_estimator_{};
    Outputs         outputs_{};
};
} // namespace app::tv::estimation
