#pragma once

#include "Test_Dim.h"

extern "C"
{
#include "App_SharedStateMachine.h"
#include "App_SevenSegDisplays.h"
#include "App_SevenSegDisplay.h"
#include "states/App_DriveState.h"
}

DECLARE_FAKE_VOID_FUNC(
    send_non_periodic_msg_DIM_STARTUP,
    struct CanMsgs_dim_startup_t *);
DECLARE_FAKE_VOID_FUNC(
    send_non_periodic_msg_DIM_WATCHDOG_TIMEOUT,
    struct CanMsgs_dim_watchdog_timeout_t *);
