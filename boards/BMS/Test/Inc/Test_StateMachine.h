#pragma once

#include "Test_Bms.h"

extern "C"
{
#include "App_SharedStateMachine.h"
#include "App_CanTx.h"
#include "App_CanRx.h"
#include "states/App_InitState.h"
#include "states/App_DriveState.h"
#include "states/App_FaultState.h"
#include "states/App_ChargeState.h"
}

DECLARE_FAKE_VOID_FUNC(
    send_non_periodic_msg_BMS_STARTUP,
    struct CanMsgs_bms_startup_t *);
DECLARE_FAKE_VOID_FUNC(
    send_non_periodic_msg_BMS_WATCHDOG_TIMEOUT,
    struct CanMsgs_bms_watchdog_timeout_t *);
