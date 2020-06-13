#pragma once

#include <stdint.h>
#include <stdbool.h>

struct Brake;

struct Brake *App_Brake_Create(
    float min_brake_pressure_psi,
    float max_brake_pressure_psi,
    float (*get_pressure_psi)(void),
    bool (*is_open_or_short_circuit)(void),
    bool (*is_brake_actuated)(void));

void App_Brake_Destroy(struct Brake *brake);

void App_Brake_GetBrakePressureInRangeCheck(
    const struct Brake *brake,
    float *             brake_pressure_psi);

bool App_Brake_IsOpenOrShortCircuit(const struct Brake *brake);

bool App_Brake_IsBrakeActuated(const struct Brake *brake);
