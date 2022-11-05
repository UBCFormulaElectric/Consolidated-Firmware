#pragma once

#include <stdbool.h>
#include "App_FsmWorld.h"

struct Coolant;

/**
 * Creates a coolant object
 * @param get_flow_rate
 * @param get_temperature_A
 * @param get_temperature_B
 * @param get_pressure_A
 * @param get_pressure_B
 * @return
 */
struct Coolant *App_Coolant_Create(
    float (*get_flow_rate)(void),
    float (*get_temperature_A)(void),
    float (*get_temperature_B)(void),
    float (*get_pressure_A)(void),
    float (*get_pressure_B)(void));

void App_Coolant_Destroy(struct Coolant *coolant);
void App_Coolant_Broadcast(const struct FsmWorld * world, bool *coolantTriggerShutdown);
