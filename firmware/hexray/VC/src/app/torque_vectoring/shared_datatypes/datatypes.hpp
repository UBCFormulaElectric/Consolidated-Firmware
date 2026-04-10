#pragma once

#include "constants.hpp"
#include "dual.hpp"
#include <cmath>

template <typename T>
concept DecimalOrDual = std::same_as<T, float> || std::same_as<T, double> || std::same_as<T, autodiff::dual>;

namespace app::tv::shared_datatypes
{
template <typename T> struct wheel_set
{
    T fl;
    T fr;
    T rl;
    T rr;

    // void rotate(const wheel_set<float> tire_angles)
    // {
    //     const float cos_delta_fl = std::cos(tire_angles.fl);
    //     const float sin_delta_fl = std::sin(tire_angles.fl);
    //     fl                       = (cos_delta_fl * fl) - (sin_delta_fl * fl);
    //
    //     const float cos_delta_fr = std::cos(tire_angles.fr);
    //     const float sin_delta_fr = std::sin(tire_angles.fr);
    //     fr                       = (cos_delta_fr * fr) - (sin_delta_fr * fr);
    //
    //     if (tire_angles.rl != 0.0f)
    //     {
    //         const float cos_delta_rl = std::cos(tire_angles.rl);
    //         const float sin_delta_rl = std::sin(tire_angles.rl);
    //         rl                       = (cos_delta_rl * rl) - (sin_delta_rl * rl);
    //     }
    //     if (tire_angles.rr != 0.0f)
    //     {
    //         const float cos_delta_rr = std::cos(tire_angles.rr);
    //         const float sin_delta_rr = std::sin(tire_angles.rr);
    //         rr                       = (cos_delta_rr * rr) - (sin_delta_rr * rr);
    //     }
    // }
};

inline float slipRatioToWheelAngularVelocity(const float slip_ratio, const float v_x_mps)
{
    // Avoid division by zero at very low speeds
    if (std::fabs(v_x_mps) < vd_constants::SMALL_EPSILON)
        return 0.0f;
    return (1.0f + slip_ratio) * (v_x_mps / vd_constants::WHEEL_RADIUS_M);
}

[[nodiscard]] inline float safe_vx(const float v_x_mps)
{
    if (std::fabs(v_x_mps) >= vd_constants::SMALL_EPSILON)
        return v_x_mps;
    return v_x_mps < 0.0f ? -vd_constants::SMALL_EPSILON : vd_constants::SMALL_EPSILON;
}

struct VehicleState
{
    // state variables
    float v_x_mps          = 0.0f;
    float v_y_mps          = 0.0f;
    float yaw_rate_radps   = 0.0f;
    float steer_ang_rad    = 0.0f;
    float a_x_mps2         = 0.0f;
    float a_y_mps2         = 0.0f;
    float pedal_percentage = 0.0f;

    wheel_set<float> omegas_radps{};
    wheel_set<float> Fxs_N{};
    wheel_set<float> Fys_N{};

    [[nodiscard]] wheel_set<float> get_tire_angle() const
    {
        // todo steering model
        return { steer_ang_rad, steer_ang_rad, 0, 0 };
    }

    struct Pair
    {
        float x;
        float y;
    };

    /**
     * @return vector of vy each in the frame of the respective tire
     */
    wheel_set<Pair> v_in_tire_frame() const
    {
        const wheel_set vx_s = {
            v_x_mps - yaw_rate_radps * vd_constants::HALF_TRACK_M,
            v_x_mps - yaw_rate_radps * -vd_constants::HALF_TRACK_M,
            v_x_mps - yaw_rate_radps * vd_constants::HALF_TRACK_M,
            v_x_mps - yaw_rate_radps * -vd_constants::HALF_TRACK_M,
        };
        const wheel_set v_ys = {
            v_y_mps + yaw_rate_radps * vd_constants::DIST_FRONT_AXLE_CG_m,
            v_y_mps + yaw_rate_radps * vd_constants::DIST_FRONT_AXLE_CG_m,
            v_y_mps + yaw_rate_radps * -vd_constants::DIST_FRONT_AXLE_CG_m,
            v_y_mps + yaw_rate_radps * -vd_constants::DIST_FRONT_AXLE_CG_m,
        };
        return {
            { vx_s.fl * std::cos(steer_ang_rad) + v_ys.fl * std::sin(steer_ang_rad),
              v_ys.fl * std::cos(steer_ang_rad) - vx_s.fl * std::sin(steer_ang_rad) },
            { vx_s.fr * std::cos(steer_ang_rad) + v_ys.fr * std::sin(steer_ang_rad),
              v_ys.fr * std::cos(steer_ang_rad) - vx_s.fr * std::sin(steer_ang_rad) },
            { vx_s.rl, v_ys.rl },
            { vx_s.rr, v_ys.rr },
        };
    }

