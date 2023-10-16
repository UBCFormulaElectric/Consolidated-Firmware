#pragma once

#include <math.h>
#include "Test_Bms.h"
#include "Test_Imd.h"
#include "Test_BaseStateMachineTest.h"

extern "C"
{
#include "App_SharedStateMachine.h"
#include "App_SharedMacros.h"
#include "states/App_InitState.h"
#include "states/App_PreChargeState.h"
#include "states/App_DriveState.h"
#include "states/App_FaultState.h"
#include "states/App_ChargeState.h"
#include "configs/App_HeartbeatMonitorConfig.h"
#include "configs/App_ImdConfig.h"
#include "App_InverterState.h"
}