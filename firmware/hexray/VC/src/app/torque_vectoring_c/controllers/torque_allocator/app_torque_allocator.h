#pragma once 

#include "app_canTx.h"
#include "app_canRx.h"
#include "app_canAlerts.h"
#include "app_regen.h"
#include "app_activeDifferential.h"
#include "app_vehicleDynamicsConstants.h"
#include "app_powerLimiting.h"
#include "app_torqueVectoring.h"
#include "app_sbgEllipse.h"
#include "io_log.h"

typedef struct TorqueAllocationOutputs
{
    double torque_fl;
    double torque_fr;
    double torque_rl;
    double torque_rr;
} TorqueAllocationOutputs_t;
