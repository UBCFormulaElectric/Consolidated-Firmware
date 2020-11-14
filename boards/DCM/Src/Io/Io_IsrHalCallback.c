#include <stm32f3xx_hal.h>
#include "main.h"

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    switch (GPIO_Pin)
    {
        case IMU_PIN_2_Pin:
            Io_LSM6DS33_ExtiCallback();
            break;
        default:
            break;
    }

}
