#include "vehicle_state_estimator.hpp"

#include <array>
#include <cmath>

#include "torque_vectoring/shared_datatypes/constants.hpp"

using namespace app::tv::shared_datatypes::vd_constants;

namespace app::tv::estimation
{
namespace
{
    using Filter      = VehicleStateEstimator::Filter;
    using StateInput  = Filter::state_inp_mtx;
    using State       = Filter::state_mtx;
    using StateVector = Filter::N_1;
    using InputVector = Filter::U_1;
    using Measurement = Filter::M_1;

    constexpr std::size_t VX = 0;
    constexpr std::size_t VY = 1;
    constexpr std::size_t R  = 2;
    constexpr std::size_t MZ = 3;

    constexpr std::size_t AX = 0;
    constexpr std::size_t AY = 1;

    constexpr float ESTIMATOR_DT_S        = 0.01f;  // Matches the 100 Hz control task.
    constexpr float ESTIMATOR_YAW_INERTIA = 110.0f; // TODO: Replace with measured Hexray yaw inertia.

    [[nodiscard]] constexpr autodiff::dual stateTransitionVx(const StateInput &x)
    {
        const autodiff::dual &v_x = x(static_cast<Eigen::Index>(VX));
        const autodiff::dual &v_y = x(static_cast<Eigen::Index>(VY));
        const autodiff::dual &r   = x(static_cast<Eigen::Index>(R));
        const autodiff::dual &a_x = x(static_cast<Eigen::Index>(4 + AX));

        return v_x + (ESTIMATOR_DT_S * (a_x + (v_y * r)));
    }

    [[nodiscard]] constexpr autodiff::dual stateTransitionVy(const StateInput &x)
    {
        const autodiff::dual &v_x = x(static_cast<Eigen::Index>(VX));
        const autodiff::dual &v_y = x(static_cast<Eigen::Index>(VY));
        const autodiff::dual &r   = x(static_cast<Eigen::Index>(R));
        const autodiff::dual &a_y = x(static_cast<Eigen::Index>(4 + AY));

        return v_y + (ESTIMATOR_DT_S * (a_y - (v_x * r)));
    }

    [[nodiscard]] constexpr autodiff::dual stateTransitionYawRate(const StateInput &x)
    {
        const autodiff::dual &r  = x(static_cast<Eigen::Index>(R));
        const autodiff::dual &mz = x(static_cast<Eigen::Index>(MZ));

        return r + (ESTIMATOR_DT_S * (mz / ESTIMATOR_YAW_INERTIA));
    }

    [[nodiscard]] constexpr autodiff::dual stateTransitionYawMoment(const StateInput &x)
    {
        return x(static_cast<Eigen::Index>(MZ));
    }

    [[nodiscard]] constexpr autodiff::dual measurementVx(const State &x)
    {
        return x(static_cast<Eigen::Index>(VX));
    }

    [[nodiscard]] constexpr autodiff::dual measurementVy(const State &x)
    {
        return x(static_cast<Eigen::Index>(VY));
    }

    [[nodiscard]] constexpr autodiff::dual measurementYawRate(const State &x)
    {
        return x(static_cast<Eigen::Index>(R));
    }

    [[nodiscard]] constexpr autodiff::dual measurementYawMoment(const State &x)
    {
        return x(static_cast<Eigen::Index>(MZ));
    }

    [[nodiscard]] consteval std::array<Filter::StateFunction, 4> createStateFunctions()
    {
        return { {
            stateTransitionVx,
            stateTransitionVy,
            stateTransitionYawRate,
            stateTransitionYawMoment,
        } };
    }

    [[nodiscard]] consteval std::array<Filter::MeasurementFunction, 4> createMeasurementFunctions()
    {
        return { {
            measurementVx,
            measurementVy,
            measurementYawRate,
            measurementYawMoment,
        } };
    }

    [[nodiscard]] constexpr Filter::N_N processNoise()
    {
        Filter::N_N q                                                   = Filter::N_N::Zero();
        q(static_cast<Eigen::Index>(VX), static_cast<Eigen::Index>(VX)) = 0.05f;
        q(static_cast<Eigen::Index>(VY), static_cast<Eigen::Index>(VY)) = 0.05f;
        q(static_cast<Eigen::Index>(R), static_cast<Eigen::Index>(R))   = 0.10f;
        q(static_cast<Eigen::Index>(MZ), static_cast<Eigen::Index>(MZ)) = 150.0f;
        return q;
    }

    [[nodiscard]] constexpr Filter::M_M measurementNoise()
    {
        Filter::M_M r                                                   = Filter::M_M::Zero();
        r(static_cast<Eigen::Index>(VX), static_cast<Eigen::Index>(VX)) = 0.75f;
        r(static_cast<Eigen::Index>(VY), static_cast<Eigen::Index>(VY)) = 0.75f;
        r(static_cast<Eigen::Index>(R), static_cast<Eigen::Index>(R))   = 0.05f;
        r(static_cast<Eigen::Index>(MZ), static_cast<Eigen::Index>(MZ)) = 250.0f;
        return r;
    }

    [[nodiscard]] constexpr StateVector initialState()
    {
        return StateVector::Zero();
    }

    [[nodiscard]] constexpr Filter::N_N initialCovariance()
    {
        Filter::N_N p0                                                   = Filter::N_N::Identity();
        p0(static_cast<Eigen::Index>(VX), static_cast<Eigen::Index>(VX)) = 5.0f;
        p0(static_cast<Eigen::Index>(VY), static_cast<Eigen::Index>(VY)) = 5.0f;
        p0(static_cast<Eigen::Index>(R), static_cast<Eigen::Index>(R))   = 1.0f;
        p0(static_cast<Eigen::Index>(MZ), static_cast<Eigen::Index>(MZ)) = 400.0f;
        return p0;
    }

