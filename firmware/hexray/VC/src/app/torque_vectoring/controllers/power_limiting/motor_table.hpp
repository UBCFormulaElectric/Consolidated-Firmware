// Compressed motor table: float32 baseline (row 0) + int16 cumulative deltas.
// Temperatures: 80C (id 0), 100C (id 1), 120C (id 2)
// Speed axis: 0-20000 rpm, 200 rpm steps, 101 entries
// Current axis: 0-105 A, 5.25 A steps, 21 entries

#pragma once
#include <stdint.h>
#include "app_units.h"

namespace MotorTable
{

static constexpr uint8_t  N_TEMP    = 3;
static constexpr uint16_t N_SPEED   = 101;
static constexpr uint8_t  N_CURRENT = 21;

extern const float   SHAFT_TORQUE_BASELINE[N_TEMP][N_CURRENT];
extern const int16_t SHAFT_TORQUE_DELTA[N_TEMP][N_SPEED - 1][N_CURRENT];
extern const float   TOTAL_LOSS_BASELINE[N_TEMP][N_CURRENT];
extern const int16_t TOTAL_LOSS_DELTA[N_TEMP][N_SPEED - 1][N_CURRENT];

float predict_line_current(float speed_rpm, float torque_Nm, float temp_C);
float predict_dc_current(float speed_rpm, float torque_Nm, float temp_C, float dc_bus_voltage_V);
float lookup_torque(float speed_rpm, float current_A, float temp_C);
} // namespace MotorTable