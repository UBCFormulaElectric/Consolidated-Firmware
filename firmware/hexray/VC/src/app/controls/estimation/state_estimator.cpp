#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>

#include "controls/controls_debug.h"
#include "controls/estimation/state_estimator.hpp"
#include "controls/estimation/steering_model.hpp"
#include "controls/params.hpp"
#include "controls/datatypes/pair.hpp"

using namespace app::controls::params;

namespace app::controls::estimation
{
namespace
{
    using namespace state_estimator;

    // Internal vehicle state used so the estimator can get access to other vehicle data
    static VehicleState<tv_real> vehicle_state{};

    using StateInput  = Filter::state_inp_mtx;
    using State       = Filter::state_mtx;
    using StateVector = Filter::N_1;
    using InputVector = Filter::U_1;

    using GpsMeas    = Filter::Measurement<GPS_DIM>;
    using WheelMeas  = Filter::Measurement<WHEEL_DIM>;
    using ZspeedMeas = Filter::Measurement<ZSPEED_DIM>;

    constexpr Eigen::Index VX = 0;
    constexpr Eigen::Index VY = 1;

    // Control inputs
    constexpr Eigen::Index R  = 2;
    constexpr std::size_t  AX = 3;
    constexpr std::size_t  AY = 4;

    // Special indices for control input vector
    constexpr Eigen::Index U_R  = 0;
    constexpr std::size_t  U_AX = 1;
    constexpr std::size_t  U_AY = 2;

    constexpr float ESTIMATOR_DT_S             = 0.01f;                       // Matches the 100 Hz control task.
    constexpr float ESTIMATOR_YAW_INERTIA_KGM2 = CAR_YAW_MOMENT_INERTIA_KGM2; // same yaw inertia as the controller

    [[nodiscard]] Filter::N_N processNoise()
    {
        return Filter::N_1(0.05f, 0.05f).asDiagonal();
    }

    [[nodiscard]] Filter::M_M<GPS_DIM> gpsMeasNoise()
    {
        return Filter::M_1<GPS_DIM>(0.001f, 0.001f).asDiagonal();
    }

    [[nodiscard]] Filter::M_M<WHEEL_DIM> wheelspeedMeasNoise()
    {
        // Variance of each wheel's angular velocity, (rad/s)^2. Large on purpose: the model assumes free rolling, but
        // all four wheels are driven, so under drive/brake torque the measured speed includes slip (~5-10 % near peak).
        return Filter::M_1<WHEEL_DIM>(8.0f, 8.0f, 8.0f, 8.0f).asDiagonal();
    }

    [[nodiscard]] Filter::M_M<ZSPEED_DIM> zeroSpeedMeasNoise()
    {
        // Variance of each wheel's angular velocity, (rad/s)^2. Large on purpose: the model assumes free rolling, but
        // all four wheels are driven, so under drive/brake torque the measured speed includes slip (~5-10 % near peak).
        return Filter::M_1<ZSPEED_DIM>(0.0001f, 0.01f).asDiagonal();
    }

    // Predict Step

    [[nodiscard]] DecimalDual<tv_real> stateTransitionVx(const StateInput &x)
    {
        const DecimalDual<tv_real> &v_x_mps  = x(VX);
        const DecimalDual<tv_real> &v_y_mps  = x(VY);
        const DecimalDual<tv_real> &r_radps  = x(R);
        const DecimalDual<tv_real> &a_x_mps2 = x(AX);

        DecimalDual<tv_real> pred_v_x_mps   = v_x_mps + (ESTIMATOR_DT_S * (a_x_mps2 + (v_y_mps * r_radps)));
        tv_debug_data.ekf_info.pred_v_x_mps = autodiff::val(pred_v_x_mps);

        return pred_v_x_mps;
    }

