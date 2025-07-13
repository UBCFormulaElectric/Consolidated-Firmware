#pragma once
#include "app_canTx.h"
#include "app_canRx.h"
#include "app_units.h"
#include "app_utils.h"
#include "app_signal.h"
#include "app_vehicleDynamicsConstants.h"
#include "app_torqueVectoring.h"
#include "app_regen.h"
#include "app_powerManager.h"
#include "app_warningHandling.h"
#include "app_canAlerts.h"
#include "app_vehicleDynamics.h"
#include "app_powerLimiting.h"
#include "app_torqueDistribution.h"

void app_vanillaDrive_run(const float apps_pedal_percentage, TorqueAllocationOutputs *torqueOutputToMotors);

void app_driveMode_run(const float apps_pedal_percentage, TorqueAllocationOutputs *torqueOutputToMotors);
