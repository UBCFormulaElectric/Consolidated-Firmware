#pragma once
#include "controls/datatypes/wheel_set.hpp"
#include "controls/params.hpp"
#include "controls/datatypes/decimal_dual.hpp"
#include <algorithm>
#include <cmath>

namespace app::controls
{
template <Decimal T> struct VehicleState
{
    // state variables
    T            v_x_mps           = 0.0f;
    T            v_y_mps           = 0.0f;
    T            yaw_rate_radps    = 0.0f;
    T            a_x_mps2          = 0.0f;
    T            a_y_mps2          = 0.0f;
    T            apps              = 0.0f; // accelerator pedal, 0..1
    T            brake             = 0.0f; // brake pedal, 0..1
    T            charge_limit_W    = 0.0f; // battery power limits from the BMS
    T            discharge_limit_W = 0.0f;
    wheel_set<T> wheel_angle_rad{};   // road wheel steer angles
    wheel_set<T> wheel_omega_radps{}; // wheel (not motor) speeds; motor_omegas() gives the motor speeds

    [[nodiscard]] wheel_set<T> motor_omegas() const
    {
        return {
            .fl = wheel_omega_radps.fl * static_cast<tv_real>(GEAR_RATIO),
            .fr = wheel_omega_radps.fr * static_cast<tv_real>(GEAR_RATIO),
            .rl = wheel_omega_radps.rl * static_cast<tv_real>(GEAR_RATIO),
            .rr = wheel_omega_radps.rr * static_cast<tv_real>(GEAR_RATIO),
        };
    }

    /**
     * @return vector of vy each in the frame of the respective tire
     */
    [[nodiscard]] wheel_set<Pair<T>> v_in_tire_frame() const
    {
        wheel_set<Pair<T>> v = {
            {
                v_x_mps - yaw_rate_radps * params::HALF_TRACK_M,
                v_y_mps + yaw_rate_radps * params::DIST_FRONT_AXLE_CG_m,
            },
            {
                v_x_mps - yaw_rate_radps * -params::HALF_TRACK_M,
                v_y_mps + yaw_rate_radps * params::DIST_FRONT_AXLE_CG_m,
            },
            {
                v_x_mps - yaw_rate_radps * params::HALF_TRACK_M,
                v_y_mps + yaw_rate_radps * -params::DIST_FRONT_AXLE_CG_m,
            },
            {
                v_x_mps - yaw_rate_radps * -params::HALF_TRACK_M,
                v_y_mps + yaw_rate_radps * -params::DIST_FRONT_AXLE_CG_m,
            },
        };
        v.rotate(wheel_angle_rad);
        return v;
    }

    [[nodiscard]] wheel_set<T> alphas() const
    {
        // v_in_tire_frame() has already rotated each wheel velocity by its steer angle, so steer must not be
        // subtracted again here. The tire fit uses the TYDEX / ISO-W convention alpha = -atan(v_y / |v_x|) (positive
        // alpha -> positive Fy), with |v_x| floored so near-standstill noise in v_y / yaw rate cannot produce
        // +-90 deg slip angles.
        const auto [fl_v, fr_v, rl_v, rr_v] = v_in_tire_frame();
        const auto slip_angle               = [](const Pair<T> &v)
        { return -std::atan2(v.y, std::max(std::abs(v.x), static_cast<T>(params::SLIP_REGULARIZATION_SPEED_MPS))); };
        return { slip_angle(fl_v), slip_angle(fr_v), slip_angle(rl_v), slip_angle(rr_v) };
    }

    // AERODYNAMIC EFFECTS
    // these might use internal state later??
    // ReSharper disable once CppMemberFunctionMayBeStatic
    [[nodiscard]] constexpr T dynamicCOPFront() const { return 1.0f - params::COP_REAR; }
    // ReSharper disable once CppMemberFunctionMayBeStatic
    [[nodiscard]] constexpr T dynamicCOPRight() const { return params::COP_RIGHT; }
    [[nodiscard]] constexpr T est_dragFx_N() const
    {
        return 0.5f * params::AIR_DENSITY_KGPM3 * params::FRONTAL_AREA_M2 * params::DRAG_COEFF * v_x_mps * v_x_mps;
    }
    [[nodiscard]] constexpr T est_downforceFz_N() const
    {
        return 0.5f * params::AIR_DENSITY_KGPM3 * params::FRONTAL_AREA_M2 * params::LIFT_COEFF * v_x_mps * v_x_mps;
    }

    // =============================================================================
    // VEHICLE DYNAMICS - VERTICAL LOAD TRANSFER
    // Reference: https://www.zotero.org/groups/5809911/vehicle_controls_2024/items/N4TQBR67/reader
    // =============================================================================

    /**
     * Longitudinal load transfer component (page 21)
     * Positive long_accel transfers load to rear axle
     * @return Load transfer force (N)
     */
    [[nodiscard]] constexpr T LONG_ACCEL_TERM_VERTICAL_FORCE() const
    {
        return (params::CAR_MASS_AT_CG_KG * a_x_mps2 * params::DIST_HEIGHT_CG_m) / params::WHEELBASE_m;
    }
    /**
     * Lateral load transfer component (page 21)
     * Transfers load to outside wheels during cornering
     * @return Load transfer force per side (N)
     */
    [[nodiscard]] constexpr T LAT_ACCEL_TERM_VERTICAL_FORCE() const
    {
        return (params::CAR_MASS_AT_CG_KG * a_y_mps2 * params::DIST_HEIGHT_CG_m) / (2.0f * params::TRACK_WIDTH_m);
    }

    [[nodiscard]] wheel_set<T> est_Fz_N() const
    {
        // static loads
        static constexpr T STATIC_FRONT_AXLE_LOAD_N =
            params::CAR_WEIGHT_N * (params::DIST_REAR_AXLE_CG_m / params::WHEELBASE_m);
        static constexpr T STATIC_REAR_AXLE_LOAD_N =
            params::CAR_WEIGHT_N * (params::DIST_FRONT_AXLE_CG_m / params::WHEELBASE_m);
        static constexpr T STATIC_FRONT_WHEEL_LOAD_N = 0.5f * STATIC_FRONT_AXLE_LOAD_N;
        static constexpr T STATIC_REAR_WHEEL_LOAD_N  = 0.5f * STATIC_REAR_AXLE_LOAD_N;

        // load transfer
        const T long_load_transfer_N = LONG_ACCEL_TERM_VERTICAL_FORCE();
        const T lat_load_transfer_N  = LAT_ACCEL_TERM_VERTICAL_FORCE();

        // downforce and cop components
        const T downforce_N = est_downforceFz_N();
        // Assuming COP is a fraction of the total downforce
        const T front_cop = dynamicCOPFront(), rear_cop = 1.0f - front_cop, right_cop = dynamicCOPRight(),
                left_cop = 1.0f - right_cop;
        return {
            .fl = std::fmax(
                0.0f, STATIC_FRONT_WHEEL_LOAD_N - (0.5f * long_load_transfer_N) - lat_load_transfer_N +
                          downforce_N * front_cop * left_cop),
            .fr = std::fmax(
                0.0f, STATIC_FRONT_WHEEL_LOAD_N - (0.5f * long_load_transfer_N) + lat_load_transfer_N +
                          downforce_N * front_cop * right_cop),
            .rl = std::fmax(
                0.0f, STATIC_REAR_WHEEL_LOAD_N + (0.5f * long_load_transfer_N) - lat_load_transfer_N +
                          downforce_N * rear_cop * left_cop),
            .rr = std::fmax(
                0.0f, STATIC_REAR_WHEEL_LOAD_N + (0.5f * long_load_transfer_N) + lat_load_transfer_N +
                          downforce_N * rear_cop * right_cop),
        };
    }

    /**
     * Get body slip
     * @return
     */
    [[nodiscard]] T est_beta_rad() const { return std::atan2(v_y_mps, safe_vx(v_x_mps)); }

    /**
     * @param tire_forces_N tire forces
     * @return Given certain tire forces, what would be the resulting yaw moment Mz about the CG?
     */
    template <DecimalOrDual F> [[nodiscard]] F est_Mz_Nm(wheel_set<Pair<F>> tire_forces_N) const
    {
        // TODO aligning moment contributions to the yaw moment equation
        // Tire forces are in each tire's (steered) frame; bring them into the body frame, i.e. rotate by
        // +wheel_angle_rad. rotate() goes body -> tire (by -wheel_angle_rad), so it is given the negated steer angles.
        tire_forces_N.rotate(
            wheel_set<T>{ -wheel_angle_rad.fl, -wheel_angle_rad.fr, -wheel_angle_rad.rl, -wheel_angle_rad.rr });
        const F fl_moment =
            (params::DIST_FRONT_AXLE_CG_m * tire_forces_N.fl.y) - (params::HALF_TRACK_M * tire_forces_N.fl.x);
        const F fr_moment =
            (params::DIST_FRONT_AXLE_CG_m * tire_forces_N.fr.y) + (params::HALF_TRACK_M * tire_forces_N.fr.x);
        const F rl_moment =
            (-params::DIST_REAR_AXLE_CG_m * tire_forces_N.rl.y) - (params::HALF_TRACK_M * tire_forces_N.rl.x);
        const F rr_moment =
            (-params::DIST_REAR_AXLE_CG_m * tire_forces_N.rr.y) + (params::HALF_TRACK_M * tire_forces_N.rr.x);
        return fl_moment + fr_moment + rl_moment + rr_moment;
    }

    /**
     * Yaw moment distribution factor Kmz (page 57)
     * Accounts for load transfer effect on yaw moment generation capacity
     * @return Effective moment arm (m)
     */
    [[nodiscard]] constexpr T ACCELERATION_TERM_KMZ() const
    {
        return params::DIST_FRONT_AXLE_CG_m + (a_x_mps2 * params::DIST_HEIGHT_CG_m) / params::GRAVITY;
    }
    [[nodiscard]] constexpr T KMZ() const
    {
        const T LONG_ACCEL_TERM = ACCELERATION_TERM_KMZ();
        return ((params::CAR_WEIGHT_N - (params::CAR_WEIGHT_N / params::WHEELBASE_m) * LONG_ACCEL_TERM)) /
               ((params::CAR_WEIGHT_N / params::WHEELBASE_m) * LONG_ACCEL_TERM);
    }
    /**
     * Moment scaling factor F (page 58)
     * Relates torque differential to yaw moment through track width and effective radius
     */
    static constexpr T F = (params::TRACK_WIDTH_m / ((WHEEL_DIAMETER_IN / 2.0f) * IN_TO_M)) * GEAR_RATIO;

  private:
    [[nodiscard]] static T safe_vx(const T v_x_mps)
    {
        if (std::fabs(v_x_mps) >= params::SMALL_EPSILON)
            return v_x_mps;
        return v_x_mps < 0.0f ? -params::SMALL_EPSILON : params::SMALL_EPSILON;
    }
};
} // namespace app::controls
