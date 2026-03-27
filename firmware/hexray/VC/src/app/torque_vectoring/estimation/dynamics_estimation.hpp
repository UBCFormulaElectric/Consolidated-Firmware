#pragma once

#include "torque_vectoring/shared_datatypes/constants.hpp"

namespace app::tv::estimation
{
class vehicleDynamics
{
public:
    struct normal_force_t
    {
        float fz_fl_N = 0.0f;
        float fz_fr_N = 0.0f;
        float fz_rl_N = 0.0f;
        float fz_rr_N = 0.0f;
    };

    [[nodiscard]] normal_force_t estimateNormalForce_N(float a_x_MPS2, float a_y_MPS2, float v_x_mps) const;
    [[nodiscard]] float estimateBodySlip(float v_x_mps, float v_y_mps) const;
    [[nodiscard]] float estimateDrag_N(float v_x_mps) const;

private:
    static constexpr float FRONTAL_AREA_M2    = 0.94f;   // m^2 from aero team
    static constexpr float AIR_DENSITY_KGPM3  = 1.2205f; // kg/m^3
    static constexpr float LIFT_COEFF         = 1.7f;    // from aero team
    static constexpr float DRAG_COEFF         = 0.92f;
    static constexpr float COP_REAR           = 0.68f;   // fraction of aero load acting behind the CG
    static constexpr float COP_RIGHT          = 0.5f;    // fraction of aero load acting on the right side
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
    [[nodiscard]] float estimateDownForce_N(float v_x_mps) const;
};
} // namespace app::tv::estimation
