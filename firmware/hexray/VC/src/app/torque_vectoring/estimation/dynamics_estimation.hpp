#pragma once

#include "torque_vectoring/shared_datatypes/constants.hpp"
#include "torque_vectoring/shared_datatypes/datatypes.hpp"

namespace app::tv::estimation
{
using namespace app::tv::shared_datatypes::vd_constants;
namespace vehicleDynamics
{
    [[nodiscard]] app::tv::datatypes::datatypes::wheel_set<float>
                        est_Fz_N(float a_x_MPS2, float a_y_MPS2, float v_x_mps);
    [[nodiscard]] float est_beta_rad(float v_x_mps, float v_y_mps);
    [[nodiscard]] float est_Mz_N(
        const app::tv::datatypes::datatypes::wheel_set<float> &tires_Fx_N,
        const app::tv::datatypes::datatypes::wheel_set<float> &tires_Fy_N,
        float                                                  delta);
    [[nodiscard]] constexpr float est_dragFx_N(const float v_x_mps)
    {
        return 0.5f * AIR_DENSITY_KGPM3 * FRONTAL_AREA_M2 * DRAG_COEFF * v_x_mps * v_x_mps;
    }
    [[nodiscard]] constexpr float est_downforceFz_N(const float v_x_mps)
    {
        return 0.5f * AIR_DENSITY_KGPM3 * FRONTAL_AREA_M2 * LIFT_COEFF * v_x_mps * v_x_mps;
    }

    // =============================================================================
    // VEHICLE DYNAMICS - VERTICAL LOAD TRANSFER
    // Reference: https://www.zotero.org/groups/5809911/vehicle_controls_2024/items/N4TQBR67/reader
    // =============================================================================

    /**
     * Longitudinal load transfer component (page 21)
     * Positive long_accel transfers load to rear axle
     *
     * @param long_accel Longitudinal acceleration (m/s^2)
     * @return Load transfer force (N)
     */
    [[nodiscard]] constexpr float LONG_ACCEL_TERM_VERTICAL_FORCE(const float long_accel)
    {
        return (CAR_MASS_AT_CG_KG * long_accel * DIST_HEIGHT_CG_m) / WHEELBASE_m;
    }
    /**
     * Lateral load transfer component (page 21)
     * Transfers load to outside wheels during cornering
     *
     * @param lat_accel Lateral acceleration (m/s^2)
     * @return Load transfer force per side (N)
     */
    [[nodiscard]] constexpr float LAT_ACCEL_TERM_VERTICAL_FORCE(const float lat_accel)
    {
        return (CAR_MASS_AT_CG_KG * lat_accel * DIST_HEIGHT_CG_m) / (2.0f * TRACK_WIDTH_m);
    }

    [[nodiscard]] constexpr float REAR_RIGHT_WHEEL_VERTICAL_FORCE(const float long_accel, const float lat_accel)
    {
        return REAR_WEIGHT_DISTRIBUTION + LONG_ACCEL_TERM_VERTICAL_FORCE(long_accel / 4.0f) +
               LAT_ACCEL_TERM_VERTICAL_FORCE(lat_accel);
    }
    [[nodiscard]] constexpr float REAR_LEFT_WHEEL_VERTICAL_FORCE(const float long_accel, const float lat_accel)
    {
        return REAR_WEIGHT_DISTRIBUTION + LONG_ACCEL_TERM_VERTICAL_FORCE(long_accel / 4.0f) -
               LAT_ACCEL_TERM_VERTICAL_FORCE(lat_accel);
    }
    [[nodiscard]] constexpr float FRONT_RIGHT_WHEEL_VERTICAL_FORCE(const float long_accel, const float lat_accel)
    {
        return REAR_WEIGHT_DISTRIBUTION + LONG_ACCEL_TERM_VERTICAL_FORCE(long_accel / 4.0f) +
               LAT_ACCEL_TERM_VERTICAL_FORCE(lat_accel);
    }
    [[nodiscard]] constexpr float FRONT_LEFT_WHEEL_VERTICAL_FORCE(const float long_accel, const float lat_accel)
    {
        return REAR_WEIGHT_DISTRIBUTION - LONG_ACCEL_TERM_VERTICAL_FORCE(long_accel / 4.0f) -
               LAT_ACCEL_TERM_VERTICAL_FORCE(lat_accel);
    }

    /**
     * Yaw moment distribution factor Kmz (page 57)
     * Accounts for load transfer effect on yaw moment generation capacity
     *
     * @param a_x Longitudinal acceleration (m/s^2)
     * @return Effective moment arm (m)
     */
    [[nodiscard]] constexpr float ACCELERATION_TERM_KMZ(const float a_x)
    {
        return DIST_FRONT_AXLE_CG_m + (a_x * DIST_HEIGHT_CG_m) / GRAVITY;
    }
    [[nodiscard]] constexpr float KMZ(const float a_x)
    {
        const float LONG_ACCEL_TERM = ACCELERATION_TERM_KMZ(a_x);
        return ((CAR_WEIGHT - (CAR_WEIGHT / WHEELBASE_m) * LONG_ACCEL_TERM)) /
               ((CAR_WEIGHT / WHEELBASE_m) * LONG_ACCEL_TERM);
    }
    /**
     * Moment scaling factor F (page 58)
     * Relates torque differential to yaw moment through track width and effective radius
     */
    inline constexpr float F = (TRACK_WIDTH_m / ((WHEEL_DIAMETER_IN / 2.0f) * IN_TO_M)) * GEAR_RATIO;

    inline constexpr float STATIC_FRONT_AXLE_LOAD_N =
        datatypes::vd_constants::CAR_WEIGHT *
        (datatypes::vd_constants::DIST_REAR_AXLE_CG_m / datatypes::vd_constants::WHEELBASE_m);
    inline constexpr float STATIC_REAR_AXLE_LOAD_N =
        datatypes::vd_constants::CAR_WEIGHT *
        (datatypes::vd_constants::DIST_FRONT_AXLE_CG_m / datatypes::vd_constants::WHEELBASE_m);
    inline constexpr float STATIC_FRONT_WHEEL_LOAD_N = 0.5f * STATIC_FRONT_AXLE_LOAD_N;
    inline constexpr float STATIC_REAR_WHEEL_LOAD_N  = 0.5f * STATIC_REAR_AXLE_LOAD_N;

    [[nodiscard]] constexpr float dynamicCOPFront(const float long_accel_mps2)
    {
        (void)long_accel_mps2;
        return 1.0f - COP_REAR;
    }
    [[nodiscard]] constexpr float dynamicCOPRight(const float lat_accel_mps2)
    {
        (void)lat_accel_mps2;
        return COP_RIGHT;
    }
}; // namespace vehicleDynamics
} // namespace app::tv::estimation
