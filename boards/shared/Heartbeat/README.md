## Overview
This repository contains our heartbeat library. The goal is for a board to quickly handle losing CAN communication with any other boards that it's listening to. Each board sends out a CAN heartbeat message every 100ms and checks for timed-out heartbeats from other boards every 320ms, allowing some room for error while ensuring that we react quickly. The missed heartbeat handler is board-specific and must be implemented by the developer.

## How to Use This Library
1. Call the heartbeat broadcast/timeout check functions every 1ms through the `Task1kHz()`:

```c
#include "Io_HeartbeatDriver.h"

void Task1Khz(void const *argument)
{
    uint32_t PreviousWakeTime = osKernelSysTick();

    for (;;)
    {
        Io_HeartbeatDriver_BroadcastHeartbeat(CANMSGS_dcm_heartbeat_FRAME_ID, CANMSGS_DCM_HEARTBEAT_LENGTH);
        Io_HeartbeatDriver_CheckHeartbeatTimeout(BMS_HEARTBEAT_ENCODING | FSM_HEARTBEAT_ENCODING);
        (void)Io_CmsisOsWrapper_osDelayUntilMs(&PreviousWakeTime, 1U)
    }
}
```
The periodicity of broadcasting and timeout checks are handled within their respective functions.

2. Implement `Heartbeat_HandleHeartbeatReception()` in `Heartbeat.c` and call the function in the `Can_RxCommonCallback()`:

```c
// Heartbeat.c example for BMS, which listens to FSM, DCM and PDM's heartbeats
#include "Io_HeartbeatDriver.h"

void Heartbeat_HandleHeartbeatReception(uint32_t std_id)
{
    switch (std_id)
    {
        case FSM_HEARTBEAT_STDID:
            Io_HeartbeatDriver_ReceiveHeartbeat(FSM_HEARTBEAT_ENCODING);
            break;
        case DCM_HEARTBEAT_STDID:
            Io_HeartbeatDriver_ReceiveHeartbeat(DCM_HEARTBEAT_ENCODING);
            break;
        case PDM_HEARTBEAT_STDID:
            Io_HeartbeatDriver_ReceiveHeartbeat(PDM_HEARTBEAT_ENCODING);
            break;
        default:
            break;
    }
}
```

3. Implement the missed heartbeats handler, checking which board(s) timed out using the `HEARTBEAT_TIMEOUT()` macro:

```c
// Heartbeat.c example for BMS, which listens to FSM, DCM and PDM's heartbeats
#include "Io_HeartbeatDriver.h"

void Heartbeat_HandleHeartbeatTimeout(uint8_t heartbeats_received)
{
    if (HEARTBEAT_TIMEOUT(heartbeats_received, FSM_HEARTBEAT_ENCODING))
    {
        // Handle FSM heartbeat timeout
    }
    if (HEARTBEAT_TIMEOUT(heartbeats_received, DCM_HEARTBEAT_ENCODING))
    {
        // Handle DCM heartbeat timeout
    }
    if (HEARTBEAT_TIMEOUT(heartbeats_received, PDM_HEARTBEAT_ENCODING))
    {
        // Handle PDM heartbeat timeout
    }
}
```
