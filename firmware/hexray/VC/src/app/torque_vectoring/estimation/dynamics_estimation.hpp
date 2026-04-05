#pragma once

#include "torque_vectoring/shared_datatypes/constants.hpp"
#include "torque_vectoring/shared_datatypes/datatypes.hpp"

namespace app::tv::estimation
{
using namespace app::tv::datatypes::vd_constants;
class vehicleDynamics
{
public:

    [[nodiscard]] app::tv::datatypes::datatypes::wheel_set estimateNormalForce_N(float a_x_MPS2, float a_y_MPS2, float v_x_mps) const;
    [[nodiscard]] float estimateBodySlip(float v_x_mps, float v_y_mps) const;
    [[nodiscard]] float estimateYawMoment_Nm(
        const app::tv::datatypes::datatypes::wheel_set& longitudinal_forces_N,
        const app::tv::datatypes::datatypes::wheel_set& lateral_forces_N,
        float steering_angle_rad) const;
    [[nodiscard]] float estimateDrag_N(float v_x_mps) const;
    [[nodiscard]] float estimateDownForce_N(float v_x_mps) const;

private:

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
    [[nodiscard]] static inline constexpr float LONG_ACCEL_TERM_VERTICAL_FORCE(const float long_accel)
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
    [[nodiscard]] static inline constexpr float LAT_ACCEL_TERM_VERTICAL_FORCE(const float lat_accel)
    {
        return (CAR_MASS_AT_CG_KG * lat_accel * DIST_HEIGHT_CG_m) / (2.0f * TRACK_WIDTH_m);
    }

    [[nodiscard]] static inline constexpr float REAR_RIGHT_WHEEL_VERTICAL_FORCE(const float long_accel, const float lat_accel)
    {
        return REAR_WEIGHT_DISTRIBUTION + LONG_ACCEL_TERM_VERTICAL_FORCE(long_accel / 4.0f) +
            LAT_ACCEL_TERM_VERTICAL_FORCE(lat_accel);
    }

    [[nodiscard]] static inline constexpr float REAR_LEFT_WHEEL_VERTICAL_FORCE(const float long_accel, const float lat_accel)
    {
        return REAR_WEIGHT_DISTRIBUTION + LONG_ACCEL_TERM_VERTICAL_FORCE(long_accel / 4.0f) -
            LAT_ACCEL_TERM_VERTICAL_FORCE(lat_accel);
    }

    // TODO: Check if front wheels use rear weight or front weight distribution
    [[nodiscard]] static inline constexpr float FRONT_RIGHT_WHEEL_VERTICAL_FORCE(const float long_accel, const float lat_accel)
    {
        return REAR_WEIGHT_DISTRIBUTION + LONG_ACCEL_TERM_VERTICAL_FORCE(long_accel / 4.0f) +
            LAT_ACCEL_TERM_VERTICAL_FORCE(lat_accel);
    }

    [[nodiscard]] static inline constexpr float FRONT_LEFT_WHEEL_VERTICAL_FORCE(const float long_accel, const float lat_accel)
    {
        return REAR_WEIGHT_DISTRIBUTION - LONG_ACCEL_TERM_VERTICAL_FORCE(long_accel / 4.0f) -
            LAT_ACCEL_TERM_VERTICAL_FORCE(lat_accel);
    }

    /**
    * Yaw moment distribution factor Kmz (page 57)
    * Accounts for load transfer effect on yaw moment generation capacity
    *
    * @param long_accel Longitudinal acceleration (m/s^2)
    * @return Effective moment arm (m)
    */
    [[nodiscard]] static inline constexpr float ACCELERATION_TERM_KMZ(const float long_accel)
    {
        return DIST_FRONT_AXLE_CG_m + (long_accel * DIST_HEIGHT_CG_m) / GRAVITY;
    }

    [[nodiscard]] static inline constexpr float KMZ(const float long_accel)
    {
        float LONG_ACCEL_TERM = ACCELERATION_TERM_KMZ(long_accel);
        return ((CAR_WEIGHT - (CAR_WEIGHT / WHEELBASE_m) * LONG_ACCEL_TERM)) / ((CAR_WEIGHT / WHEELBASE_m) * LONG_ACCEL_TERM);
    }

    /**
    * Moment scaling factor F (page 58)
    * Relates torque differential to yaw moment through track width and effective radius
    */
    static constexpr float F = (TRACK_WIDTH_m / ((WHEEL_DIAMETER_IN / 2.0f) * IN_TO_M)) * GEAR_RATIO;

    static constexpr float STATIC_FRONT_AXLE_LOAD_N = datatypes::vd_constants::CAR_WEIGHT *
                                                      (datatypes::vd_constants::DIST_REAR_AXLE_CG_m /
                                                       datatypes::vd_constants::WHEELBASE_m);
    static constexpr float STATIC_REAR_AXLE_LOAD_N = datatypes::vd_constants::CAR_WEIGHT *
                                                     (datatypes::vd_constants::DIST_FRONT_AXLE_CG_m /
                                                      datatypes::vd_constants::WHEELBASE_m);
    static constexpr float STATIC_FRONT_WHEEL_LOAD_N = 0.5f * STATIC_FRONT_AXLE_LOAD_N;
    static constexpr float STATIC_REAR_WHEEL_LOAD_N  = 0.5f * STATIC_REAR_AXLE_LOAD_N;

    [[nodiscard]] static float dynamicCopFront(float long_accel_mps2);
    [[nodiscard]] static float dynamicCopRight(float lat_accel_mps2);
};
} // namespace app::tv::estimation