    wheel_set<float> alphas() const
    {
        const auto [fl_v, fr_v, rl_v, rr_v] = v_in_tire_frame();
        const wheel_set<float> tire_angles  = get_tire_angle();
        return {
            std::atan2(fl_v.y, safe_vx(fl_v.x)) - tire_angles.fl,
            std::atan2(fr_v.y, safe_vx(fr_v.x)) - tire_angles.fr,
            std::atan2(rl_v.y, safe_vx(rl_v.x)),
            std::atan2(rr_v.y, safe_vx(rr_v.x)),
        };
    }

    [[nodiscard]] wheel_set<float> kappas() const
    {
        const auto [fl, fr, rl, rr] = v_in_tire_frame();
        return {
            .fl = slipRatioToWheelAngularVelocity(omegas_radps.fl, fl.x),
            .fr = slipRatioToWheelAngularVelocity(omegas_radps.fr, fr.x),
            .rl = slipRatioToWheelAngularVelocity(omegas_radps.rl, rl.x),
            .rr = slipRatioToWheelAngularVelocity(omegas_radps.rr, rr.x),
        };
    }

    // AERODYNAMIC EFFECTS
    // these might use internal state later??
    // ReSharper disable once CppMemberFunctionMayBeStatic
    [[nodiscard]] constexpr float dynamicCOPFront() const { return 1.0f - vd_constants::COP_REAR; }
    // ReSharper disable once CppMemberFunctionMayBeStatic
    [[nodiscard]] constexpr float dynamicCOPRight() const { return vd_constants::COP_RIGHT; }
    [[nodiscard]] constexpr float est_dragFx_N() const
    {
        return 0.5f * vd_constants::AIR_DENSITY_KGPM3 * vd_constants::FRONTAL_AREA_M2 * vd_constants::DRAG_COEFF *
               v_x_mps * v_x_mps;
    }
    [[nodiscard]] constexpr float est_downforceFz_N() const
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
    [[nodiscard]] constexpr float LONG_ACCEL_TERM_VERTICAL_FORCE() const
    {
        return (vd_constants::CAR_MASS_AT_CG_KG * a_x_mps2 * vd_constants::DIST_HEIGHT_CG_m) /
               vd_constants::WHEELBASE_m;
    }
    /**
     * Lateral load transfer component (page 21)
     * Transfers load to outside wheels during cornering
     * @return Load transfer force per side (N)
     */
    [[nodiscard]] constexpr float LAT_ACCEL_TERM_VERTICAL_FORCE() const
    {
        return (vd_constants::CAR_MASS_AT_CG_KG * a_y_mps2 * vd_constants::DIST_HEIGHT_CG_m) /
               (2.0f * vd_constants::TRACK_WIDTH_m);
    }

