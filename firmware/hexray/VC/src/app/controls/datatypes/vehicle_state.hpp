#pragma once
#include "controls/datatypes/wheel_set.hpp"
#include "controls/params.hpp"
#include "controls/datatypes/decimal_dual.hpp"
#include <algorithm>
#include <cmath>

namespace app::controls
{
struct VehicleState
{
    // state variables
    float            v_x_mps           = 0.0f;
    float            v_y_mps           = 0.0f;
    float            yaw_rate_radps    = 0.0f;
    float            a_x_mps2          = 0.0f;
    float            a_y_mps2          = 0.0f;
    float            apps              = 0.0f; // accelerator pedal, 0..1
    float            brake             = 0.0f; // brake pedal, 0..1
    float            charge_limit_W    = 0.0f; // battery power limits from the BMS
    float            discharge_limit_W = 0.0f;
    wheel_set<float> wheel_angle_rad{};   // road wheel steer angles
    wheel_set<float> wheel_omega_radps{}; // wheel (not motor) speeds; motor_omegas() gives the motor speeds

    [[nodiscard]] wheel_set<float> motor_omegas() const
    {
        return {
            .fl = wheel_omega_radps.fl * GEAR_RATIO,
            .fr = wheel_omega_radps.fr * GEAR_RATIO,
            .rl = wheel_omega_radps.rl * GEAR_RATIO,
            .rr = wheel_omega_radps.rr * GEAR_RATIO,
        };
    }

    /**
     * @return vector of vy each in the frame of the respective tire
     */
    [[nodiscard]] wheel_set<Pair<float>> v_in_tire_frame() const
    {
        wheel_set<Pair<float>> v = {
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

    [[nodiscard]] wheel_set<float> alphas() const
    {
        // v_in_tire_frame() has already rotated each wheel velocity by its steer angle, so steer must not be
        // subtracted again here. The tire fit uses the TYDEX / ISO-W convention alpha = -atan(v_y / |v_x|) (positive
        // alpha -> positive Fy), with |v_x| floored so near-standstill noise in v_y / yaw rate cannot produce
        // +-90 deg slip angles.
        const auto [fl_v, fr_v, rl_v, rr_v] = v_in_tire_frame();
        const auto slip_angle               = [](const Pair<float> &v)
        { return -std::atan2(v.y, std::max(std::abs(v.x), params::SLIP_REGULARIZATION_SPEED_MPS)); };
        return { slip_angle(fl_v), slip_angle(fr_v), slip_angle(rl_v), slip_angle(rr_v) };
    }

    // AERODYNAMIC EFFECTS
    // these might use internal state later??
    // ReSharper disable once CppMemberFunctionMayBeStatic
    [[nodiscard]] constexpr float dynamicCOPFront() const { return 1.0f - params::COP_REAR; }
    // ReSharper disable once CppMemberFunctionMayBeStatic
    [[nodiscard]] constexpr float dynamicCOPRight() const { return params::COP_RIGHT; }
    [[nodiscard]] constexpr float est_dragFx_N() const
    {
        return 0.5f * params::AIR_DENSITY_KGPM3 * params::FRONTAL_AREA_M2 * params::DRAG_COEFF * v_x_mps * v_x_mps;
    }
    [[nodiscard]] constexpr float est_downforceFz_N() const
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
    [[nodiscard]] constexpr float LONG_ACCEL_TERM_VERTICAL_FORCE() const
    {
        return (params::CAR_MASS_AT_CG_KG * a_x_mps2 * params::DIST_HEIGHT_CG_m) / params::WHEELBASE_m;
    }
    /**
     * Lateral load transfer component (page 21)
     * Transfers load to outside wheels during cornering
     * @return Load transfer force per side (N)
     */
    [[nodiscard]] constexpr float LAT_ACCEL_TERM_VERTICAL_FORCE() const
    {
        return (params::CAR_MASS_AT_CG_KG * a_y_mps2 * params::DIST_HEIGHT_CG_m) / (2.0f * params::TRACK_WIDTH_m);
    }

    [[nodiscard]] wheel_set<float> est_Fz_N() const
    {
        // static loads
        static constexpr float STATIC_FRONT_AXLE_LOAD_N =
            params::CAR_WEIGHT_N * (params::DIST_REAR_AXLE_CG_m / params::WHEELBASE_m);
        static constexpr float STATIC_REAR_AXLE_LOAD_N =
            params::CAR_WEIGHT_N * (params::DIST_FRONT_AXLE_CG_m / params::WHEELBASE_m);
        static constexpr float STATIC_FRONT_WHEEL_LOAD_N = 0.5f * STATIC_FRONT_AXLE_LOAD_N;
        static constexpr float STATIC_REAR_WHEEL_LOAD_N  = 0.5f * STATIC_REAR_AXLE_LOAD_N;

        // load transfer
        const float long_load_transfer_N = LONG_ACCEL_TERM_VERTICAL_FORCE();
        const float lat_load_transfer_N  = LAT_ACCEL_TERM_VERTICAL_FORCE();

        // downforce and cop components
        const float downforce_N = est_downforceFz_N();
        // Assuming COP is a fraction of the total downforce
        const float front_cop = dynamicCOPFront(), rear_cop = 1.0f - front_cop, right_cop = dynamicCOPRight(),
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
    [[nodiscard]] float est_beta_rad() const { return std::atan2(v_y_mps, safe_vx(v_x_mps)); }

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
            wheel_set<float>{ -wheel_angle_rad.fl, -wheel_angle_rad.fr, -wheel_angle_rad.rl, -wheel_angle_rad.rr });
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
    [[nodiscard]] constexpr float ACCELERATION_TERM_KMZ() const
    {
        return params::DIST_FRONT_AXLE_CG_m + (a_x_mps2 * params::DIST_HEIGHT_CG_m) / params::GRAVITY;
    }
    [[nodiscard]] constexpr float KMZ() const
    {
        const float LONG_ACCEL_TERM = ACCELERATION_TERM_KMZ();
        return ((params::CAR_WEIGHT_N - (params::CAR_WEIGHT_N / params::WHEELBASE_m) * LONG_ACCEL_TERM)) /
               ((params::CAR_WEIGHT_N / params::WHEELBASE_m) * LONG_ACCEL_TERM);
    }
    /**
     * Moment scaling factor F (page 58)
     * Relates torque differential to yaw moment through track width and effective radius
     */
    static constexpr float F = (params::TRACK_WIDTH_m / ((WHEEL_DIAMETER_IN / 2.0f) * IN_TO_M)) * GEAR_RATIO;

  private:
    [[nodiscard]] static float safe_vx(const float v_x_mps)
    {
        if (std::fabs(v_x_mps) >= params::SMALL_EPSILON)
            return v_x_mps;
        return v_x_mps < 0.0f ? -params::SMALL_EPSILON : params::SMALL_EPSILON;
    }
};
} // namespace app::controls
