#include "jobs.hpp"
#include "main.h"
#include "usbd_cdc_if.h"

void jobs_init() {}
void jobs_run1Hz_tick() {}
void jobs_run100Hz_tick()
{
    static bool last_pressed = false;

    bool pressed = (HAL_GPIO_ReadPin(LED_GPIO_Port, LED_Pin) == GPIO_PIN_SET);

    if (pressed && !last_pressed)
    {
        const uint8_t msg[] = "Button pressed\r\n";
        CDC_Transmit_FS((uint8_t *)msg, sizeof(msg) - 1);
    }

    last_pressed = pressed;
}
void jobs_run1kHz_tick() {}
