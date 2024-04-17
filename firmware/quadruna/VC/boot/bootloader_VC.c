#include "bootloader.h"
#include "hw_gpio.h"
#include "main.h"

Gpio lvpwr_enable_pin = {
    .port = LVPWR_EN_GPIO_Port,
    .pin  = LVPWR_EN_Pin,
};

void bootloader_boardSpecific_init()
{
    // Must enable the LVPWR load switch so the VC will power the rest of the car, even while in the bootloader.
    hw_gpio_writePin(&lvpwr_enable_pin, true);
}