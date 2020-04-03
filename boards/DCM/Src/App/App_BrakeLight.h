#pragma once

#include <stdbool.h>

enum BrakeLight_Status
{
    BRAKE_LIGHT_ON,
    BRAKE_LIGHT_OFF,
};

struct BrakeLight* App_BrakeLight_Create(
    bool (*is_brake_actuated)(void),
    bool (*is_regen_active)(void),
    void (*turn_on_brake_light)(void),
    void (*turn_off_brake_light)(void));

void App_BrakeLight_Tick(struct BrakeLight* brake_light);

enum BrakeLight_Status App_BrakeLight_GetStatus(struct BrakeLight* brake_light);

void App_BrakeLight_Destroy(struct BrakeLight* brake_light);


