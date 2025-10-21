#pragma once
#include "app_canTx.h"
#include "app_canRx.h"
#include "app/units.h"
#include "app/utils.h"
#include "app/signal.h"
#include "app/vehicle_dynamics/vehicleDynamicsConstants.h"
#include "app/vehicle_dynamics/torqueVectoring.h"
#include "app/vehicle_dynamics/regen.h"
#include "app/powerManager.h"
#include "app/warningHandling.h"
#include "app_canAlerts.h"
#include "app/vehicle_dynamics/vehicleDynamics.h"
#include "app/vehicle_dynamics/powerLimiting.h"
#include "app/vehicle_dynamics/torqueDistribution.h"

void app_vanillaDrive_run(const float apps_pedal_percentage, TorqueAllocationOutputs *torqueOutputToMotors);

void app_driveMode_run(const float apps_pedal_percentage, TorqueAllocationOutputs *torqueOutputToMotors);
