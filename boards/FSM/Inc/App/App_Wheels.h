#pragma once

#include "App_FsmWorld.h"

struct Wheels;

struct Wheels *App_Wheels_Create(float (*get_left_wheel_speed)(void),
                                 float (*get_right_wheel_speed)(void));
void App_Wheels_Destroy(struct Wheels *wheels);

struct InRangeCheck *App_Wheels_GetLeftWheelSpeedInRange(
    const struct Wheels *wheels);
struct InRangeCheck *App_Wheels_GetRightWheelSpeedInRange(
    const struct Wheels *wheels);

void App_Wheels_Broadcast(const struct FsmWorld *world);
