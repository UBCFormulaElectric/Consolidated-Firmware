#include "vicor.h"
#include "app_utils.h"
#include "hw_i2c.h"
#include <stdint.h>

#define I2C_ADDR 0x51
#define I2C_TIMEOUT_MS 1000

#define CMD_OPERATION 0x1
#define CMD_CLEAR_FAULTS 0x03
#define CMD_READ_VIN 0x88
#define CMD_READ_IIN 0x89
#define CMD_READ_VOUT 0x8B
#define CMD_READ_IOUT 0x8C
#define CMD_READ_TEMP 0x8D
#define CMD_READ_POUT 0x96

#define DECODE(m, Y, R, b) ((1.0f / m) * (Y * R - b))
#define DECODE_V(Y) (DECODE(1.0f, Y, 1e-1f, 0))
#define DECODE_IIN(Y) (DECODE(1.0f, Y, 1e-3f, 0))
#define DECODE_IOUT(Y) (DECODE(1.0f, Y, 1e-2f, 0))
#define DECODE_POUT(Y) (DECODE(1.0f, Y, 1e-0f, 0))
#define DECODE_TEMP(Y) (DECODE(1.0f, Y, 1e-0f, 0))

static I2cBus    vicor_bus = { .handle = &hi2c1 };
static I2cDevice vicor_i2c = { .bus = &vicor_bus, .target_address = I2C_ADDR, .timeout_ms = I2C_TIMEOUT_MS };

I2cBus *hw_i2c_getBusFromHandle(const I2C_HandleTypeDef *handle)
{
    assert(handle == &hi2c1);
    return &vicor_bus;
}

static ExitCode writeByte(uint8_t cmd, uint8_t data)
{
    uint8_t payload[2];
    payload[0] = cmd;
    payload[1] = data;
    return hw_i2c_transmit(&vicor_i2c, payload, sizeof(payload));
}

// I think these are right but we don't need them for any of the commands below, so commenting them out.
// static ExitCode writeWord(uint8_t cmd, uint16_t data)
// {
//     uint8_t payload[2];
//     payload[0] = cmd;
//     memcpy(&payload[1], &data, sizeof(data));
//     return hw_i2c_receive(&vicor_i2c, payload, sizeof(payload));
// }
// static ExitCode readByte(uint8_t cmd, uint8_t *data)
// {
//     RETURN_IF_ERR(hw_i2c_transmit(&vicor_i2c, &cmd, sizeof(uint8_t)));
//     return hw_i2c_receive(&vicor_i2c, data, sizeof(uint8_t));
// }

static ExitCode readWord(uint8_t cmd, uint16_t *data)
{
    RETURN_IF_ERR(hw_i2c_transmit(&vicor_i2c, &cmd, sizeof(uint8_t)));
    return hw_i2c_receive(&vicor_i2c, (uint8_t *)data, sizeof(uint16_t));
}

ExitCode vicor_operation(bool enable)
{
    return writeByte(CMD_OPERATION, (uint8_t)(enable << 7));
}

ExitCode vicor_clearFaults(void)
{
    const uint8_t cmd = CMD_CLEAR_FAULTS;
    return hw_i2c_transmit(&vicor_i2c, &cmd, sizeof(cmd));
}

ExitCode vicor_readVin(float *val)
{
    uint16_t word = 0;
    RETURN_IF_ERR(readWord(CMD_READ_VIN, &word));
    *val = DECODE_V(word);
    return EXIT_CODE_OK;
}

ExitCode vicor_readIin(float *val)
{
    uint16_t word = 0;
    RETURN_IF_ERR(readWord(CMD_READ_IIN, &word));
    *val = DECODE_IIN(word);
    return EXIT_CODE_OK;
}

ExitCode vicor_readVout(float *val)
{
    uint16_t word = 0;
    RETURN_IF_ERR(readWord(CMD_READ_VOUT, &word));
    *val = DECODE_V(word);
    return EXIT_CODE_OK;
}

ExitCode vicor_readIout(float *val)
{
    uint16_t word = 0;
    RETURN_IF_ERR(readWord(CMD_READ_IOUT, &word));
    *val = DECODE_IOUT(word);
    return EXIT_CODE_OK;
}

ExitCode vicor_readTemp(float *val)
{
    uint16_t word = 0;
    RETURN_IF_ERR(readWord(CMD_READ_TEMP, &word));
    *val = DECODE_TEMP(word);
    return EXIT_CODE_OK;
}

ExitCode vicor_readPout(float *val)
{
    uint16_t word = 0;
    RETURN_IF_ERR(readWord(CMD_READ_POUT, &word));
    *val = DECODE_POUT(word);
    return EXIT_CODE_OK;
}
