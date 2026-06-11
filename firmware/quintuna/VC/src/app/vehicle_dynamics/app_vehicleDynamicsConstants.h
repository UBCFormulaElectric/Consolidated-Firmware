#pragma once
#include "app_pid.h"
#include "app_yawRateController.h"
#include "app_units.h"

// Constants

#define CAR_MASS_AT_CG_KG (300.0f) // checked with suspension -- weight with driver
#define GRAVITY (9.81f)
#define CG_HEIGHT_FROM_GROUND_m (30.0f * CM_TO_M) // got 30cm from suspension team
#define SMALL_EPSILON .000001f                    // to avoid divide by zero error
#define SLIP_RATIO_IDEAL 0.05f
#define WHEELBASE_mm 1550
#define WHEELBASE_m (WHEELBASE_mm * MM_TO_M)
#define TRACK_WIDTH_mm 1100
#define TRACK_WIDTH_m (TRACK_WIDTH_mm * MM_TO_M)
#define POWER_TO_TORQUE_CONVERSION_FACTOR 9550 // 60 / (2 * pi) * 1000 to for T = P/w formula
#define POWER_LIMIT_CAR_kW 40.0f               // max power = 80.0f we can use by FSAE rules
#define MOTOR_TEMP_CUTOFF_c 90.0f
#define MOTOR_TEMP_POWER_DECREMENTING_RATIO \
    80.0f / 30.0f // amount to decrease max power by for every degree celsius over motor cutoff temp (we got this number
                  // by looking at the emrax 188 manual)
#define PID_TIMEOUT_ms 1000 // Time after witch PID values
// TODO(akoen): Power factors limits may need to be adjusted
#define PID_POWER_FACTOR_MIN (-0.9f)
#define PID_POWER_FACTOR_MAX 0.1f
#define APPROX_STEERING_TO_WHEEL_ANGLE 0.3f // Underestimate for wheel angles > 40deg. See Confluence/Steering System
#define MAX_TORQUE_REQUEST_NM (20.5f) // Max torque of AMK DD5-14-10-POW motor (actual max is 21, using this as safety)
#define NOMINAL_TORQUE_REQUEST_NM (9.8f) // Nominal torque of AMK DD5-14-10-POW motor
#define RULES_BASED_POWER_LIMIT_KW (80.0f)
#define MAX_REGEN_Nm -15.0f
#define MAX_BATTERY_TEMP 45
#define POWER_LIMIT_REGEN_kW 17.0f // 17.64kW ~ 30A charge for molicel cells
#define FRONTAL_AREA 0.94f         // m^2 from aero team
#define AIR_DENSITY 1.2205f        // kg/m^3
#define COEFFICIENT_OF_LIFT 1.7f   // from Aero team
#define COEFFICIENT_OF_DRAG 1.1f   // from Aero team
#define COP_REAR 0.68f             // from Aero team
#define COP_RIGHT 0.5f             // Assumed symmetric. Aero team has not characterized lateral CoP
#define COP_HEIGHT (42.0f * CM_TO_M)
#define WHEEL_RADIUS_m ((WHEEL_DIAMETER_IN / 2.0f) * 0.0254f)
#define KMH_TO_MS (1.0f / 3.6f)

/**
 * Reference used: https://www.zotero.org/groups/5809911/vehicle_controls_2024/items/N4TQBR67/reader
 */

#define DIST_FRONT_AXLE_CG (0.837f)                          // a in meters
#define DIST_REAR_AXLE_CG (WHEELBASE_m - DIST_FRONT_AXLE_CG) // b in meters
#define WEIGHT_ACROSS_BODY (CAR_MASS_AT_CG_KG * GRAVITY / WHEELBASE_m)
#define DOWNFORCE_CONSTANT (AIR_DENSITY * FRONTAL_AREA * COEFFICIENT_OF_LIFT / 2.0f)
#define DRAG_CONSTANT (AIR_DENSITY * FRONTAL_AREA * COEFFICIENT_OF_DRAG / 2.0f)

