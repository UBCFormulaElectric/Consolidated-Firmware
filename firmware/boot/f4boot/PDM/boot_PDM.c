#include "bootloader.h"
#include "hw_gpio.h"

#define EN_LVPWR_GPIO_PORT GPIOB
#define EN_LVPWR_PIN GPIO_PIN_14

Gpio lvpwr_enable_pin = {
    .port = EN_LVPWR_GPIO_PORT,
    .pin  = EN_LVPWR_PIN,
};

void bootloader_boardSpecific_init()
{
    GPIO_InitTypeDef lvpwr_enable_gpio_init = {
        .Pin  = EN_LVPWR_PIN,
        .Mode = GPIO_MODE_OUTPUT_PP,
        .Pull = GPIO_NOPULL,
    };
    HAL_GPIO_Init(EN_LVPWR_GPIO_PORT, &lvpwr_enable_gpio_init);

    // Must enable the LVPWR load switch so the PDM will power the rest of the car, even while in the bootloader.
    hw_gpio_writePin(&lvpwr_enable_pin, true);
}

void bootloader_boardSpecific_deinit()
{
    HAL_GPIO_DeInit(EN_LVPWR_GPIO_PORT, EN_LVPWR_PIN);
}
