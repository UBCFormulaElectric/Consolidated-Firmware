#include <cmsis_os.h>
#include "Io_HeartbeatMonitor.h"
#include "states/App_FaultState.h"

uint32_t Io_HeartbeatMonitor_GetCurrentMs(void)
{
    return osKernelSysTick() * portTICK_RATE_MS;
}

// void Io_HeartbeatMonitor_TimeoutCallback(
//    enum HeartbeatOneHot heartbeats_to_check,
//    enum HeartbeatOneHot heartbeats_checked_in)
//{
//    App_SharedStateMachine_SetNextState(
//}