    [[nodiscard]] DecimalDual<tv_real> stateTransitionVy(const StateInput &x)
    {
        const DecimalDual<tv_real> &v_x_mps  = x(VX);
        const DecimalDual<tv_real> &v_y_mps  = x(VY);
        const DecimalDual<tv_real> &r_radps  = x(R);
        const DecimalDual<tv_real> &a_y_mps2 = x(AY);

        DecimalDual<tv_real> pred_v_y_mps   = v_y_mps + (ESTIMATOR_DT_S * (a_y_mps2 - (v_x_mps * r_radps)));
        tv_debug_data.ekf_info.pred_v_y_mps = autodiff::val(pred_v_y_mps);

        return pred_v_y_mps;
    }

    Filter::PredictStep predict = { { stateTransitionVx, stateTransitionVy } };

    // GPS Update Step

    [[nodiscard]] DecimalDual<tv_real> gpsMeasVx(const State &x)
    {
        return x(VX);
    }

    [[nodiscard]] DecimalDual<tv_real> gpsMeasVy(const State &x)
    {
        return x(VY);
    }

    Filter::UpdateStep<GPS_DIM> gpsUpdate = {
        .h = { { gpsMeasVx, gpsMeasVy } },
        .R = gpsMeasNoise(),
    };

    // Wheelspeed update step

    //
    // Measurement: the four wheel angular velocities [fl, fr, rl, rr] in rad/s (from the inverters).
    // h maps the predicted body velocity to the angular velocity each wheel would have if it rolled without slip:
    //   1. velocity of the wheel centre (rigid body, ISO axes: x forward, y left, yaw rate r positive CCW)
    //        v_x,i = v_x - r * y_i        v_y,i = v_y + r * x_i
    //   2. component along the wheel's rolling direction (rotate by the wheel's steer angle delta_i)
    //        v_long,i = v_x,i * cos(delta_i) + v_y,i * sin(delta_i)
    //   3. angular velocity  omega_i = v_long,i / R_e,i
    // Same convention as VehicleState::v_in_tire_frame().
    // R_e is the effective rolling radius, the same one kappa_update() uses to turn speed back into a wheel speed
    // request. Using the unloaded radius here would bias v_x ~2% high, and the speed controller would chase it.

    constexpr std::array<app::controls::Pair<float>, WHEEL_DIM> WHEEL_POSITIONS = { {
        { DIST_FRONT_AXLE_CG_m, HALF_TRACK_M },  // fl
        { DIST_FRONT_AXLE_CG_m, -HALF_TRACK_M }, // fr
        { -DIST_REAR_AXLE_CG_m, HALF_TRACK_M },  // rl
        { -DIST_REAR_AXLE_CG_m, -HALF_TRACK_M }, // rr
    } };

    // One h function per wheel; WHEEL selects the row (FL, FR, RL, RR).
    template <Wheel WHEEL> [[nodiscard]] DecimalDual<tv_real> wheelspeedMeas(const State &x)
    {
        const DecimalDual<tv_real> &v_x_mps         = x(VX);
        const DecimalDual<tv_real> &v_y_mps         = x(VY);
        const float                 r_radps         = vehicle_state.yaw_rate_radps;
        const float                 wheel_angle_rad = vehicle_state.wheel_angle_rad[WHEEL];

        const auto                &pos_m     = WHEEL_POSITIONS[static_cast<std::size_t>(WHEEL)];
        const DecimalDual<tv_real> v_x_i_mps = v_x_mps - r_radps * pos_m.y;
        const DecimalDual<tv_real> v_y_i_mps = v_y_mps + r_radps * pos_m.x;
        const DecimalDual<tv_real> v_long_i_mps =
            v_x_i_mps * std::cos(wheel_angle_rad) + v_y_i_mps * std::sin(wheel_angle_rad);

        // Same load model and radius as kappa_update(). R_e is held constant for the Jacobian (its v_x dependence is
        // tiny).
        const auto  v_x_val_mps = static_cast<tv_real>(autodiff::val(v_x_mps));
        const auto  fz_N        = vehicle_state.est_Fz_N();
        const float re_m        = static_cast<float>(tire_model.effectiveRollingRadius_m(
            static_cast<float>(fz_N[WHEEL]), static_cast<tv_real>(std::abs(v_x_val_mps))));

        const DecimalDual<tv_real> pred_wheel_omega_radps = v_long_i_mps / re_m;
        tv_debug_data.ekf_info.pred_wheel_omega_radps[static_cast<std::size_t>(WHEEL)] =
            autodiff::val(pred_wheel_omega_radps);
        return pred_wheel_omega_radps;
    }

