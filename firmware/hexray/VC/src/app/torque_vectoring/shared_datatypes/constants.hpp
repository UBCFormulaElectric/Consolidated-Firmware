#pragma once

#include "torque_limits.hpp"
#include "util_units.hpp"
#include <cmath>

namespace app::tv::shared_datatypes::vd_constants
{
// =============================================================================
// PHYSICAL CONSTANTS
// =============================================================================

inline constexpr float GRAVITY       = 9.81f;     // m/s^2
inline constexpr float SMALL_EPSILON = 0.000001f; // Numerical stability for division
// Floor on the longitudinal speed used as the denominator of slip ratio / slip angle. Below this, tiny lateral
// velocities would otherwise produce huge slip angles (atan2(v_y, ~0) -> +-90 deg) and poison the allocator.
inline constexpr float SLIP_REGULARIZATION_SPEED_MPS = 1.0f;
// Braking (negative APPS) requests fade out linearly below this speed so the motors stop the car instead of
// driving it backwards once v_x reaches zero.
inline constexpr float BRAKE_FADE_SPEED_MPS = 2.0f;
inline constexpr float FRONTAL_AREA_M2      = 0.94f;   // m^2 from aero team
inline constexpr float AIR_DENSITY_KGPM3    = 1.2205f; // kg/m^3
inline constexpr float LIFT_COEFF           = 1.7f;    // from aero team
inline constexpr float DRAG_COEFF           = 0.92f;
inline constexpr float COP_REAR             = 0.68f; // fraction of aero load acting behind the CG
inline constexpr float COP_RIGHT            = 0.5f;  // fraction of aero load acting on the right side

// =============================================================================
// VEHICLE DIMENSIONS
// =============================================================================

inline constexpr float WHEELBASE_mm = 1550.0f;
inline constexpr float WHEELBASE_m  = WHEELBASE_mm * MM_TO_M;

inline constexpr float TRACK_WIDTH_mm = 1100.0f;
inline constexpr float TRACK_WIDTH_m  = TRACK_WIDTH_mm * MM_TO_M;
inline constexpr float HALF_TRACK_M   = TRACK_WIDTH_m * 0.5f;
inline constexpr float WHEEL_RADIUS_M = WHEEL_DIAMETER_IN * IN_TO_M / 2.0f;

// =============================================================================
// VEHICLE MASS & CENTER OF GRAVITY
// =============================================================================

inline constexpr float CAR_MASS_CG_NO_DRIVER_KG = 223.7f; // Mass with driver (verified with suspension team)
inline constexpr float DRIVER_MASS_KG           = 70.0f;  // Mass with driver (verified with suspension team)
inline constexpr float CAR_MASS_AT_CG_KG =
    CAR_MASS_CG_NO_DRIVER_KG + DRIVER_MASS_KG; // Mass with driver (verified with suspension team)
// Estimated yaw moment of inertia about CG (TODO: Update with suspension team)
inline constexpr float CAR_YAW_MOMENT_INERTIA_KGM2 = 400.0f;

inline constexpr float DIST_FRONT_AXLE_CG_m = 0.837f; // Distance from front axle to CG (parameter 'a')
inline constexpr float DIST_REAR_AXLE_CG_m =
    WHEELBASE_m - DIST_FRONT_AXLE_CG_m;                    // Distance from rear axle to CG (parameter 'b')
inline constexpr float DIST_HEIGHT_CG_m = 30.0f * CM_TO_M; // CG height (from suspension team)

// Derived weight distribution properties
inline constexpr float CAR_WEIGHT                = CAR_MASS_AT_CG_KG * GRAVITY;
inline constexpr float WEIGHT_ACROSS_BODY        = CAR_MASS_AT_CG_KG * GRAVITY / WHEELBASE_m;
inline constexpr float REAR_WEIGHT_DISTRIBUTION  = WEIGHT_ACROSS_BODY * DIST_REAR_AXLE_CG_m;
inline constexpr float FRONT_WEIGHT_DISTRIBUTION = WEIGHT_ACROSS_BODY * DIST_FRONT_AXLE_CG_m;

// =============================================================================
// POWER & THERMAL LIMITS
// =============================================================================

// TODO: Verify all of these
// Power Limits
inline constexpr float RULES_BASED_POWER_LIMIT_KW = 80.0f; // FSAE maximum allowed power
inline constexpr float POWER_LIMIT_CAR_kW         = 40.0f; // TODO: Update with hexray constants or remove
inline constexpr float POWER_LIMIT_REGEN_kW       = 17.0f; // ~17.64kW = 30A charge for Molicel cells
                                                           // TODO: Verify if per motor or entire accumulator

// Thermal Limits
inline constexpr float MOTOR_TEMP_CUTOFF_c = 90.0f; // Motor temperature cutoff
inline constexpr float MOTOR_TEMP_POWER_DECREMENTING_RATIO =
    80.0f / 30.0f;                               // Power reduction per °C over cutoff (from Emrax 188 manual)
inline constexpr float MAX_BATTERY_TEMP = 45.0f; // TODO: Verify this is current

// =============================================================================
// REGEN ADDITIONAL PARAMETERS
// =============================================================================

inline constexpr float MIN_SPEED_REGEN_KMH = 5.0f;
inline constexpr float REGEN_PEDAL_REGION  = 0.2f;
inline constexpr float PEDAL_DEADZONE      = 0.1f;

// =============================================================================
// WHEEL AND STEERING PARAMETERS
// =============================================================================

inline constexpr float MAX_AX_MPS2 = 9.81f * 1.8f; // TODO idk this number bruh

inline constexpr float STEER_WHEEL_RANGE_rad = 1.48632f;
inline constexpr float STEER_WHEEL_RANGE_deg = RAD_TO_DEG(1.48632f);

// Note: Bump camber is the amount the camber changes in degrees due to compression
inline constexpr float STATIC_CAMBER_FRONT_deg  = -1.0f;
inline constexpr float FRONT_BUMP_CAMBER_deg_mm = 0.02f;

inline constexpr float STATIC_CAMBER_REAR_deg  = -0.75f;
inline constexpr float REAR_BUMP_CAMBER_deg_mm = 0.06f;

// =============================================================================
// EXTERNAL CONFIGURATION (Commented Out)
// =============================================================================

// extern const PID_Config PID_POWER_CORRECTION_CONFIG;
// extern const PID_Config PID_TRACTION_CONTROL_CONFIG;
// extern const PID_Config PID_YAW_RATE_CONTROLLER_CONFIG;
// extern const YawRateController_Config YAW_RATE_CONTROLLER_CONFIG;
} // namespace app::tv::shared_datatypes::vd_constants
