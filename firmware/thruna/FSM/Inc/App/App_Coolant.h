#pragma once

#include <stdbool.h>
#include "App_FsmWorld.h"

struct Coolant;

/**
 * Creates a Coolant Object, and passes ownership to the caller.
 * @param get_flow_rate Function which returns the flow rate of the coolant
 * @param get_temperature_A Function which returns the temperature at point A of the coolant
 * @param get_temperature_B Function which returns the temperature at point B of the coolant
 * @param get_pressure_A Function which returns the pressure at point A of the coolant
 * @param get_pressure_B Function which returns the pressure at point B of the coolant
 * @return Coolant Object
 */
struct Coolant *App_Coolant_Create(
    float (*get_flow_rate)(void),
    float (*get_temperature_A)(void),
    float (*get_temperature_B)(void),
    float (*get_pressure_A)(void),
    float (*get_pressure_B)(void));

void App_Coolant_Destroy(struct Coolant *coolant);
void App_Coolant_Broadcast(const struct FsmWorld *world);
