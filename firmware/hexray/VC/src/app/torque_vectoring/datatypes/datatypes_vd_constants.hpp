#pragma once

#include "util_units.hpp"

namespace app::tv::datatypes::vd_constants
{

// =============================================================================
// PHYSICAL CONSTANTS
// =============================================================================

inline constexpr float GRAVITY       = 9.81f;     // m/s^2
inline constexpr float SMALL_EPSILON = 0.000001f; // Numerical stability for division

// =============================================================================
// VEHICLE DIMENSIONS
// =============================================================================

inline constexpr float WHEELBASE_mm = 1550.0f;
inline constexpr float WHEELBASE_m  = WHEELBASE_mm * MM_TO_M;

inline constexpr float TRACK_WIDTH_mm = 1100.0f;
inline constexpr float TRACK_WIDTH_m  = TRACK_WIDTH_mm * MM_TO_M;

// =============================================================================
// VEHICLE MASS & CENTER OF GRAVITY
// =============================================================================

inline constexpr float CAR_MASS_AT_CG_KG = 300.0f; // Mass with driver (verified with suspension team)

inline constexpr float DIST_FRONT_AXLE_CG_m = 0.837f; // Distance from front axle to CG (parameter 'a')
inline constexpr float DIST_REAR_AXLE_CG_m =
    WHEELBASE_m - DIST_FRONT_AXLE_CG_m;                    // Distance from rear axle to CG (parameter 'b')
inline constexpr float DIST_HEIGHT_CG_m = 30.0f * CM_TO_M; // CG height (from suspension team)

// Derived weight distribution properties
inline constexpr float WEIGHT_ACROSS_BODY        = CAR_MASS_AT_CG_KG * GRAVITY / WHEELBASE_m;
inline constexpr float REAR_WEIGHT_DISTRIBUTION  = WEIGHT_ACROSS_BODY * DIST_REAR_AXLE_CG_m;
inline constexpr float FRONT_WEIGHT_DISTRIBUTION = WEIGHT_ACROSS_BODY * DIST_FRONT_AXLE_CG_m;

// =============================================================================
// MOTOR & POWERTRAIN SPECIFICATIONS
// =============================================================================

// AMK DD5-14-10-POW Motor Specifications
inline constexpr float MAX_TORQUE_REQUEST_NM     = 20.5f;  // Safety limit (actual max is 21 Nm)
inline constexpr float NOMINAL_TORQUE_REQUEST_NM = 9.8f;   // Nominal continuous torque
inline constexpr float MAX_REGEN_Nm              = -15.0f; // Maximum regenerative braking torque

inline constexpr uint16_t POWER_TO_TORQUE_CONVERSION_FACTOR = 9550; // 60/(2*pi)*1000 for T = P/ω

// =============================================================================
// POWER & THERMAL LIMITS
// =============================================================================

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
// CONTROL SYSTEM PARAMETERS
// =============================================================================

// TODO: these probably are legacy and not needed
// PID Control Parameters
inline constexpr float PID_TIMEOUT_ms       = 1000.0f; // PID timeout period
inline constexpr float PID_POWER_FACTOR_MIN = -0.9f;   // TODO: May need adjustment
inline constexpr float PID_POWER_FACTOR_MAX = 0.1f;    // TODO: May need adjustment

// =============================================================================
// TIRE & TRACTION PARAMETERS
// =============================================================================

inline constexpr float SLIP_RATIO_IDEAL = 0.05f; // Ideal slip ratio for maximum traction
inline constexpr float APPROX_STEERING_TO_WHEEL_ANGLE =
    0.3f; // TODO: Replace with reverse/anti-Ackermann model
          // Note: Underestimate for wheel angles > 40° (see Confluence/Steering System)

// =============================================================================
// UTILITY FUNCTIONS & CONVERSION HELPERS
// =============================================================================

/**
 * Motor Torque Command Conversion
 *
 * DD5-14-10-POW motors accept percentage of nominal torque (not absolute torque).
 * 100% = 9.8 Nm (nominal), but motors can output up to 21 Nm (not sustained).
 * Input: torque in Nm
 * Output: int16_t representing (torque/nominal) * 1000
 */
[[nodiscard]] inline constexpr int16_t MOTOR_TORQUE_REQUEST(const float torque)
{
    return static_cast<int16_t>((torque / NOMINAL_TORQUE_REQUEST_NM) * 1000.0f);
}

/**
 * Convert torque and RPM to power (kW)
 */
[[nodiscard]] inline constexpr float TORQUE_TO_POWER(const float torque, const float rpm)
{
    return (torque * (rpm / GEAR_RATIO)) / static_cast<float>(POWER_TO_TORQUE_CONVERSION_FACTOR);
}

/**
 * Convert power (kW) and RPM to torque (Nm)
 * Includes safety guard against division by zero
 */
[[nodiscard]] inline constexpr float POWER_TO_TORQUE(const float power, const float rpm)
{
    return (power * static_cast<float>(POWER_TO_TORQUE_CONVERSION_FACTOR)) / (std::fmax(rpm, 0.00001f) / GEAR_RATIO);
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
[[nodiscard]] inline constexpr float LONG_ACCEL_TERM_VERTICAL_FORCE(const float long_accel)
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
[[nodiscard]] inline constexpr float LAT_ACCEL_TERM_VERTICAL_FORCE(const float lat_accel)
{
    return (CAR_MASS_AT_CG_KG * lat_accel * DIST_HEIGHT_CG_m) / (2.0f * TRACK_WIDTH_m);
}

[[nodiscard]] inline constexpr float REAR_RIGHT_WHEEL_VERTICAL_FORCE(const float long_accel, const float lat_accel)
{
    return REAR_WEIGHT_DISTRIBUTION + LONG_ACCEL_TERM_VERTICAL_FORCE(long_accel / 4.0f) +
           LAT_ACCEL_TERM_VERTICAL_FORCE(lat_accel);
}

[[nodiscard]] inline constexpr float REAR_LEFT_WHEEL_VERTICAL_FORCE(const float long_accel, const float lat_accel)
{
    return REAR_WEIGHT_DISTRIBUTION + LONG_ACCEL_TERM_VERTICAL_FORCE(long_accel / 4.0f) -
           LAT_ACCEL_TERM_VERTICAL_FORCE(lat_accel);
}

// TODO: Check if front wheels use rear weight or front weight distribution
[[nodiscard]] inline constexpr float FRONT_RIGHT_WHEEL_VERTICAL_FORCE(const float long_accel, const float lat_accel)
{
    return REAR_WEIGHT_DISTRIBUTION + LONG_ACCEL_TERM_VERTICAL_FORCE(long_accel / 4.0f) +
           LAT_ACCEL_TERM_VERTICAL_FORCE(lat_accel);
}

[[nodiscard]] inline constexpr float FRONT_LEFT_WHEEL_VERTICAL_FORCE(const float long_accel, const float lat_accel)
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
[[nodiscard]] inline constexpr float ACCELERATION_TERM_KMZ(const float long_accel)
{
    return DIST_FRONT_AXLE_CG_m + (long_accel * DIST_HEIGHT_CG_m) / GRAVITY;
}

/**
 * Moment scaling factor F (page 58)
 * Relates torque differential to yaw moment through track width and effective radius
 */
inline constexpr float F = (TRACK_WIDTH_m / ((WHEEL_DIAMETER_IN / 2.0f) * 2.54f)) * GEAR_RATIO;

// =============================================================================
// EXTERNAL CONFIGURATION (Commented Out)
// =============================================================================

// extern const PID_Config PID_POWER_CORRECTION_CONFIG;
// extern const PID_Config PID_TRACTION_CONTROL_CONFIG;
// extern const PID_Config PID_YAW_RATE_CONTROLLER_CONFIG;
// extern const YawRateController_Config YAW_RATE_CONTROLLER_CONFIG;
} // namespace app::tv::datatypes::vd_constants