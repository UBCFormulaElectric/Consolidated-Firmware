#include "tasks.h"
#include "app_utils.h"
#include "hw_i2c.h"
#include "main.h"

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "io_log.h"
#include "hw_hardFaultHandler.h"

#define VICOR_ADDRESS 0x51
#define OPERATION 0x1

#define ENCODE(m, Y, R, b) ((1.0f / m) * (Y * R - b))
#define ENCODE_TON_DELAY(Y) (ENCODE(1.0f, Y, 1e-3f, 0))

static I2cBus    vicor_bus = { .handle = &hi2c1 };
static I2cDevice vicor_i2c = { .bus = &vicor_bus, .target_address = VICOR_ADDRESS, .timeout_ms = 1000 };
static bool      pcm_en    = false;

I2cBus *hw_i2c_getBusFromHandle(const I2C_HandleTypeDef *handle)
{
    assert(handle == &hi2c1);
    // const float x = CONVERT(1.0f, 1.0f, 1e-3f, 1.0f);
    return &vicor_bus;
}

static ExitCode vicorWriteByte(uint8_t cmd, uint8_t data)
{
    uint8_t payload[2];
    payload[0] = cmd;
    payload[1] = data;
    return hw_i2c_transmit(&vicor_i2c, payload, sizeof(payload));
}

static ExitCode vicorWriteWord(uint8_t cmd, uint16_t data)
{
    uint8_t payload[2];
    payload[0] = cmd;
    memcpy(&payload[1], &data, sizeof(uint16_t));
    return hw_i2c_receive(&vicor_i2c, payload, sizeof(payload));
}

static ExitCode vicorReadByte(uint8_t cmd, uint8_t *data)
{
    RETURN_IF_ERR(hw_i2c_transmit(&vicor_i2c, &cmd, sizeof(uint8_t)));
    return hw_i2c_receive(&vicor_i2c, data, sizeof(uint8_t));
}

static ExitCode vicorReadWord(uint8_t cmd, uint16_t *data)
{
    RETURN_IF_ERR(hw_i2c_transmit(&vicor_i2c, &cmd, sizeof(uint8_t)));
    return hw_i2c_receive(&vicor_i2c, (uint8_t *)data, sizeof(uint16_t));
}

typedef struct
{
} ReadCommand;

void tasks_preInit(void)
{
    hw_hardFaultHandler_init();
}

void tasks_init(void)
{
    SEGGER_RTT_Init();
    LOG_INFO("PCM reset!");

    pcm_en = HAL_GPIO_ReadPin(PCM_EN_GPIO_Port, PCM_EN_Pin) == GPIO_PIN_SET;
}

void tasks_tick(void)
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