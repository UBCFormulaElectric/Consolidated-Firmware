#pragma once

#include <math.h>

#include "Test_BaseStateMachineTest.h"
#include "Test_Bms.h"
#include "Test_Imd.h"

extern "C" {
#include "App_SharedMacros.h"
#include "App_SharedStateMachine.h"
#include "configs/App_HeartbeatMonitorConfig.h"
#include "configs/App_ImdConfig.h"
#include "states/App_ChargeState.h"
#include "states/App_DriveState.h"
#include "states/App_FaultState.h"
#include "states/App_InitState.h"
#include "states/App_PreChargeState.h"
}