## Overview
This repository contains our heartbeat library. The goal is for a board to quickly handle losing CAN communication with any other boards that it's listening to. Each board sends out a CAN heartbeat message every 100ms and checks for timed-out heartbeats from other boards every 320ms, allowing some room for error while ensuring that we react quickly. The missed heartbeat handler is board-specific and must be implemented by the developer.

## How to Use This Library
1. Call the heartbeat broadcast/timeout check functions every 1ms through the `SysTick_Handler()`:

```c
// stm32f3xx_it.c
#include "SharedHeartbeat.h"

void SysTick_Handler(void)
{
    /* USER CODE BEGIN SysTick_IRQn 0 */

    /* USER CODE END SysTick_IRQn 0 */
    HAL_IncTick();
    /* USER CODE BEGIN SysTick_IRQn 1 */
    SharedHeartbeat_BroadcastHeartbeat();
    SharedHeartbeat_CheckHeartbeatTimeout();
    /* USER CODE END SysTick_IRQn 1 */
}
```

The periodicity of broadcasting and timeout checks are handled within their respective functions, and don't need to be handled in the `SysTick_Handler()`.

2. Implement `Heartbeat_HandleHeartbeatReception()` in `Heartbeat.c` and call the function in the `Can_RxCommonCallback()`:

```c
// Heartbeat.c example for BMS, which listens to FSM, DCM and PDM's heartbeats
#include "SharedHeartbeat.h"

void Heartbeat_HandleHeartbeatReception(uint32_t std_id)
{
    switch (std_id)
    {
        case FSM_HEARTBEAT_STDID:
            SharedHeartbeat_ReceiveHeartbeat(FSM_HEARTBEAT_ENCODING);
            break;
        case DCM_HEARTBEAT_STDID:
            SharedHeartbeat_ReceiveHeartbeat(DCM_HEARTBEAT_ENCODING);
            break;
        case PDM_HEARTBEAT_STDID:
            SharedHeartbeat_ReceiveHeartbeat(PDM_HEARTBEAT_ENCODING);
            break;
        default:
            break;
    }
}
```

3. Implement the missed heartbeats handler, checking which board(s) timed out using the `HEARTBEAT_TIMEOUT()` macro:

```c
// Heartbeat.c example for BMS, which listens to FSM, DCM and PDM's heartbeats
#include "SharedHeartbeat.h"

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
