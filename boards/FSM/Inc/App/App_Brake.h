#pragma once

#include <stdbool.h>

struct Brake;

struct Brake *App_Brake_Create(
    float (*get_pressure_psi)(void),
    bool (*is_pressure_sensor_open_or_short_circuit_binary_status)(void),
    bool (*is_brake_actuated)(void),
    float min_pressure_psi,
    float max_pressure_psi);

struct InRangeCheck *
    App_Brake_GetPressureInRangeCheck(const struct Brake *const brake);

struct BinaryStatus *
    App_Brake_GetIsBrakeActuatedBinaryStatus(const struct Brake *brake);

struct BinaryStatus *
    App_Brake_GetIsPressureSensorOpenOrShortCircuitBinaryStatus(
        const struct Brake *brake);

void App_Brake_Destroy(struct Brake *brake);
