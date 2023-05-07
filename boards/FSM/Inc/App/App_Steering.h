#pragma once

#include "App_FsmWorld.h"

struct Steering;

struct Steering *App_Steering_Create(float (*get_steering_angle)(void),
                                     bool (*steering_sensor_OCSC)(void));
void App_Steering_Destroy(struct Steering *steering);

void App_Steering_Broadcast(const struct FsmWorld *world);