    [[nodiscard]] Measurement pseudoMeasurementFromWheelSpeeds(
        const datatypes::datatypes::wheel_set<float> &wheel_angular_velocities_radps,
        const float                                   yaw_rate_radps,
        const float                                   steering_angle_rad,
        const StateVector                            &previous_state)
    {
        const float     front_cos    = std::cos(steering_angle_rad);
        const float     front_sin    = std::sin(steering_angle_rad);
        constexpr float half_track_m = TRACK_WIDTH_m * 0.5f;

        const std::array<float, 4> wheel_surface_speeds_mps = { {
            wheel_angular_velocities_radps.fl * WHEEL_RADIUS_M,
            wheel_angular_velocities_radps.fr * WHEEL_RADIUS_M,
            wheel_angular_velocities_radps.rl * WHEEL_RADIUS_M,
            wheel_angular_velocities_radps.rr * WHEEL_RADIUS_M,
        } };

        const float speed_sum_mps = std::fabs(wheel_surface_speeds_mps[0]) + std::fabs(wheel_surface_speeds_mps[1]) +
                                    std::fabs(wheel_surface_speeds_mps[2]) + std::fabs(wheel_surface_speeds_mps[3]);

        Measurement z                    = Measurement::Zero();
        z(static_cast<Eigen::Index>(VX)) = previous_state(static_cast<Eigen::Index>(VX));
        z(static_cast<Eigen::Index>(VY)) = previous_state(static_cast<Eigen::Index>(VY));

        if (speed_sum_mps <= SMALL_EPSILON)
        {
            z(static_cast<Eigen::Index>(VX)) = 0.0f;
            z(static_cast<Eigen::Index>(VY)) = 0.0f;
            return z;
        }

        const float fl_vx = (wheel_surface_speeds_mps[0] * front_cos) + (yaw_rate_radps * half_track_m);
        const float fr_vx = (wheel_surface_speeds_mps[1] * front_cos) - (yaw_rate_radps * half_track_m);
        const float rl_vx = wheel_surface_speeds_mps[2] + (yaw_rate_radps * half_track_m);
        const float rr_vx = wheel_surface_speeds_mps[3] - (yaw_rate_radps * half_track_m);

        const float fl_vy = (wheel_surface_speeds_mps[0] * front_sin) - (yaw_rate_radps * DIST_FRONT_AXLE_CG_m);
        const float fr_vy = (wheel_surface_speeds_mps[1] * front_sin) - (yaw_rate_radps * DIST_FRONT_AXLE_CG_m);
        const float rl_vy = yaw_rate_radps * DIST_REAR_AXLE_CG_m;
        const float rr_vy = yaw_rate_radps * DIST_REAR_AXLE_CG_m;

        z(static_cast<Eigen::Index>(VX)) = 0.25f * (fl_vx + fr_vx + rl_vx + rr_vx);
        z(static_cast<Eigen::Index>(VY)) = 0.25f * (fl_vy + fr_vy + rl_vy + rr_vy);
        return z;
    }

    [[nodiscard]] constexpr Filter createFilter()
    {
        return Filter(
            createStateFunctions(), createMeasurementFunctions(), processNoise(), measurementNoise(), initialState(),
            initialCovariance());
    }
} // namespace

namespace VehicleStateEstimator
{
    Filter filter_ = createFilter();
    void   reset_filter()
    {
        filter_ = createFilter();
    }

    [[nodiscard]] shared_datatypes::datatypes::VehicleState estimate(const Measurements &state)
    {
        InputVector u                    = InputVector::Zero();
        u(static_cast<Eigen::Index>(AX)) = state.ax;
        u(static_cast<Eigen::Index>(AY)) = state.ay;

        const float                                  measured_yaw_rate_radps  = state.yaw_rate;
        const float                                  measured_steering_angle  = state.delta;
        const datatypes::datatypes::wheel_set<float> wheel_angular_velocities = state.omegas;
        const auto                                  &previous_state           = filter_.state();

        Measurement z = pseudoMeasurementFromWheelSpeeds(
            wheel_angular_velocities, measured_yaw_rate_radps, measured_steering_angle, previous_state);

        z(static_cast<Eigen::Index>(R)) = measured_yaw_rate_radps;
        // z(static_cast<Eigen::Index>(MZ)) =
        //     dynamics_estimator_.est_Mz_N(inputs.longitudinal_forces_N, inputs.lateral_forces_N,
        //     measured_steering_angle);

        const StateVector estimated_state = filter_.estimated_states(u, z);

        // outputs_.yaw_moment_nm = estimated_state(static_cast<Eigen::Index>(MZ));
        return {
            .v_x_mps        = estimated_state(static_cast<Eigen::Index>(VX)),
            .v_y_mps        = estimated_state(static_cast<Eigen::Index>(VY)),
            .yaw_rate_radps = estimated_state(static_cast<Eigen::Index>(R)),
            .steer_ang_rad  = measured_steering_angle,
            .a_x_mps2       = u(static_cast<Eigen::Index>(AX)),
            .a_y_mps2       = u(static_cast<Eigen::Index>(AY)),
        };
    }

    const Covariance &covariance()
    {
        return filter_.covariance();
    }
} // namespace VehicleStateEstimator
} // namespace app::tv::estimation
