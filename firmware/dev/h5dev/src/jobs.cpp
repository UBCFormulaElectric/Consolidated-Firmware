#include "jobs.hpp"
#include "main.h"

void jobs_init()
{
    HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
}
void jobs_run1Hz_tick()
{
    HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
}
void jobs_run100Hz_tick() {}
void jobs_run1kHz_tick() {}
