#pragma once
#include "app_vehicleDynamics.h"

TorqueAllocationOutputs app_vanillaDrive_run(float apps_pedal_percentage);

TorqueAllocationOutputs app_driveMode_run(float apps_pedal_percentage);