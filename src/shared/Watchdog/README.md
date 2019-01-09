## Overview
The Independent Watchdog (IWDG) is an embedded watchdog peripheral needs to be periodically refreshed using `HAL_IWDG_Refresh(&hiwdg)` or else it will trigger a system reset when the counter reaches a given timeout value. Note that `hiwdg` is the **IWDG** handle. 

It is possible to run `HAL_IWDG_Refresh(&hiwdg)` when `hiwdg` is not yet initialized which would result in a Hard Fault. To get around this issue, this library proposes to use `SharedWatchdog_RefreshIwdg()` instead of `HAL_IWDG_Refresh(&hiwdg)`, which only calls `HAL_IWDG_Refresh(&hiwdg)` if `hiwdg` has been properly initialized. This requires using `SharedWatchdog_SetIwdgInitialized()` as described below.


In addition, we must use `__HAL_DBGMCU_FREEZE_IWDG()` to stop the **IWDG** clock when the CPU core is halted during **debugging**, or else **IWDG** could trigger an undesired system reset during a debug session.

## How to Use This Library
1. Insert `SharedWatchdog_SetIwdgInitialized();`:
```
// main.c

static void MX_IWDG_Init(void)
{
    /* Omitted some code */
    if (HAL_IWDG_Init(&hiwdg) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN IWDG_Init 2 */
    SharedWatchdog_SetIwdgInitialized();
    /* USER CODE END IWDG_Init 2 */
}
```
1. Insert `__HAL_DBGMCU_FREEZE_IWDG();`:
```
// main.c

{
    /* USER CODE BEGIN 1 */
    __HAL_DBGMCU_FREEZE_IWDG();
    /* USER CODE END 1 */
    
    /* Omitted some code */
}
```

1. Insert `SharedWatchdog_RefreshIwdg()` where needed (e.g. In a 100Hz Control Loop):

```
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == ControlLoopTimer)
    {
        SharedWatchdog_RefreshIwdg();
    }
}
```
