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

