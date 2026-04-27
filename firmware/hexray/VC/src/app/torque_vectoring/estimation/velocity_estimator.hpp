#pragma once

#include <cstddef>
#include <optional>

#include "state_estimation/app_kalman_filter.hpp"
#include "torque_vectoring/shared_datatypes/pair.hpp"
#include "torque_vectoring/shared_datatypes/wheel_set.hpp"

namespace app::tv::estimators::velocity_estimator
{
using app::tv::shared_datatypes::Pair;
using app::tv::shared_datatypes::wheel_set;

template <Decimal T> struct VelocityEstimatorInputs
{
    struct
    {
        Pair<T> a_body_mps2{};
        T       r_rads = T{ 0 };
    } control;
    wheel_set<T> rpm{};
    wheel_set<T> steer_angles_rad{};
    Pair<T>      v_body_gps_mps{};
};

static constexpr std::size_t NUM_STATES           = 2;
static constexpr std::size_t NUM_INPUTS           = 3;
static constexpr std::size_t NUM_WS_MEASUREMENTS  = 2;
static constexpr std::size_t NUM_GPS_MEASUREMENTS = 2;

template <Decimal T>
using VelocityEstimator =
    app::state_estimation::ekf<T, NUM_STATES, NUM_INPUTS, NUM_WS_MEASUREMENTS, NUM_GPS_MEASUREMENTS>;

template <Decimal T> using Input         = typename VelocityEstimator<T>::U_1;
template <Decimal T> using WsUpdateStep  = typename VelocityEstimator<T>::template UpdateStep<NUM_WS_MEASUREMENTS>;
template <Decimal T> using GpsUpdateStep = typename VelocityEstimator<T>::template UpdateStep<NUM_GPS_MEASUREMENTS>;

template <Decimal T> using WsMeasurement  = Eigen::Matrix<T, NUM_WS_MEASUREMENTS, 1>;
template <Decimal T> using GpsMeasurement = Eigen::Matrix<T, NUM_GPS_MEASUREMENTS, 1>;

template <Decimal T> using EkfStateInp = typename VelocityEstimator<T>::state_inp_mtx;
template <Decimal T> using EkfState    = typename VelocityEstimator<T>::state_mtx;
template <Decimal T> using Velocity    = typename VelocityEstimator<T>::N_1;

template <Decimal T> using ProcessNoiseCov = Eigen::Matrix<T, NUM_STATES, NUM_STATES>;
template <Decimal T> using WsNoiseCov      = Eigen::Matrix<T, NUM_WS_MEASUREMENTS, NUM_WS_MEASUREMENTS>;
template <Decimal T> using GpsNoiseCov     = Eigen::Matrix<T, NUM_GPS_MEASUREMENTS, NUM_GPS_MEASUREMENTS>;

namespace detail
{
    template <Decimal T> wheel_set<T> motorRpmToWheelSpeedMps(const wheel_set<T> &rpm);
    template <Decimal T>
    wheel_set<Pair<T>> computeWheelVelocities(
        const wheel_set<T> &wheel_speeds_mps,
        const wheel_set<T> &steer_angles_rad,
        T                   yaw_rate_radps);
    template <Decimal T> T computeSlipRatio(T vx_wheel, T vx_state);
    template <Decimal T>
    std::optional<WsMeasurement<T>> averageValidWheels(const wheel_set<Pair<T>> &wheel_velocities_mps, T vx_state);
} // namespace detail

template <Decimal T> Pair<T> estimate_body_velocity(const VelocityEstimatorInputs<T> &inputs);
} // namespace app::tv::estimators::velocity_estimator
