#pragma once

#include <stdint.h>
#include <stdbool.h>

struct APPS;

struct APPS *App_APPS_Create(
    uint32_t max_counter_val,
    uint32_t max_pressed_counter_value,
    float    percent_deflection,
    uint32_t (*get_counter_value)(void),
    bool (*is_brake_actuated)(void));

float App_APPS_GetPedalPercentage(const struct APPS *apps);

void App_APPS_Destroy(struct APPS *apps);
