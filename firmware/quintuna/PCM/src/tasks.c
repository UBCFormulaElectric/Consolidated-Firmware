#include "tasks.h"
#include "main.h"

#include <stdbool.h>

#include "io_log.h"
#include "hw_hardFaultHandler.h"

static bool pcm_en = false;

void tasks_preInit()
{
    hw_hardFaultHandler_init();
}

void tasks_init()
{
    SEGGER_RTT_Init();
    LOG_INFO("PCM reset!");

    pcm_en = HAL_GPIO_ReadPin(PCM_EN_GPIO_Port, PCM_EN_Pin) == GPIO_PIN_SET;
}

#define VICOR_ADDRESS 0x92
#define OPERATION 0x1

void tasks_tick()
{
    // HAL_SMBUS_IsDeviceReady(&hsmbus1, VICOR_ADDRESS, 5, 10);

    // note that transfer options are
    // SMBUS_FIRST_FRAME,SMBUS_NEXT_FRAME,SMBUS_FIRST_AND_LAST_FRAME_NO_PEC,SMBUS_LAST_FRAME_NO_PEC
    // SMBUS_FIRST_AND_LAST_FRAME_WITH_PEC,SMBUS_LAST_FRAME_WITH_PEC

    uint8_t tx[2] = { OPERATION, pcm_en ? 0x80 : 0x00 };
    // HAL_SMBUS_Master_Transmit_IT(&hsmbus1, VICOR_ADDRESS, tx, 2, SMBUS_FIRST_FRAME);
}

void HAL_GPIO_EXTI_Callback(const uint16_t GPIO_Pin)
{
    assert(GPIO_Pin == PCM_EN_Pin);
}