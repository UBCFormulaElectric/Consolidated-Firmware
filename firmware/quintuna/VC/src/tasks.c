#include "tasks.h"
#include "/Users/jackpolloway/Consolidated-Firmware/firmware/quintuna/VC/src/cubemx/Inc/main.h"
#include "io_lowVoltageBattery.h"

void tasks_init(void)
{
    io_lowVoltageBattery_init();
}

void tasks_bat_mtr(void)
{
    io_lowVoltageBattery_get_SOC();
    io_lowVoltageBattery_get_voltage();
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_pin)
{
    io_lowVoltageBattery_completeAlert(GPIO_pin);
}
