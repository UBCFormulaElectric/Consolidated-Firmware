#pragma once
#include "torque_vectoring/shared_datatypes/wheel_set.hpp"
#include "torque_vectoring/shared_datatypes/constants.hpp"
#include "torque_vectoring/shared_datatypes/decimal_dual.hpp"

namespace app::tv::shared_datatypes
{
template <Decimal T> struct VehicleState
{
    // state variables
    T            v_x_mps        = 0.0f;
    T            v_y_mps        = 0.0f;
    T            yaw_rate_radps = 0.0f;
    T            a_x_mps2       = 0.0f;
    T            a_y_mps2       = 0.0f;
    T            apps           = 0.0f;
    wheel_set<T> delta{};

    /**
     * @return vector of vy each in the frame of the respective tire
     */
    [[nodiscard]] wheel_set<Pair<T>> v_in_tire_frame() const
    {
        wheel_set<Pair<T>> v = {
            {
                v_x_mps - yaw_rate_radps * vd_constants::HALF_TRACK_M,
                v_y_mps + yaw_rate_radps * vd_constants::DIST_FRONT_AXLE_CG_m,
            },
            {
                v_x_mps - yaw_rate_radps * -vd_constants::HALF_TRACK_M,
                v_y_mps + yaw_rate_radps * vd_constants::DIST_FRONT_AXLE_CG_m,
            },
            {
                v_x_mps - yaw_rate_radps * vd_constants::HALF_TRACK_M,
                v_y_mps + yaw_rate_radps * -vd_constants::DIST_FRONT_AXLE_CG_m,
            },
            {
                v_x_mps - yaw_rate_radps * -vd_constants::HALF_TRACK_M,
                v_y_mps + yaw_rate_radps * -vd_constants::DIST_FRONT_AXLE_CG_m,
            },
        };
        v.rotate(delta);
        return v;
    }

    [[nodiscard]] wheel_set<T> alphas() const
    {
        const auto [fl_v, fr_v, rl_v, rr_v]         = v_in_tire_frame();
        const auto [fl_rot, fr_rot, rl_rot, rr_rot] = delta;
        return {
            std::atan2(fl_v.y, safe_vx(fl_v.x)) - fl_rot,
            std::atan2(fr_v.y, safe_vx(fr_v.x)) - fr_rot,
            std::atan2(rl_v.y, safe_vx(rl_v.x)) - rl_rot,
            std::atan2(rr_v.y, safe_vx(rr_v.x)) - rr_rot,
        };
    }

    // AERODYNAMIC EFFECTS
    // these might use internal state later??
    // ReSharper disable once CppMemberFunctionMayBeStatic
    [[nodiscard]] constexpr T dynamicCOPFront() const { return 1.0f - vd_constants::COP_REAR; }
    // ReSharper disable once CppMemberFunctionMayBeStatic
    [[nodiscard]] constexpr T dynamicCOPRight() const { return vd_constants::COP_RIGHT; }
    [[nodiscard]] constexpr T est_dragFx_N() const
    {
        return 0.5f * vd_constants::AIR_DENSITY_KGPM3 * vd_constants::FRONTAL_AREA_M2 * vd_constants::DRAG_COEFF *
               v_x_mps * v_x_mps;
    }
    [[nodiscard]] constexpr T est_downforceFz_N() const
    {
        return 0.5f * vd_constants::AIR_DENSITY_KGPM3 * vd_constants::FRONTAL_AREA_M2 * vd_constants::LIFT_COEFF *
               v_x_mps * v_x_mps;
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
        return (vd_constants::CAR_MASS_AT_CG_KG * a_x_mps2 * vd_constants::DIST_HEIGHT_CG_m) /
               vd_constants::WHEELBASE_m;
    }
    /**
     * Lateral load transfer component (page 21)
     * Transfers load to outside wheels during cornering
     * @return Load transfer force per side (N)
     */
    [[nodiscard]] constexpr T LAT_ACCEL_TERM_VERTICAL_FORCE() const
    {
        return (vd_constants::CAR_MASS_AT_CG_KG * a_y_mps2 * vd_constants::DIST_HEIGHT_CG_m) /
               (2.0f * vd_constants::TRACK_WIDTH_m);
    }

    [[nodiscard]] wheel_set<T> est_Fz_N() const
    {
        // static loads
        static constexpr T STATIC_FRONT_AXLE_LOAD_N =
            vd_constants::CAR_WEIGHT * (vd_constants::DIST_REAR_AXLE_CG_m / vd_constants::WHEELBASE_m);
        static constexpr T STATIC_REAR_AXLE_LOAD_N =
            vd_constants::CAR_WEIGHT * (vd_constants::DIST_FRONT_AXLE_CG_m / vd_constants::WHEELBASE_m);
        static constexpr T STATIC_FRONT_WHEEL_LOAD_N = 0.5f * STATIC_FRONT_AXLE_LOAD_N;
        static constexpr T STATIC_REAR_WHEEL_LOAD_N  = 0.5f * STATIC_REAR_AXLE_LOAD_N;

        // load transfer
        const T long_load_tf = LONG_ACCEL_TERM_VERTICAL_FORCE();
        const T lat_load_tf  = LAT_ACCEL_TERM_VERTICAL_FORCE();

        // downforce and cop components
        const T down_force_n = est_downforceFz_N();
        // Assuming COP is a fraction of the total downforce
        const T front_cop = dynamicCOPFront(), rear_cop = 1.0f - front_cop, right_cop = dynamicCOPRight(),
                left_cop = 1.0f - right_cop;
        return {
            .fl = std::fmax(
                0.0f,
                STATIC_FRONT_WHEEL_LOAD_N - (0.5f * long_load_tf) - lat_load_tf + down_force_n * front_cop * left_cop),
            .fr = std::fmax(
                0.0f,
                STATIC_FRONT_WHEEL_LOAD_N - (0.5f * long_load_tf) + lat_load_tf + down_force_n * front_cop * right_cop),
            .rl = std::fmax(
                0.0f,
                STATIC_REAR_WHEEL_LOAD_N + (0.5f * long_load_tf) - lat_load_tf + down_force_n * rear_cop * left_cop),
            .rr = std::fmax(
                0.0f,
                STATIC_REAR_WHEEL_LOAD_N + (0.5f * long_load_tf) + lat_load_tf + down_force_n * rear_cop * right_cop),
        };
    }

    /**
     * Get body slip
     * @return
     */
    [[nodiscard]] T est_beta_rad() const { return std::atan2(v_y_mps, safe_vx(v_x_mps)); }

    /**
     * @param tires_F_N tire forces
     * @return Given certain tire forces, what would be the resulting yaw moment Mz about the CG?
     */
    template <DecimalOrDual F> [[nodiscard]] F est_Mz_N(wheel_set<Pair<F>> tires_F_N) const
    {
        // TODO aligning moment contributions to the yaw moment equation
        tires_F_N.rotate(delta);
        const F fl_moment =
            (vd_constants::DIST_FRONT_AXLE_CG_m * tires_F_N.fl.y) - (vd_constants::HALF_TRACK_M * tires_F_N.fl.x);
        const F fr_moment =
            (vd_constants::DIST_FRONT_AXLE_CG_m * tires_F_N.fr.y) + (vd_constants::HALF_TRACK_M * tires_F_N.fr.x);
        const F rl_moment =
            (-vd_constants::DIST_REAR_AXLE_CG_m * tires_F_N.rl.y) - (vd_constants::HALF_TRACK_M * tires_F_N.rl.x);
        const F rr_moment =
            (-vd_constants::DIST_REAR_AXLE_CG_m * tires_F_N.rr.y) + (vd_constants::HALF_TRACK_M * tires_F_N.rr.x);
        return fl_moment + fr_moment + rl_moment + rr_moment;
    }

    /**
     * Yaw moment distribution factor Kmz (page 57)
     * Accounts for load transfer effect on yaw moment generation capacity
     * @return Effective moment arm (m)
     */
    [[nodiscard]] constexpr T ACCELERATION_TERM_KMZ() const
    {
        return vd_constants::DIST_FRONT_AXLE_CG_m + (a_x_mps2 * vd_constants::DIST_HEIGHT_CG_m) / vd_constants::GRAVITY;
    }
    [[nodiscard]] constexpr T KMZ() const
    {
        const T LONG_ACCEL_TERM = ACCELERATION_TERM_KMZ();
        return ((vd_constants::CAR_WEIGHT - (vd_constants::CAR_WEIGHT / vd_constants::WHEELBASE_m) * LONG_ACCEL_TERM)) /
               ((vd_constants::CAR_WEIGHT / vd_constants::WHEELBASE_m) * LONG_ACCEL_TERM);
    }
    /**
     * Moment scaling factor F (page 58)
     * Relates torque differential to yaw moment through track width and effective radius
     */
    static constexpr T F = (vd_constants::TRACK_WIDTH_m / ((WHEEL_DIAMETER_IN / 2.0f) * IN_TO_M)) * GEAR_RATIO;

  private:
    [[nodiscard]] static T safe_vx(const T v_x_mps)
    {
        if (std::fabs(v_x_mps) >= vd_constants::SMALL_EPSILON)
            return v_x_mps;
        return v_x_mps < 0.0f ? -vd_constants::SMALL_EPSILON : vd_constants::SMALL_EPSILON;
    }
};
} // namespace app::tv::shared_datatypes