/************** Macros for finding vertical forces on wheels based on diagram on page 21 ****************/
#define FRONT_AXLE_WEIGHT_DISTRIBUTION (WEIGHT_ACROSS_BODY * DIST_REAR_AXLE_CG)
#define REAR_AXLE_WEIGHT_DISTRIBUTION (WEIGHT_ACROSS_BODY * DIST_FRONT_AXLE_CG)
#define LONG_ACCEL_TERM_VERTICAL_FORCE(long_accel) \
    ((CAR_MASS_AT_CG_KG * (long_accel) * CG_HEIGHT_FROM_GROUND_m / WHEELBASE_m))
#define LAT_ACCEL_TERM_VERTICAL_FORCE(lat_accel) \
    ((CAR_MASS_AT_CG_KG * (lat_accel) * CG_HEIGHT_FROM_GROUND_m / (2.0f * (TRACK_WIDTH_m))))
#define DOWNFORCE_TERM_VERTICAL_FORCE(vehicle_velocity_kmh) \
    ((DOWNFORCE_CONSTANT) * (vehicle_velocity_kmh * KMH_TO_MS) * (vehicle_velocity_kmh * KMH_TO_MS))

// Longitudinal CoP shift with pitch not modeled — aero team to provide data next year

#define STATIC_COP_FRONT(long_accel) (1 - (COP_REAR))
#define STATIC_COP_REAR(long_accel) (COP_REAR)
#define STATIC_COP_LEFT(lat_accel) (COP_RIGHT)
#define STATIC_COP_RIGHT(lat_accel) (1 - (COP_RIGHT))

#define DRAG_FORCE_NEWTONS(vehicle_velocity_kmh) \
    ((DRAG_CONSTANT) * (vehicle_velocity_kmh * KMH_TO_MS) * (vehicle_velocity_kmh * KMH_TO_MS))
#define DRAG_PITCH_TERM_VERTICAL_FORCE(vehicle_velocity_kmh) \
    ((DRAG_FORCE_NEWTONS(vehicle_velocity_kmh) * COP_HEIGHT / WHEELBASE_m))

// Torque required to counteract resistive drag force per motor (4WD)
#define DRAG_COMPENSATION_TORQUE_NM(vehicle_velocity_kmh) \
    ((DRAG_FORCE_NEWTONS(vehicle_velocity_kmh) * WHEEL_RADIUS_m) / 4.0f)

/************** Macros for finding Kmz based on diagram on page 57 ****************/
#define ACCELERATION_TERM_KMZ(long_accel) (DIST_FRONT_AXLE_CG + (long_accel) * CG_HEIGHT_FROM_GROUND_m / GRAVITY)

/************** Macros for moment scaling on page 58 ****************/
#define F ((TRACK_WIDTH_m / (WHEEL_DIAMETER_IN / 2.0f * 2.54f)) * GEAR_RATIO)

// Our motors no longer take in a torque command, instead they take in a percentage value. The percentage for the
// DD5-14-10-POW motor is relative to their nominal torque (9.8 Nm) where 100% torque is 9.8. The motors are able to
// output up to 21 Nm, this however cannot be a sustained behaviour. Note the message takes a int 16 that is essentially
// your percentage of nominal torque * 1000
#define PEDAL_REMAPPING(torque) ((int16_t)((torque / NOMINAL_TORQUE_REQUEST_NM) * 1000.0f))
#define TORQUE_TO_POWER(torque, rpm) ((torque) * ((float)(rpm) / (GEAR_RATIO)) / (POWER_TO_TORQUE_CONVERSION_FACTOR))
#define POWER_TO_TORQUE(power, rpm)                  \
    (((power) * POWER_TO_TORQUE_CONVERSION_FACTOR) / \
     ((fmaxf(rpm, 0.00001f)) / (GEAR_RATIO))) // Doing this for no division by 0, and assuming rpm is always positive
// Tunable parameters
extern const PID_Config               PID_POWER_CORRECTION_CONFIG;
extern const PID_Config               PID_TRACTION_CONTROL_CONFIG;
extern const PID_Config               PID_YAW_RATE_CONTROLLER_CONFIG;
extern const YawRateController_Config YAW_RATE_CONTROLLER_CONFIG;
