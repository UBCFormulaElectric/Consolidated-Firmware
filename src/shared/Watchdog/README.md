## Overview
The Independent Watchdog (IWDG) is an embedded watchdog peripheral needs to be periodically refreshed using `HAL_IWDG_Refresh(&hiwdg)` or else it will trigger a system reset when the counter reaches a given timeout value. Note that `hiwdg` is the **IWDG** handle. 

It is possible to run `HAL_IWDG_Refresh()` when `hiwdg` is not yet initialized which would result in a Hard Fault. To get around this issue, this library proposes to use `SharedWatchdog_RefreshIwdg()` instead of `HAL_IWDG_Refresh()`, which only calls `HAL_IWDG_Refresh()` only if `hiwdg` has been properly initialized. This requires using `SharedWatchdog_SetIwdgInitialized()` as described below.


In addition, we must use `__HAL_DBGMCU_FREEZE_IWDG()` to stop the **IWDG** clock when the CPU core is halted during **debugging**, or else **IWDG** could trigger an undesired system reset during a debug session.

## STM32CubeMX Setup
Create **User Constants** in STM32CubeMX with the following values:

```
IWDG_WINDOW_DISABLE_VALUE = 4095
LSI_FREQUENCY             = <LSI Clock Frequency in Hertz. For example, it is 40,000Hz for STM32F302R8.>
IWDG_PRESCALER            = 4
IWDG_RESET_FREQUENCY      = 5
```

then set the following parameters for **Watchdog Clocking** under **Parameter Settings** for **IWDG**:
```
IWDG counter clock prescaler   = 4
IWDG window value              = IWDG_WINDOW_DISABLE_VALUE 
IWDG down-counter reload value = LSI_FREQUENCY / IWDG_PRESCALER / IWDG_RESET_FREQUENCY
```
By using User Constants, the STM32CubeMX configurations become much more readable and easier to adjust later on if we want to change the watchdog frequency. In addition, we disable Window Watchdog by setting `IWDG window value = IWDG_WINDOW_DISABLE_VALUE`, as Window Watchdog would add complexity to our project with little to no benefit.

## How to Use This Library
1. Insert `SharedWatchdog_SetIwdgInitialized(&hiwdg);`:
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
    SharedWatchdog_SetIwdgInitialized(&hiwdg);
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
