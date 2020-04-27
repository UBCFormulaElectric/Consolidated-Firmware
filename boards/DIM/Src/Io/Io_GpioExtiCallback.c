#include "Io_DriveMode.h"

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    Io_RotarySwitch_ExtiCallback(GPIO_Pin);
}
