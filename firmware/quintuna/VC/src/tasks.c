#include "tasks.h"
#include "/Users/jackpolloway/Consolidated-Firmware/firmware/quintuna/VC/src/cubemx/Inc/main.h"
#include "cmsis_os2.h"
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

HAL_GPIO_EXTI_Callback(uint16_t GPIO_pin)
{
    GPIO_pin = BAT_MTR_ALERT_Pin;
    osSemaphoreRelease(bat_mtr_sem);
}
