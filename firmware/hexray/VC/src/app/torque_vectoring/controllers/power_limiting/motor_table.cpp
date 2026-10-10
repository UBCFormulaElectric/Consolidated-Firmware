#include "motor_table.hpp"
#include <algorithm>
#include <cmath>

namespace MotorTable
{

// Constants
static constexpr float TEMP_POINTS_C[N_TEMP] = { 80.0f, 100.0f, 120.0f };
static constexpr float SPEED_STEP_RPM        = 200.0f;
static constexpr float CURRENT_STEP_A        = 5.25f;
static constexpr float SPEED_MAX_RPM         = 20000.0f;
static constexpr float CURRENT_MAX_A         = 105.0f;
static const float     RPM_TO_RAD_PER_SEC    = 2.0f * M_PI_F / 60.0f;

// Scale factors for delta decompression
static constexpr float TORQUE_DELTA_SCALE = 0.0002f;
static constexpr float LOSS_DELTA_SCALE   = 0.11f;

// HELPERS

// Linear interpolation
static inline float lerp(float x, float y, float t)
{
    return x + t * (y - x);
}

// Decompress a single speed row from the delta-encoded table.
// row 0 is stored as float32 baseline; rows 1-200 are int16 deltas from row 0.
// Result written into out[N_CURRENT].

static void decompress_torque_row(int temp_id, int speed_id, float out[N_CURRENT])
{
    if (speed_id == 0)
    {
        for (int j = 0; j < N_CURRENT; j++)
            out[j] = SHAFT_TORQUE_BASELINE[temp_id][j];
    }
    else
    {
        for (int j = 0; j < N_CURRENT; j++)
            out[j] =
                SHAFT_TORQUE_BASELINE[temp_id][j] + SHAFT_TORQUE_DELTA[temp_id][speed_id - 1][j] * TORQUE_DELTA_SCALE;
    }
}

static void decompress_loss_row(int temp_id, int speed_id, float out[N_CURRENT])
{
    if (speed_id == 0)
    {
        for (int j = 0; j < N_CURRENT; j++)
            out[j] = TOTAL_LOSS_BASELINE[temp_id][j];
    }
    else
    {
        for (int j = 0; j < N_CURRENT; j++)
            out[j] = TOTAL_LOSS_BASELINE[temp_id][j] + TOTAL_LOSS_DELTA[temp_id][speed_id - 1][j] * LOSS_DELTA_SCALE;
    }
}

// Bilinear interpolation in (speed, current) using decompressed rows.
static float interp2d_torque(int temp_id, float speed_rpm, float current_A)
{
    float speed_clamped   = std::clamp(speed_rpm, 0.0f, SPEED_MAX_RPM);
    float current_clamped = std::clamp(current_A, 0.0f, CURRENT_MAX_A);

    float speed_table_id   = speed_clamped / SPEED_STEP_RPM;
    float current_table_id = current_clamped / CURRENT_STEP_A;

    int speed_id_0   = std::min((int)speed_table_id, (int)N_SPEED - 2);
    int current_id_0 = std::min((int)current_table_id, (int)N_CURRENT - 2);
    int speed_id_1   = speed_id_0 + 1;
    int current_id_1 = current_id_0 + 1;

    float speed_frac   = speed_table_id - (float)speed_id_0;
    float current_frac = current_table_id - (float)current_id_0;

    float row0[N_CURRENT], row1[N_CURRENT];
    decompress_torque_row(temp_id, speed_id_0, row0);
    decompress_torque_row(temp_id, speed_id_1, row1);

    return lerp(
        lerp(row0[current_id_0], row0[current_id_1], current_frac),
        lerp(row1[current_id_0], row1[current_id_1], current_frac), speed_frac);
}

static float interp2d_loss(int temp_id, float speed_rpm, float current_A)
{
    float speed_clamped   = std::clamp(speed_rpm, 0.0f, SPEED_MAX_RPM);
    float current_clamped = std::clamp(current_A, 0.0f, CURRENT_MAX_A);

    float speed_table_id   = speed_clamped / SPEED_STEP_RPM;
    float current_table_id = current_clamped / CURRENT_STEP_A;

    int speed_id_0   = std::min((int)speed_table_id, (int)N_SPEED - 2);
    int current_id_0 = std::min((int)current_table_id, (int)N_CURRENT - 2);
    int speed_id_1   = speed_id_0 + 1;
    int current_id_1 = current_id_0 + 1;

    float speed_frac   = speed_table_id - (float)speed_id_0;
    float current_frac = current_table_id - (float)current_id_0;

    float row0[N_CURRENT], row1[N_CURRENT];
    decompress_loss_row(temp_id, speed_id_0, row0);
    decompress_loss_row(temp_id, speed_id_1, row1);

    return lerp(
        lerp(row0[current_id_0], row0[current_id_1], current_frac),
        lerp(row1[current_id_0], row1[current_id_1], current_frac), speed_frac);
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

// Invert torque table at a fixed speed row and temp:
// given desired torque, find the line current that produces it.
static float invert_torque_to_current(const float row[N_CURRENT], float torque_Nm)
{
    // Find the last monotonically increasing index
    int monotonic_limit = 0;
    for (int i = 1; i < N_CURRENT; i++)
    {
        if (row[i] <= row[i - 1])
            break;
        monotonic_limit = i;
    }

    float t_min     = row[0];
    float t_max     = row[monotonic_limit];
    float t_clamped = std::clamp(torque_Nm, t_min, t_max);

    int lo = 0, hi = monotonic_limit;
    while (hi - lo > 1)
    {
        int mid = (lo + hi) / 2;
        (row[mid] <= t_clamped) ? lo = mid : hi = mid;
    }

    float frac = (row[hi] > row[lo]) ? (t_clamped - row[lo]) / (row[hi] - row[lo]) : 0.0f;

    return ((float)lo + frac) * CURRENT_STEP_A;
}

float predict_line_current(float speed_rpm, float torque_Nm, float temp_C)
{
    int   temp_lo, temp_hi;
    float temp_frac;
    temp_interp_params(temp_C, temp_lo, temp_hi, temp_frac);

    float speed_clamped = std::clamp(speed_rpm, 0.0f, SPEED_MAX_RPM);
    int   speed_id      = std::min((int)(speed_clamped / SPEED_STEP_RPM), (int)N_SPEED - 2);

    float row_lo[N_CURRENT], row_hi[N_CURRENT];
    decompress_torque_row(temp_lo, speed_id, row_lo);
    decompress_torque_row(temp_hi, speed_id, row_hi);

    float current_lo = invert_torque_to_current(row_lo, torque_Nm);
    float current_hi = invert_torque_to_current(row_hi, torque_Nm);

    return lerp(current_lo, current_hi, temp_frac);
}

float predict_dc_current(float speed_rpm, float torque_Nm, float temp_C, float dc_bus_voltage_V)
{
    float I_line = predict_line_current(speed_rpm, torque_Nm, temp_C);

    int   temp_lo, temp_hi;
    float temp_frac;
    temp_interp_params(temp_C, temp_lo, temp_hi, temp_frac);

    float loss_lo      = interp2d_loss(temp_lo, speed_rpm, I_line);
    float loss_hi      = interp2d_loss(temp_hi, speed_rpm, I_line);
    float total_loss_W = lerp(loss_lo, loss_hi, temp_frac);

    float omega_rad_s = speed_rpm * RPM_TO_RAD_PER_SEC;
    float P_mech_W    = torque_Nm * omega_rad_s;
    float P_elec_W    = P_mech_W + total_loss_W;

    return P_elec_W / dc_bus_voltage_V;
}

float lookup_torque(float speed_rpm, float current_A, float temp_C)
{
    int   temp_lo, temp_hi;
    float temp_frac;
    temp_interp_params(temp_C, temp_lo, temp_hi, temp_frac);

    float torque_lo = interp2d_torque(temp_lo, speed_rpm, current_A);
    float torque_hi = interp2d_torque(temp_hi, speed_rpm, current_A);

    return lerp(torque_lo, torque_hi, temp_frac);
}

} // namespace MotorTable
