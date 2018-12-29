## Overview
The ADC can be activated in a variety of modes: polling, interrupt driven, or DMA. Using DMA is the cleanest and most efficient way to transfer ADC readings into memory. For our purpose, there should be no reason why we will ever need to use polling mode or interrupt driven mode for ADC. This shared library attemps to abstract away the details for starting the ADC module in DMA mode.

## How to Use This Library
1. Add `SharedAdc_StartAdcInDmaMode(&hadc1, &adc_readings[0], NUM_ADC_CHANNELS)` to `MX_ADC_INIT()` 
```
// main.c
static void MX_ADC_INIT(void)
{
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
      Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */
  SharedAdc_StartAdcInDmaMode(&hadc1, &adc_readings[0], NUM_ADC_CHANNELS);
  /* USER CODE END ADC1_Init 2 */

}
```
3. Include `SharedAdc.h` in main.c`
```
// main.c
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "SharedAdc.h"
/* USER CODE END Includes */

```

4. Add `shared\ADC\Inc` to Include Paths inside Keil (Note: Your relative path may look different than mine)
5. When DMA transfer is complete, the `HAL_ADC_ConvCpltCallback()` will be called and `adc_reading[NUM_ADC_CHANNELS]`will contain the most recent ADC readings. Implement the board-specific `HAL_ADC_ConvCpltCallback()` in `Adc.c`
