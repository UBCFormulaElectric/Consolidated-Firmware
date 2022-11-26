#pragma once

#include <stdbool.h>
#include "App_FsmWorld.h"

struct Coolant;

/**
 *
 * @param get_flow_rate
 * @param get_temperature_A
 * @param temperatureA_OCSC
 * @param get_temperature_B
 * @param temperatureB_OCSC
 * @param get_pressure_A
 * @param pressureA_OCSC
 * @param get_pressure_B
 * @param pressureB_OCSC
 * @return
 */
struct Coolant *App_Coolant_Create(
    float (*get_flow_rate)(void),
    float (*get_temperature_A)(void),
    bool (*temperatureA_OCSC)(void),
    float (*get_temperature_B)(void),
    bool (*temperatureB_OCSC)(void),
    float (*get_pressure_A)(void),
    bool (*pressureA_OCSC)(void),
    float (*get_pressure_B)(void),
    bool (*pressureB_OCSC)(void));

void App_Coolant_Destroy(struct Coolant *coolant);
void App_Coolant_Broadcast(const struct FsmWorld *world);