    [[nodiscard]] wheel_set<float> est_Fz_N() const
    {
        // static loads
        static constexpr float STATIC_FRONT_AXLE_LOAD_N =
            vd_constants::CAR_WEIGHT * (vd_constants::DIST_REAR_AXLE_CG_m / vd_constants::WHEELBASE_m);
        static constexpr float STATIC_REAR_AXLE_LOAD_N =
            vd_constants::CAR_WEIGHT * (vd_constants::DIST_FRONT_AXLE_CG_m / vd_constants::WHEELBASE_m);
        static constexpr float STATIC_FRONT_WHEEL_LOAD_N = 0.5f * STATIC_FRONT_AXLE_LOAD_N;
        static constexpr float STATIC_REAR_WHEEL_LOAD_N  = 0.5f * STATIC_REAR_AXLE_LOAD_N;

        // load transfer
        const float long_load_tf = LONG_ACCEL_TERM_VERTICAL_FORCE();
        const float lat_load_tf  = LAT_ACCEL_TERM_VERTICAL_FORCE();

        // downforce and cop components
        const float down_force_n = est_downforceFz_N();
        // Assuming COP is a fraction of the total downforce
        const float front_cop = dynamicCOPFront(), rear_cop = 1.0f - front_cop, right_cop = dynamicCOPRight(),
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
    [[nodiscard]] float est_beta_rad() const { return std::atan2(v_y_mps, safe_vx(v_x_mps)); }

    /**
     * @param tires_Fx_N tire longitudinal forces in Newtons
     * @param tires_Fy_N tire lateral forces in Newtons
     * @return Given certain tire forces, what would be the resulting yaw moment Mz about the CG?
     */
    template <DecimalOrDual T> [[nodiscard]] T est_Mz_N(wheel_set<T> tires_Fx_N, wheel_set<T> tires_Fy_N) const
    {
        tires_Fx_N.rotate(get_tire_angle());
        tires_Fy_N.rotate(get_tire_angle());

        // contributions to moment of each tire
        const T fl_moment =
            (vd_constants::DIST_FRONT_AXLE_CG_m * tires_Fy_N.fl) - (vd_constants::HALF_TRACK_M * tires_Fx_N.fl);
        const T fr_moment =
            (vd_constants::DIST_FRONT_AXLE_CG_m * tires_Fy_N.fr) + (vd_constants::HALF_TRACK_M * tires_Fx_N.fr);
        const T rl_moment =
            (-vd_constants::DIST_REAR_AXLE_CG_m * tires_Fy_N.rl) - (vd_constants::HALF_TRACK_M * tires_Fx_N.rl);
        const T rr_moment =
            (-vd_constants::DIST_REAR_AXLE_CG_m * tires_Fy_N.rr) + (vd_constants::HALF_TRACK_M * tires_Fx_N.rr);

        return fl_moment + fr_moment + rl_moment + rr_moment;
    }
    /**
     * Yaw moment distribution factor Kmz (page 57)
     * Accounts for load transfer effect on yaw moment generation capacity
     * @return Effective moment arm (m)
     */
    [[nodiscard]] constexpr float ACCELERATION_TERM_KMZ() const
    {
        return vd_constants::DIST_FRONT_AXLE_CG_m + (a_x_mps2 * vd_constants::DIST_HEIGHT_CG_m) / vd_constants::GRAVITY;
    }
    [[nodiscard]] constexpr float KMZ() const
    {
        const float LONG_ACCEL_TERM = ACCELERATION_TERM_KMZ();
        return ((vd_constants::CAR_WEIGHT - (vd_constants::CAR_WEIGHT / vd_constants::WHEELBASE_m) * LONG_ACCEL_TERM)) /
               ((vd_constants::CAR_WEIGHT / vd_constants::WHEELBASE_m) * LONG_ACCEL_TERM);
    }
    /**
     * Moment scaling factor F (page 58)
     * Relates torque differential to yaw moment through track width and effective radius
     */
    static constexpr float F = (vd_constants::TRACK_WIDTH_m / ((WHEEL_DIAMETER_IN / 2.0f) * IN_TO_M)) * GEAR_RATIO;
};

struct ControlOutput
{
    const float fl_kappa;
    const float fr_kappa;
    const float rl_kappa;
    const float rr_kappa;
    const float max_torque;
    const float min_torque;
};
struct ControlOutputAutonomous
{
    const float fl_kappa;
    const float fr_kappa;
    const float rl_kappa;
    const float rr_kappa;
    const float delta;
    const float max_torque;
    const float min_torque;
};
} // namespace app::tv::shared_datatypes
