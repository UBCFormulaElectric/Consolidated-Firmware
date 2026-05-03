#include "motor_table.hpp"
#include <algorithm>
#include <cmath>

namespace MotorTable
{

// HELPERS

// Linear interpolation
static inline float lerp(float x, float y, float t)
{
    return x + t * (y - x);
}

// Bilinear interpolation in (speed, current) at a fixed temperature index

static float interp2d(const float table[N_SPEED][N_CURRENT], float speed_rpm, float current_A)
{
    // Speed index
    float speed_clamped   = std::clamp(speed_rpm, 0.0f, SPEED_MAX_RPM);
    float current_clamped = std::clamp(current_A, 0.0f, CURRENT_MAX_A);

    float speed_table_id   = speed_clamped / SPEED_STEP_RPM;
    float current_table_id = current_clamped / CURRENT_STEP_A;

    // floor to get lower bounding index
    int speed_id_0   = (int)speed_table_id;
    int current_id_0 = (int)current_table_id;

    speed_id_0   = std::min(speed_id_0, (int)N_SPEED - 2);
    current_id_0 = std::min(current_id_0, (int)N_CURRENT - 2);

    int speed_id_1   = speed_id_0 + 1;
    int current_id_1 = current_id_0 + 1;

    float speed_frac   = speed_table_id - speed_id_0;
    float current_frac = current_table_id - current_id_0;

    float torque_00 = table[speed_id_0][current_id_0];
    float torque_01 = table[speed_id_0][current_id_1];
    float torque_10 = table[speed_id_1][current_id_0];
    float torque_11 = table[speed_id_1][current_id_1];

    return lerp(lerp(torque_00, torque_01, current_frac), lerp(torque_10, torque_11, current_frac), speed_frac);
}

// Temperature interpolation index + fraction

static void temp_interp_params(float temp_C, int &temp_lo, int &temp_hi, float &temp_frac)
{
    if (temp_C <= TEMP_POINTS_C[0])
    {
        temp_lo = temp_hi = 0;
        temp_frac         = 0.0f;
        return;
    }
    if (temp_C >= TEMP_POINTS_C[N_TEMP - 1])
    {
        temp_lo = temp_hi = N_TEMP - 1;
        temp_frac         = 0.0f;
        return;
    }
    for (int i = 0; i < (int)N_TEMP - 1; i++)
    {
        if (temp_C < TEMP_POINTS_C[i + 1])
        {
            temp_lo   = i;
            temp_hi   = i + 1;
            temp_frac = (temp_C - TEMP_POINTS_C[i]) / (TEMP_POINTS_C[i + 1] - TEMP_POINTS_C[i]);
            return;
        }
    }
}

// Invert torque table at fixed speed row and temp:
// given desired torque, find the line current that produces it.
// Table is monotonic in current — binary search then interpolate.

static float invert_torque_to_current(const float table[N_SPEED][N_CURRENT], int speed_id, float torque_Nm)
{
    // The table is monotonic in current for most operating points, but may
    // become non-monotonic at high speed and high current (flux weakening region).
    // Find the last index where the table is still monotonically increasing
    // and clamp the torque request to that range before inverting.
    int monotonic_limit = 0;
    for (int i = 1; i < N_CURRENT; i++)
    {
        if (table[speed_id][i] <= table[speed_id][i - 1])
            break;
        monotonic_limit = i;
    }

    float t_min     = table[speed_id][0];
    float t_max     = table[speed_id][monotonic_limit];
    float t_clamped = std::clamp(torque_Nm, t_min, t_max);

    // Binary search for bracketing current indices within monotonic range
    int lo = 0, hi = monotonic_limit;
    while (hi - lo > 1)
    {
        int mid = (lo + hi) / 2;
        if (table[speed_id][mid] <= t_clamped)
        {
            lo = mid;
        }
        else
        {
            hi = mid;
        }
    }

    float torque_at_id_lo = table[speed_id][lo];
    float torque_at_id_hi = table[speed_id][hi];

    // Interpolates between the two bracketing current values to find the
    // exact current that produces the requested torque.
    float frac = (torque_at_id_hi > torque_at_id_lo)
                     ? (t_clamped - torque_at_id_lo) / (torque_at_id_hi - torque_at_id_lo)
                     : 0.0f;

    return (lo + frac) * CURRENT_STEP_A;
}

// Given wheel speed, requested torque, and motor temperature,
// returns predicted line current RMS (A) the motor will draw.
float predict_line_current_A(float speed_rpm, float torque_Nm, float temp_C)
{
    int   temp_lo, temp_hi;
    float temp_frac;
    temp_interp_params(temp_C, temp_lo, temp_hi, temp_frac); // find which temperature tables to look at

    // Speed index for inversion (use nearest row — inversion is per row)
    float speed_clamped = std::clamp(speed_rpm, 0.0f, SPEED_MAX_RPM);
    int   speed_id      = (int)(speed_clamped / SPEED_STEP_RPM);
    speed_id            = std::min(speed_id, (int)N_SPEED - 2);

    float current_at_temp_0 = invert_torque_to_current(SHAFT_TORQUE_NM[temp_lo], speed_id, torque_Nm);
    float current_at_temp_1 = invert_torque_to_current(SHAFT_TORQUE_NM[temp_hi], speed_id, torque_Nm);

    return lerp(current_at_temp_0, current_at_temp_1, temp_frac);
}

// Given wheel speed, requested torque, and motor temperature,
// returns predicted DC bus current (A) contribution from this motor.
// Uses: I_DC = (T * omega + Total_Loss) / V_DC

float predict_dc_current_A(float speed_rpm, float torque_Nm, float temp_C, float dc_bus_voltage_V)
{
    // Get predicted line current to find operating point
    float I_line = predict_line_current_A(speed_rpm, torque_Nm, temp_C);

    // Get total loss at this operating point via forward lookup
    int   temp_lo, temp_hi;
    float temp_frac;
    temp_interp_params(temp_C, temp_lo, temp_hi, temp_frac);

    float loss_temp_lo = interp2d(TOTAL_LOSS_W[temp_lo], speed_rpm, I_line);
    float loss_temp_hi = interp2d(TOTAL_LOSS_W[temp_hi], speed_rpm, I_line);
    float total_loss_W = lerp(loss_temp_lo, loss_temp_hi, temp_frac);

    // Mechanical power = torque * angular velocity
    float omega_rad_s = speed_rpm * RPM_TO_RAD_PER_SEC;
    float P_mech_W    = torque_Nm * omega_rad_s;

    float P_elec_W = P_mech_W + total_loss_W;
    return P_elec_W / dc_bus_voltage_V;
}

// Forward lookup: given speed and line current, returns shaft torque.
// Useful for validation and round-trip testing.
float lookup_torque_Nm(float speed_rpm, float current_A, float temp_C)
{
    int   temp_lo, temp_hi;
    float temp_frac;
    temp_interp_params(temp_C, temp_lo, temp_hi, temp_frac);

    float torque_temp_lo = interp2d(SHAFT_TORQUE_NM[temp_lo], speed_rpm, current_A);
    float torque_temp_hi = interp2d(SHAFT_TORQUE_NM[temp_hi], speed_rpm, current_A);

    return lerp(torque_temp_lo, torque_temp_hi, temp_frac);
}

} // namespace MotorTable
