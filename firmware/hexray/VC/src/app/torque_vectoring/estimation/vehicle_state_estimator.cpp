#include <array>
#include <cmath>

#include "vehicle_state_estimator.hpp"
#include "torque_vectoring/estimation/steering_model.hpp"
#include "torque_vectoring/shared_datatypes/constants.hpp"
#include <torque_vectoring/shared_datatypes/pair.hpp>

using namespace app::tv::shared_datatypes::vd_constants;

namespace app::tv::estimation
{
namespace
{
    using namespace VehicleStateEstimator;

    using StateInput  = Filter::state_inp_mtx;
    using State       = Filter::state_mtx;
    using StateVector = Filter::N_1;
    using InputVector = Filter::U_1;

    using MeasGps    = Filter::M_1<GPS_DIM>;
    using MeasWheels = Filter::M_1<WHEEL_DIM>;

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

    constexpr float ESTIMATOR_DT_S        = 0.01f;  // Matches the 100 Hz control task.
    constexpr float ESTIMATOR_YAW_INERTIA = 110.0f; // TODO: Replace with measured Hexray yaw inertia.

    [[nodiscard]] Filter::N_N processNoise()
    {
        return Filter::N_1(0.05f, 0.05f).asDiagonal();
    }

    [[nodiscard]] Filter::M_M<GPS_DIM> gpsMeasNoise()
    {
        return Filter::M_1<GPS_DIM>(0.75f, 0.75f).asDiagonal();
    }

    [[nodiscard]] Filter::M_M<WHEEL_DIM> wheelspeedMeasNoise()
    {
        // Variance of each wheel's angular velocity, (rad/s)^2. Large on purpose: the model assumes free rolling, but
        // all four wheels are driven, so under drive/brake torque the measured speed includes slip (~5-10 % near peak).
        return Filter::M_1<WHEEL_DIM>(4.0f, 4.0f, 4.0f, 4.0f).asDiagonal();
    }

    // Predict Step

    [[nodiscard]] autodiff::dual stateTransitionVx(const StateInput &x)
    {
        const autodiff::dual &v_x = x(VX);
        const autodiff::dual &v_y = x(VY);
        const autodiff::dual &r   = x(R);
        const autodiff::dual &a_x = x(AX);

        return v_x + (ESTIMATOR_DT_S * (a_x + (v_y * r)));
    }

    [[nodiscard]] autodiff::dual stateTransitionVy(const StateInput &x)
    {
        const autodiff::dual &v_x = x(VX);
        const autodiff::dual &v_y = x(VY);
        const autodiff::dual &r   = x(R);
        const autodiff::dual &a_y = x(AY);

        return v_y + (ESTIMATOR_DT_S * (a_y - (v_x * r)));
    }

    Filter::PredictStep predict = { stateTransitionVx, stateTransitionVy };

    // GPS Update Step

    [[nodiscard]] autodiff::dual gpsMeasVx(const State &x)
    {
        return x(VX);
    }

    [[nodiscard]] autodiff::dual gpsMeasVy(const State &x)
    {
        return x(VY);
    }

    Filter::UpdateStep<GPS_DIM> gpsUpdate = { .h = { gpsMeasVx, gpsMeasVy }, .R = gpsMeasNoise() };

    // Wheelspeed update step

    //
    // Measurement: the four wheel angular velocities [fl, fr, rl, rr] in rad/s (from the inverters).
    // h maps the predicted body velocity to the angular velocity each wheel would have if it rolled without slip:
    //   1. velocity of the wheel centre (rigid body, ISO axes: x forward, y left, yaw rate r positive CCW)
    //        v_x,i = v_x - r * y_i        v_y,i = v_y + r * x_i
    //   2. component along the wheel's rolling direction (rotate by the wheel's steer angle delta_i)
    //        v_long,i = v_x,i * cos(delta_i) + v_y,i * sin(delta_i)
    //   3. angular velocity  omega_i = v_long,i / R_wheel
    // Same convention as VehicleState::v_in_tire_frame().
    // TODO: remove this somehow to make this estimator stateless
    app::tv::shared_datatypes::wheel_set<float> delta_rad{};

