#pragma once

#include "state_estimation/app_kalman_filter.hpp"

namespace app::tv::estimators::velocity_estimator
{
using namespace state_estimation;

struct VelocityEstimatorInputs
{
    struct
    {
        float ax_mps2 = 0.0f;
        float ay_mps2 = 0.0f;
        float r_rads  = 0.0f;
    } control;
    struct
    {
        float rr_rpm     = 0.0f;
        float rl_rpm     = 0.0f;
        float fr_rpm     = 0.0f;
        float fl_rpm     = 0.0f;
        float ang_fr_rad = 0.0f;
        float ang_fl_rad = 0.0f;
    } wheels;
    struct
    {
        float body_vx_mps = 0.0f;
        float body_vy_mps = 0.0f;
    } gps;
};

static constexpr std::size_t NUM_STATES           = 2;
static constexpr std::size_t NUM_INPUTS           = 3;
static constexpr std::size_t NUM_WS_MEASUREMENTS  = 2;
static constexpr std::size_t NUM_GPS_MEASUREMENTS = 2;
static constexpr std::size_t NUM_WHEELS           = 4;

using VelocityEstimator = ekf<float, NUM_STATES, NUM_INPUTS, NUM_WS_MEASUREMENTS, NUM_GPS_MEASUREMENTS>;

using Input         = VelocityEstimator::U_1;
using WsUpdateStep  = VelocityEstimator::UpdateStep<NUM_WS_MEASUREMENTS>;
using GpsUpdateStep = VelocityEstimator::UpdateStep<NUM_GPS_MEASUREMENTS>;

using WsMeasurement  = Eigen::Matrix<float, NUM_WS_MEASUREMENTS, 1>;
using GpsMeasurement = Eigen::Matrix<float, NUM_GPS_MEASUREMENTS, 1>;

using EkfStateInp     = VelocityEstimator::state_inp_mtx;
using EkfState        = VelocityEstimator::state_mtx;
using Velocity        = VelocityEstimator::N_1;
using ProcessNoiseCov = Eigen::Matrix<float, NUM_STATES, NUM_STATES>;
using WsNoiseCov      = Eigen::Matrix<float, NUM_WS_MEASUREMENTS, NUM_WS_MEASUREMENTS>;
using GpsNoiseCov     = Eigen::Matrix<float, NUM_GPS_MEASUREMENTS, NUM_GPS_MEASUREMENTS>;

namespace detail
{
    Eigen::Matrix<float, NUM_WHEELS, 1>
        computeWheelVx(const Eigen::Matrix<float, NUM_WHEELS, 1> &ws_mps, float cos_fr, float cos_fl, float yaw);
    Eigen::Matrix<float, NUM_WHEELS, 1>
          computeWheelVy(const Eigen::Matrix<float, NUM_WHEELS, 1> &ws_mps, float sin_fr, float sin_fl, float yaw);
    float computeSlipRatio(float vx_wheel, float vx_state);
    std::optional<WsMeasurement> averageValidWheels(
        const Eigen::Matrix<float, NUM_WHEELS, 1> &vx_wheels,
        const Eigen::Matrix<float, NUM_WHEELS, 1> &vy_wheels,
        float                                      vx_state);
} // namespace detail

// TODO: should outside interface use stl arrays or eigen? should we force user to use eigen?
Velocity estimate_body_velocity(const VelocityEstimatorInputs &inputs);
} // namespace app::tv::estimators::velocity_estimator