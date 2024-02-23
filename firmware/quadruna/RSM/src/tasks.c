#include "tasks.h"
#include "main.h"
#include "cmsis_os.h"

#include "io_log.h"

#include "hw_gpio.h"
#include "hw_adc.h"

extern ADC_HandleTypeDef hadc1;
extern TIM_HandleTypeDef htim3;

static const Gpio n_chimera_pin      = { .port = nCHIMERA_GPIO_Port, .pin = nCHIMERA_Pin };
static const Gpio led_pin            = { .port = LED_GPIO_Port, .pin = LED_Pin };
static const Gpio rad_fan_en_pin     = { .port = RAD_FAN_EN_GPIO_Port, .pin = RAD_FAN_EN_Pin };
static const Gpio fr_stby_pin        = { .port = FR_STBY_GPIO_Port, .pin = FR_STBY_Pin };
static const Gpio brake_light_en_pin = { .port = BRAKE_LIGHT_EN_3V3_GPIO_Port, .pin = BRAKE_LIGHT_EN_3V3_Pin };
static const Gpio acc_fan_en_pin     = { .port = ACC_FAN_EN_GPIO_Port, .pin = ACC_FAN_EN_Pin };
static const Gpio n_program_pin      = { .port = nProgram_3V3_GPIO_Port, .pin = nProgram_3V3_Pin };

void tasks_preInit(void)
{
    // TODO: Setup bootloader.
}

void tasks_init(void)
{
    // Start DMA/TIM3 for the ADC.
    HAL_ADC_Start_DMA(&hadc1, (uint32_t *)hw_adc_getRawValuesBuffer(), hadc1.Init.NbrOfConversion);
    HAL_TIM_Base_Start(&htim3);

    // TODO: Re-enable watchdog.
}

void tasks_run100Hz(void)
{
    // TODO: Setup tasks.
    osDelay(osWaitForever);
}

void tasks_runCanTx(void)
{
    // TODO: Setup tasks.
    osDelay(osWaitForever);
}

void tasks_runCanRx(void)
{
    // TODO: Setup tasks.
    osDelay(osWaitForever);
}

void tasks_run1kHz(void)
{
    // TODO: Setup tasks.
    osDelay(osWaitForever);
}

void tasks_run1Hz(void)
{
    // TODO: Setup tasks.
    for (;;)
    {
        hw_gpio_writePin(&led_pin, true);
        osDelay(1000);
        hw_gpio_writePin(&led_pin, false);
        osDelay(1000);
    }
}
