#include "main.h"

#include "hw/gpios.h"
#include "hw/gpio.h"

const Gpio dout_1 = { .port = DOUT1_GPIO_Port, .pin = DOUT1_Pin };
const Gpio dout_2 = { .port = DOUT2_GPIO_Port, .pin = DOUT2_Pin };
const Gpio dout_3 = { .port = DOUT3_GPIO_Port, .pin = DOUT3_Pin };
const Gpio dout_4 = { .port = DOUT4_GPIO_Port, .pin = DOUT4_Pin };

const Gpio debug_led = { .port = Debug_LED_GPIO_Port, .pin = Debug_LED_Pin };
const Gpio boot_led  = { .port = Boot_LED_GPIO_Port, .pin = Boot_LED_Pin };

const Gpio indicator_1 = { .port = INDICATOR1_GPIO_Port, .pin = INDICATOR1_Pin };
const Gpio indicator_2 = { .port = INDICATOR2_GPIO_Port, .pin = INDICATOR2_Pin };
const Gpio indicator_3 = { .port = INDICATOR3_GPIO_Port, .pin = INDICATOR3_Pin };

const Gpio isospi_chipSelect_ls = { .port = CS_LS_GPIO_Port, .pin = CS_LS_Pin };
const Gpio isospi_chipSelect_hs = { .port = CS_HS_GPIO_Port, .pin = CS_HS_Pin };

const Gpio interlock_1 = { .port = INT1_GPIO_Port, .pin = INT1_Pin };
const Gpio interlock_2 = { .port = INT2_GPIO_Port, .pin = INT2_Pin };
const Gpio interlock_3 = { .port = INT3_GPIO_Port, .pin = INT3_Pin };
const Gpio interlock_4 = { .port = INT4_GPIO_Port, .pin = INT4_Pin };

const Gpio dac_n_clear    = { .port = nCLR_GPIO_Port, .pin = nCLR_Pin };
const Gpio dac_chipSelect = { .port = CS_LD_GPIO_Port, .pin = CS_LD_Pin };
