#pragma once

#include "Test_Pdm.h"

extern "C"
{
#include "App_SharedStateMachine.h"
#include "states/App_InitState.h"
#include "states/App_AirOpenState.h"
#include "states/App_AirClosedState.h"
}

DECLARE_FAKE_VOID_FUNC(
    send_non_periodic_msg_PDM_STARTUP,
    struct CanMsgs_pdm_startup_t *);
DECLARE_FAKE_VOID_FUNC(
    send_non_periodic_msg_PDM_AIR_SHUTDOWN,
    struct CanMsgs_pdm_air_shutdown_t *);
DECLARE_FAKE_VOID_FUNC(
    send_non_periodic_msg_PDM_MOTOR_SHUTDOWN,
    struct CanMsgs_pdm_motor_shutdown_t *);
DECLARE_FAKE_VOID_FUNC(
    send_non_periodic_msg_PDM_WATCHDOG_TIMEOUT,
    struct CanMsgs_pdm_watchdog_timeout_t *);
