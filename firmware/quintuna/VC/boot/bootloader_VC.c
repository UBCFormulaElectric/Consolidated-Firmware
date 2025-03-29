#include "bootloader.h"
#include "hw_gpio.h"
#include "main.h"

const Gpio f_inv_en = { .port = F_INV_EN_GPIO_Port, .pin = F_INV_EN_Pin };
const Gpio r_inv_en = { .port = R_INV_EN_GPIO_Port, .pin = R_INV_EN_Pin };
const Gpio dam_en   = { .port = DAM_EN_GPIO_Port, .pin = DAM_EN_Pin };
const Gpio rsm_en   = { .port = RSM_EN_GPIO_Port, .pin = RSM_EN_Pin };
const Gpio front_en = { .port = FRONT_EN_GPIO_Port, .pin = FRONT_EN_Pin };
const Gpio bms_en   = { .port = BMS_EN_GPIO_Port, .pin = BMS_EN_Pin };

void bootloader_boardSpecific_init(void)
{
    // Must enable the all load switches so the VC will power the rest of the car, even while in the bootloader.
    hw_gpio_writePin(&dam_en, true);
    hw_gpio_writePin(&rsm_en, true);
    hw_gpio_writePin(&front_en, true);
    hw_gpio_writePin(&bms_en, true);
}