    constexpr std::array<app::tv::shared_datatypes::Pair<float>, WHEEL_DIM> WHEEL_POSITIONS = { {
        { DIST_FRONT_AXLE_CG_m, HALF_TRACK_M },  // fl
        { DIST_FRONT_AXLE_CG_m, -HALF_TRACK_M }, // fr
        { -DIST_REAR_AXLE_CG_m, HALF_TRACK_M },  // rl
        { -DIST_REAR_AXLE_CG_m, -HALF_TRACK_M }, // rr
    } };

    // One h function per wheel; WHEEL selects the row (FL, FR, RL, RR).
    template <shared_datatypes::Wheel WHEEL> [[nodiscard]] autodiff::dual wheelspeedMeas(const State &x)
    {
        const autodiff::dual &v_x = x(VX);
        const autodiff::dual &v_y = x(VY);
        const autodiff::dual &r   = x(R);
        const float           d   = delta_rad[WHEEL];

        const auto          &pos_m    = WHEEL_POSITIONS[static_cast<std::size_t>(WHEEL)];
        const autodiff::dual v_x_i    = v_x - r * pos_m.y;
        const autodiff::dual v_y_i    = v_y + r * pos_m.x;
        const autodiff::dual v_long_i = v_x_i * std::cos(d) + v_y_i * std::sin(d);

        return v_long_i / WHEEL_RADIUS_M;
    }

    Filter::UpdateStep<WHEEL_DIM> wheelspeedUpdate = {
        .h = { wheelspeedMeas<shared_datatypes::Wheel::FL>, wheelspeedMeas<shared_datatypes::Wheel::FR>,
               wheelspeedMeas<shared_datatypes::Wheel::RL>, wheelspeedMeas<shared_datatypes::Wheel::RR> },
        .R = wheelspeedMeasNoise(),
    };

    // all update steps together

    Filter::UpdateSteps updateSteps = {
        wheelspeedUpdate,
        gpsUpdate,
    };

    [[nodiscard]] Filter createFilter()
    {
        return Filter(predict, processNoise(), updateSteps);
    }
} // namespace

namespace VehicleStateEstimator
{
    Filter filter_ = createFilter();
    void   reset_filter()
    {
        filter_ = createFilter();
    }

    [[nodiscard]] shared_datatypes::VehicleState<float> estimate(const Measurements &state)
    {
        InputVector u = InputVector::Zero();
        u(U_R)        = state.yaw_rate;
        u(U_AX)       = state.ax;
        u(U_AY)       = state.ay;

        delta_rad = app::tv::estimation::steering::wheel_steer_angles(state.delta);

        Filter::Measurements z = {
            // wheelspeed measurement
            Filter::M_1<WHEEL_DIM>(state.omegas.fl, state.omegas.fr, state.omegas.rl, state.omegas.rr),
            // gps measurement
            // Filter::M_1<GPS_DIM>(state.gps_vx_mps, state.gps_vy_mps),
            std::nullopt, // temporarily disabling for testing
        };

        const StateVector estimated_state = filter_.estimated_states(u, z);

        // outputs_.yaw_moment_nm = estimated_state(static_cast<Eigen::Index>(MZ));
        return {
            .v_x_mps        = estimated_state(VX),
            .v_y_mps        = estimated_state(VY),
            .yaw_rate_radps = state.yaw_rate,
            .a_x_mps2       = state.ax,
            .a_y_mps2       = state.ay,
            .delta          = delta_rad,
        };
    }

    const Covariance &covariance()
    {
        return filter_.covariance();
    }
} // namespace VehicleStateEstimator
} // namespace app::tv::estimation
