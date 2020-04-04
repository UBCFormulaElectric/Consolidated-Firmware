#pragma once

#include <stdbool.h>

enum BrakeLight_Status
{
    BRAKE_LIGHT_ON,
    BRAKE_LIGHT_OFF,
};

// TODO: Fill in javadoc once the review looks good
struct BrakeLight *App_BrakeLight_Create(
    bool (*is_brake_actuated)(void),
    bool (*is_regen_active)(void),
    void (*turn_on_brake_light)(void),
    void (*turn_off_brake_light)(void));

// TODO: Fill in javadoc once the review looks good
void App_BrakeLight_Tick(struct BrakeLight *brake_light);

// TODO: Fill in javadoc once the review looks good
enum BrakeLight_Status App_BrakeLight_GetStatus(struct BrakeLight *brake_light);

// TODO: Fill in javadoc once the review looks good
void App_BrakeLight_Destroy(struct BrakeLight *brake_light);
