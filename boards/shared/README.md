### ADC
## Overview
The ADC can be activated in a variety of modes: polling, interrupt driven, or DMA. Using DMA is the cleanest and most efficient way to transfer ADC readings into memory. For our purpose, there should be no reason why we will ever need to use polling mode or interrupt driven mode for ADC. This shared library attemps to abstract away the details for starting the ADC module in DMA mode.

In addition, the VDDA power supply voltage applied to the microcontroller may be subject to variation or not precisely known. The embedded internal voltage reference (VREFINT) and its calibration data acquired by the ADC during the manufacturing process at VDDA = 3.3 V can be used to evaluate the actual VDDA voltage level. This library has a helper function `SharedAdc_GetActualVdda()` to calculate the aforementioned actual VDDA voltage level.

## How to Use This Library
The folowing steps assume that there are 2 ADC channels: VREFINT has **Regular Rank 1** and a generic 3.3V analog input has **Regular Rank 2**.

1. Enable desired ADC channels in STM32CubeMX
2. Configure **Regular Rank** for each enabled ADC channel, which decides the order of ADC values inside `adc_values[]` (i.e. The ADC channel with **Regular Rank 1** would be the first element in `adc_values[]`)
3. Add `SharedAdc_StartAdcInDmaMode(&hadc1)` to `MX_ADC_INIT()`
```
// main.c
#define VREFINT_REGULAR_RANK 1

static void MX_ADC_INIT(void)
{
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
      Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */
  SharedAdc_StartAdcInDmaMode(&hadc1, VREFINT_REGULAR_RANK);
  /* USER CODE END ADC1_Init 2 */
}
```
4. Include `SharedAdc.h` in main.c`
```
// main.c
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "SharedAdc.h"
/* USER CODE END Includes */

```
5. Add `shared\ADC\Inc` to Include Paths inside Keil
6. When DMA transfer is complete, `HAL_ADC_ConvCpltCallback()` will be called and you should implement the board-specific `HAL_ADC_ConvCpltCallback()` in `Adc.c`. You can use `SharedAdc_GetAdcVoltage()` to get the voltage value at any ADC channel.
```
// Adc.c
typedef enum
{
    VREFINT,
    ANALOG_INPUT
} AdcValuesIndex_Enum;

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    float32_t analog_input_voltage = SharedAdc_GetAdcVoltage(ANALOG_INPUT);
}
```
7. At anytime, you can use `SharedAdc_GetAdcMaxValue()` to get the maximum ADC value for the configured ADC bit resolution (e.g. 1023 for ADC 10-bit resolution or 4095 for ADC 12-bit resolution). You can also use `SharedAdc_GetAdcValues()` to get the array of most recent raw ADC values (in bits) transferred over DMA.


### Hearbeat
This repository contains our heartbeat library. The goal is for a board to quickly handle losing CAN communication with any other boards that it's listening to. Each board sends out a CAN heartbeat message every 100ms and checks for timed-out heartbeats from other boards every 320ms, allowing some room for error while ensuring that we react quickly. The missed heartbeat handler is board-specific and must be implemented by the developer.

## How to Use This Library
1. Call the heartbeat broadcast/timeout check functions every 1ms through the `Task1kHz()`:

```c
#include "SharedHeartbeat.h"

void Task1Khz(void const *argument)
{
    uint32_t PreviousWakeTime = osKernelSysTick();

    for (;;)
    {
        SharedHeartbeat_BroadcastHeartbeat(CANMSGS_dcm_heartbeat_FRAME_ID, CANMSGS_DCM_HEARTBEAT_LENGTH);
        SharedHeartbeat_CheckHeartbeatTimeout(BMS_HEARTBEAT_ENCODING | FSM_HEARTBEAT_ENCODING);
        (void)SharedCmsisOs_osDelayUntilMs(&PreviousWakeTime, 1U)
    }
}
```
The periodicity of broadcasting and timeout checks are handled within their respective functions.

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

### Watchdog
The Independent Watchdog (IWDG) is an embedded watchdog peripheral needs to be periodically refreshed using `HAL_IWDG_Refresh(&hiwdg)` or else it will trigger a system reset when the counter reaches a given timeout value. Note that `hiwdg` is the **IWDG** handle. 

It is possible to run `HAL_IWDG_Refresh()` when `hiwdg` is not yet initialized which would result in a Hard Fault. To get around this issue, this library proposes to use `SharedWatchdog_RefreshIwdg()` instead of `HAL_IWDG_Refresh()`, which only calls `HAL_IWDG_Refresh()` only if `hiwdg` has been properly initialized. This requires using `SharedWatchdog_SetIwdgInitialized()` as described below.


In addition, we must use `__HAL_DBGMCU_FREEZE_IWDG()` to stop the **IWDG** clock when the CPU core is halted during **debugging**, or else **IWDG** could trigger an undesired system reset during a debug session.

## STM32CubeMX Setup
Create **User Constants** in STM32CubeMX with the following values:

```
IWDG_WINDOW_DISABLE_VALUE = 4095
LSI_FREQUENCY             = <LSI Clock Frequency in Hertz. For example, it is 40000 for STM32F302R8.>
IWDG_PRESCALER            = 4
IWDG_RESET_FREQUENCY      = 5
```

then set the following parameters for **Watchdog Clocking** under **Parameter Settings** for **IWDG**:
```
IWDG counter clock prescaler   = 4
IWDG window value              = IWDG_WINDOW_DISABLE_VALUE 
IWDG down-counter reload value = LSI_FREQUENCY / IWDG_PRESCALER / IWDG_RESET_FREQUENCY
```

In case it was not clear, `IWDG_RESET_FREQUENCY = 5` means that the IWDG has a frequency of 5Hz, or a period of 200ms.

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

### SEGGER Real-Time Transfer (RTT)
To use this library:
1. Download **J-Link Software and Documentation pack** from [here](https://www.segger.com/downloads/jlink/)
1. Configure `JLinkRTTViewer` works and can connect to J-Link debugger
    - Open `JLinkRTTViewer`
    - Select `USB` under `Connection to J-Link`
    - Select the appropriate ST microcontroller (e.g. `STM32F302C8) under `Specify Target Device`
    - Select `SWD` and `4000kHz` under `Target Interface & Speed`
    - Select `Auto Detection` under `RTT Control Block`
1. `#include "SEGGER_RTT.h"` to use `SEGGER_RTT_printf()`:
    ```  
    static uint32_t i = 0;
    for (;;)
    {
        SEGGER_RTT_printf(0, "Test message from SEGGER RTT:%u\r\n", i);
        osDelay(1);
    }
    ```  
1. In `JLinkRTTViewer`, you should see your debug print messages:
![image](https://user-images.githubusercontent.com/16970019/61187626-b6f62c80-a628-11e9-9f7f-6d02f3082de0.png)
1. Happy RTT-ing!
