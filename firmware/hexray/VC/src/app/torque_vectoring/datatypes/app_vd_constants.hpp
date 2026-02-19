#pragma once
#include "app_pid.h"
#include "app_units.h"
#include "math.h"

// Include generated CAN utils first to ensure enums are defined before macros
// This prevents macro/enum conflicts (SWITCH_OFF, DRIVE_MODE_POWER, etc.)
#include "app_canUtils.h"

// Constants

constexpr double CAR_MASS_AT_CG_KG = 300.0; // checked with suspension -- weight with driver
constexpr double GRAVITY           = 9.81;
// got 30cm from suspension team
constexpr double CG_HEIGHT_FROM_GROUND_M = static_cast<double>(30) * static_cast<double>(CM_TO_M);
// TODO: Will be used by rest of the team
// constexpr double SMALL_EPSILON = 0.1; // to avoid divide by zero error
constexpr int    WHEELBASE_mm   = 1550;
constexpr double WHEELBASE_m    = static_cast<double>(WHEELBASE_mm) * static_cast<double>(MM_TO_M);
constexpr int    TRACK_WIDTH_mm = 1100;
constexpr double TRACK_WIDTH_m  = static_cast<double>(TRACK_WIDTH_mm) * static_cast<double>(MM_TO_M);
// 60 / (2 * pi) * 1000 to for T = P/w formula
constexpr int POWER_TO_TORQUE_CONVERSION_FACTOR = 9550;
// TODO: Will be used by rest of the team
// max power = 80.0 we can use by FSAE rules
// constexpr double POWER_LIMIT_CAR_kW = 40.0;
// constexpr double MOTOR_TEMP_CUTOFF_c = 90.0;
// amount to decrease max power by for every degree celsius over motor cutoff temp (we got this number by looking at the
// emrax 188 manual)
// constexpr double MOTOR_TEMP_POWER_DECREMENTING_RATIO = 80.0 / 30.0;
// constexpr int PID_TIMEOUT_ms = 1000; // Time after witch PID values
// TODO(akoen): Power factors limits may need to be adjusted
// constexpr double PID_POWER_FACTOR_MIN = -0.9;
// constexpr double PID_POWER_FACTOR_MAX = 0.1;
constexpr double APPROX_STEERING_TO_WHEEL_ANGLE =
    0.3; // Underestimate for wheel angles > 40deg. See Confluence/Steering System
// TODO: Will be used by rest of the team
// constexpr double MAX_TORQUE_REQUEST_NM = 20.5; // Max torque of AMK DD5-14-10-POW motor (actual max is 21, using this
// as safety)
constexpr double NOMINAL_TORQUE_REQUEST_NM = 9.8; // Nominal torque of AMK DD5-14-10-POW motor
// constexpr double RULES_BASED_POWER_LIMIT_KW = 80.0;
// constexpr double MAX_REGEN_Nm = -15.0;
// constexpr int MAX_BATTERY_TEMP = 45;
// constexpr double POWER_LIMIT_REGEN_kW = 17.0; // 17.64kW ~ 30A charge for molicel cells

/**
 * Reference used: https://www.zotero.org/groups/5809911/vehicle_controls_2024/items/N4TQBR67/reader
 */

constexpr double DIST_FRONT_AXLE_CG = 0.7; // a in meters
// TODO: Will be used by rest of the team
// constexpr double DIST_REAR_AXLE_CG = WHEELBASE_m - DIST_FRONT_AXLE_CG; // b in meters
// constexpr double WEIGHT_ACROSS_BODY = CAR_MASS_AT_CG_KG * GRAVITY / WHEELBASE_m;

/************** Constants and functions for finding normal forces on wheels based on diagram on page 21
 * ****************/
// TODO: Will be used by rest of the team
// constexpr double REAR_CONST_LOAD = WEIGHT_ACROSS_BODY * DIST_FRONT_AXLE_CG / 2.0; // per wheel on axle
// constexpr double FRONT_CONST_LOAD = WEIGHT_ACROSS_BODY * DIST_REAR_AXLE_CG / 2.0; // per wheel on axle
constexpr double LONG_LOAD_TRANSFER(double long_accel)
{
    return CAR_MASS_AT_CG_KG * long_accel * CG_HEIGHT_FROM_GROUND_M / WHEELBASE_m;
}
constexpr double LAT_LOAD_TRANSFER(double lat_accel)
{
    return CAR_MASS_AT_CG_KG * lat_accel * CG_HEIGHT_FROM_GROUND_M / (2.0 * TRACK_WIDTH_m);
}

/************** Function for finding Kmz based on diagram on page 57 ****************/
constexpr double ACCELERATION_TERM_KMZ(double long_accel)
{
    return DIST_FRONT_AXLE_CG + long_accel * CG_HEIGHT_FROM_GROUND_M / GRAVITY;
}

/************** Functions for moment scaling on page 58 ****************/

// Our motors no longer take in a torque command, instead they take in a percentage value. The percentage for the
// DD5-14-10-POW motor is relative to their nominal torque (9.8 Nm) where 100% torque is 9.8. The motors are able to
// output up to 21 Nm, this however cannot be a sustained behaviour. Note the message takes a int 16 that is essentially
// your percentage of nominal torque * 1000
constexpr int16_t NM_TO_INVERTER_TORQUE(double torque)
{
    return static_cast<int16_t>((torque / NOMINAL_TORQUE_REQUEST_NM) * 1000.0);
}
constexpr double TORQUE_TO_POWER(double torque, double rpm)
{
    return torque * (rpm / static_cast<double>(GEAR_RATIO)) / static_cast<double>(POWER_TO_TORQUE_CONVERSION_FACTOR);
}
constexpr double POWER_TO_TORQUE(double power, double rpm)
{
    // Doing this for no division by 0, and assuming rpm is always positive
    return (power * static_cast<double>(POWER_TO_TORQUE_CONVERSION_FACTOR)) /
           (fmax(rpm, 0.1) / static_cast<double>(GEAR_RATIO));
}

// Eqn 14.11. from RCVD
constexpr double SLIP_RATIO(double wheel_speed, double vehicle_speed, double slip_angle)
{
    double vehicle_speed_cos = vehicle_speed * cos(slip_angle);
    return (wheel_speed - vehicle_speed_cos) / vehicle_speed_cos;
}

// TODO: Will be used by rest of the team
// constexpr double SLIP_RATIO_IDEAL = 0.05;

// Switch state enum values (from CAN enum SwitchState)
// Only define if not already defined (e.g., by generated enum)
// TODO: Will be used by rest of the team
// #ifndef SWITCH_OFF
// constexpr int SWITCH_OFF = 0;
// #endif
// #ifndef SWITCH_ON
// constexpr int SWITCH_ON = 1;
// #endif

// Drive mode enum values (from CAN enum DriveMode)
// Only define if not already defined (e.g., by generated enum)
// TODO: Will be used by rest of the team
// #ifndef DRIVE_MODE_POWER
// constexpr int DRIVE_MODE_POWER = 0;
// #endif
// #ifndef DRIVE_MODE_POWER_AND_ACTIVE
// constexpr int DRIVE_MODE_POWER_AND_ACTIVE = 1;
// #endif
// TODO: Will be used by rest of the team
// #ifndef DRIVE_MODE_TV
// constexpr int DRIVE_MODE_TV = 2;
// #endif
// #ifndef DRIVE_MODE_COUNT
// constexpr int DRIVE_MODE_COUNT = 3;
// #endif
