#pragma once

#include "App_FsmWorld.h"

struct Steering;

struct Steering *App_Steering_Create(float (*get_steering_angle)(void));
void App_Steering_Destroy(struct Steering *steering);

struct InRangeCheck *App_Steering_GetInRange(const struct Steering *steering);

void App_Steering_Broadcast(const struct FsmWorld * world);
