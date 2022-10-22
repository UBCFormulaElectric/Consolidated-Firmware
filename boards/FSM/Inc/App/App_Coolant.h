#pragma once

#include <stdbool.h>

struct Coolant;

struct Coolant * App_Coolant_Create(
    float (*get_flow_rate)(void),
    float (*get_temperature_A)(void),
    float (*get_temperature_B)(void),
    float (*get_pressure_A)(void),
    float (*get_pressure_B)(void)
);

struct InRangeCheck *App_Coolant_GetFlowInRangeCheck(struct Coolant *coolant);
void App_Coolant_Destroy(struct Coolant *coolant);
