#pragma once
#include "App_DcmWorld.h"

struct DcmWorld;

void App_SetPeriodicCanSignals_TorqueRequests(const struct DcmWorld *world);

void App_SetPeriodicCanSignals_Imu(const struct DcmWorld *world);

void App_SetPeriodicCanSignals_InverterSwitches(const struct DcmWorld *world);