    Filter::UpdateStep<WHEEL_DIM> wheelspeedUpdate = {
        .h                     = { { wheelspeedMeas<Wheel::FL>, wheelspeedMeas<Wheel::FR>, wheelspeedMeas<Wheel::RL>,
                                     wheelspeedMeas<Wheel::RR> } },
        .R                     = wheelspeedMeasNoise(),
        .innovation_gate_thres = 9.0f,
    };

    // Zero Speed Update Step

    [[nodiscard]] DecimalDual<tv_real> zeroSpeedMeasVx(const State &x)
    {
        return x(VX);
    }

    [[nodiscard]] DecimalDual<tv_real> zeroSpeedMeasVy(const State &x)
    {
        return x(VY);
    }

    Filter::UpdateStep<ZSPEED_DIM> zeroSpeedUpdate = {
        .h = { { zeroSpeedMeasVx, zeroSpeedMeasVy } },
        .R = zeroSpeedMeasNoise(),
    };

    // Zero-speed detection, separately for each axis.
    //   v_x = 0: wheels stopped AND no longitudinal acceleration AND accelerator released. The a_x check rejects a
    //            locked-wheel skid, where the wheels read 0 but the car is still decelerating. The accelerator check
    //            keeps the row off at a launch. The brake pedal is not checked, so the driver does not have to hold the
    //            brake at standstill.
    //   v_y = 0: no lateral acceleration AND no yaw rate, at any speed. With no lateral tire force and no yaw moment
    //            the slip angles are ~0, so v_y is ~0 (e.g. driving or accelerating in a straight line). Rejects the
    //            car sliding sideways, which needs lateral tire force. No pedal check.
    // Thresholds are sized for worst-case sensor noise (wheel speed sigma 0.5 rad/s, acceleration sigma 0.8 m/s^2 with
    // a 0.6 m/s^2 bias) so the checks still pass at standstill. A locked-wheel skid (a_x ~ -8 m/s^2) is still rejected.
    constexpr float    ZERO_SPEED_WHEEL_RADPS = 1.5f; // 3 sigma of wheel speed noise, ~0.35 m/s at the tire
    constexpr float    ZERO_SPEED_ACCEL_MPS2  = 2.0f; // bias + ~1.75 sigma of acceleration noise
    constexpr float    ZERO_SPEED_YAW_RADPS   = 0.1f;
    constexpr float    ZERO_SPEED_MAX_APPS    = 0.02f; // accelerator pedal, 0..1
    constexpr uint32_t ZERO_SPEED_CONFIRM     = 4U;    // consecutive cycles (40 ms at 100 Hz) before the row is used

    uint32_t vx_zero_cycles = 0U;
    uint32_t vy_zero_cycles = 0U;

    [[nodiscard]] bool wheelsStopped()
    {
        const auto &w = vehicle_state.wheel_omega_radps;
        return std::abs(w.fl) < ZERO_SPEED_WHEEL_RADPS && std::abs(w.fr) < ZERO_SPEED_WHEEL_RADPS &&
               std::abs(w.rl) < ZERO_SPEED_WHEEL_RADPS && std::abs(w.rr) < ZERO_SPEED_WHEEL_RADPS;
    }

    // Counts consecutive true cycles; returns true once the condition has held for ZERO_SPEED_CONFIRM cycles.
    [[nodiscard]] bool confirm(uint32_t &cycles, const bool condition)
    {
        cycles = condition ? std::min(cycles + 1U, ZERO_SPEED_CONFIRM) : 0U;
        return cycles >= ZERO_SPEED_CONFIRM;
    }

    // Call once per estimate() cycle. Returns which axes are confirmed at zero velocity: {v_x, v_y}.
    [[nodiscard]] std::array<bool, ZSPEED_DIM> updateZeroSpeed()
    {
        const bool vx_zero = wheelsStopped() && vehicle_state.apps < ZERO_SPEED_MAX_APPS &&
                             std::abs(vehicle_state.a_x_mps2) < ZERO_SPEED_ACCEL_MPS2;
        const bool vy_zero = std::abs(vehicle_state.a_y_mps2) < ZERO_SPEED_ACCEL_MPS2 &&
                             std::abs(vehicle_state.yaw_rate_radps) < ZERO_SPEED_YAW_RADPS;

        return { { confirm(vx_zero_cycles, vx_zero), confirm(vy_zero_cycles, vy_zero) } };
    }

    // all update steps together

    Filter::UpdateSteps updateSteps = { wheelspeedUpdate, gpsUpdate, zeroSpeedUpdate };

    [[nodiscard]] Filter createFilter()
    {
        return Filter(predict, processNoise(), updateSteps);
    }
} // namespace

namespace state_estimator
{
    Filter filter_ = createFilter();
    void   reset_filter()
    {
        filter_        = createFilter();
        vx_zero_cycles = 0U;
        vy_zero_cycles = 0U;
    }

    [[nodiscard]] VehicleState<float> estimate(const Measurements &meas)
    {
        InputVector u = InputVector::Zero();
        u(U_R)        = meas.yaw_rate_radps;
        u(U_AX)       = meas.ax_mps2;
        u(U_AY)       = meas.ay_mps2;

        vehicle_state = {
            .yaw_rate_radps    = meas.yaw_rate_radps,
            .a_x_mps2          = meas.ax_mps2,
            .a_y_mps2          = meas.ay_mps2,
            .apps              = meas.apps,
            .brake             = meas.brake,
            .charge_limit_W    = meas.charge_limit_W,
            .discharge_limit_W = meas.discharge_limit_W,
            .wheel_angle_rad   = app::controls::estimation::steering::wheel_steer_angles(meas.steering_wheel_angle_rad),
            .wheel_omega_radps = meas.wheel_omega_radps,
        };

        const auto wheelspeed_meas = WheelMeas(
            meas.wheel_omega_radps.fl, meas.wheel_omega_radps.fr, meas.wheel_omega_radps.rl, meas.wheel_omega_radps.rr);

        auto gps_meas = GpsMeas::none();
#ifdef TARGET_EMBEDDED
        if (meas.sbg_ekf_mode == app::can_utils::VcEkfStatus::POSITION)
        {
            gps_meas = GpsMeas(meas.gps_vx_mps, meas.gps_vy_mps);
        }
#else
        if (meas.enable_gps)
        {
            gps_meas = GpsMeas(meas.gps_vx_mps, meas.gps_vy_mps);
        }
#endif
        const auto [vx_zero, vy_zero]         = updateZeroSpeed();
        auto zerospeed_meas                   = ZspeedMeas(0.0f, 0.0f);
        zerospeed_meas.valid[0]               = vx_zero;
        zerospeed_meas.valid[1]               = vy_zero;
        tv_debug_data.ekf_info.zero_vx_active = vx_zero ? 1.0 : 0.0;
        tv_debug_data.ekf_info.zero_vy_active = vy_zero ? 1.0 : 0.0;

        Filter::Measurements z = { wheelspeed_meas, gps_meas, zerospeed_meas };

        const StateVector estimated_state  = filter_.estimated_states(u, z);
        tv_debug_data.ekf_info.est_v_x_mps = estimated_state(VX);
        tv_debug_data.ekf_info.est_v_y_mps = estimated_state(VY);

        Covariance P                         = filter_.covariance();
        tv_debug_data.ekf_info.covariance[0] = P(0, 0);
        tv_debug_data.ekf_info.covariance[1] = P(0, 1);
        tv_debug_data.ekf_info.covariance[2] = P(1, 0);
        tv_debug_data.ekf_info.covariance[3] = P(1, 1);

        vehicle_state.v_x_mps = estimated_state(VX);
        vehicle_state.v_y_mps = estimated_state(VY);

        return vehicle_state;
    }

    const Covariance &covariance()
    {
        return filter_.covariance();
    }
} // namespace state_estimator
} // namespace app::controls::estimation
