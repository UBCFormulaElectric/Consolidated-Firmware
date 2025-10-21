#include "vicor.h"
#include "app/utils.h"
#include "hw/i2c.h"
#include <stdint.h>

typedef enum
{
    VICOR_PAGE_CONFIG  = 0X00,
    VICOR_PAGE_TELEM   = 0X01,
    VICOR_PAGE_UNKNOWN = 0X02,
} VicorPage;

#define I2C_ADDR 0x51
#define I2C_TIMEOUT_MS 1000

#define CMD_PAGE 0x0
#define CMD_OPERATION 0x1
#define CMD_CLEAR_FAULTS 0x03
#define CMD_STATUS_WORD 0x79
#define CMD_STATUS_IOUT 0x7B
#define CMD_STATUS_INPUT 0x7C
#define CMD_STATUS_TEMP 0x7D
#define CMD_STATUS_CML 0x7E
#define CMD_STATUS_MFR_SPECIFIC 0x80
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

static I2cBus    vicor_bus    = { .handle = &hi2c1 };
static I2cDevice vicor_i2c    = { .bus = &vicor_bus, .target_address = I2C_ADDR, .timeout_ms = I2C_TIMEOUT_MS };
static bool      current_page = VICOR_PAGE_UNKNOWN;

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

static ExitCode readByte(uint8_t cmd, uint8_t *data)
{
    return hw_i2c_memoryRead(&vicor_i2c, cmd, data, sizeof(uint8_t));
}

static ExitCode readWord(uint8_t cmd, uint16_t *data)
{
    return hw_i2c_memoryRead(&vicor_i2c, cmd, (uint8_t *)data, sizeof(uint16_t));
}

static ExitCode enforcePage(VicorPage page)
{
    assert(page == VICOR_PAGE_CONFIG || page == VICOR_PAGE_TELEM);

    if (page != current_page)
    {
        RETURN_IF_ERR(writeByte(CMD_PAGE, page));
        current_page = page;
    }

    return EXIT_CODE_OK;
}

ExitCode vicor_operation(bool enable)
{
    RETURN_IF_ERR(enforcePage(VICOR_PAGE_CONFIG));

    return writeByte(CMD_OPERATION, (uint8_t)(enable << 7));
}

ExitCode vicor_clearFaults(void)
{
    RETURN_IF_ERR(enforcePage(VICOR_PAGE_CONFIG));

    const uint8_t cmd = CMD_CLEAR_FAULTS;
    return hw_i2c_transmit(&vicor_i2c, &cmd, sizeof(cmd));
}

ExitCode vicor_readVin(float *val)
{
    RETURN_IF_ERR(enforcePage(VICOR_PAGE_TELEM));

    uint16_t word = 0;
    RETURN_IF_ERR(readWord(CMD_READ_VIN, &word));
    *val = DECODE_V(word);
    return EXIT_CODE_OK;
}

ExitCode vicor_readIin(float *val)
{
    RETURN_IF_ERR(enforcePage(VICOR_PAGE_TELEM));

    uint16_t word = 0;
    RETURN_IF_ERR(readWord(CMD_READ_IIN, &word));
    *val = DECODE_IIN(word);
    return EXIT_CODE_OK;
}

ExitCode vicor_readVout(float *val)
{
    RETURN_IF_ERR(enforcePage(VICOR_PAGE_TELEM));

    uint16_t word = 0;
    RETURN_IF_ERR(readWord(CMD_READ_VOUT, &word));
    *val = DECODE_V(word);
    return EXIT_CODE_OK;
}

ExitCode vicor_readIout(float *val)
{
    RETURN_IF_ERR(enforcePage(VICOR_PAGE_TELEM));

    uint16_t word = 0;
    RETURN_IF_ERR(readWord(CMD_READ_IOUT, &word));
    *val = DECODE_IOUT(word);
    return EXIT_CODE_OK;
}

ExitCode vicor_readTemp(float *val)
{
    RETURN_IF_ERR(enforcePage(VICOR_PAGE_TELEM));

    uint16_t word = 0;
    RETURN_IF_ERR(readWord(CMD_READ_TEMP, &word));
    *val = DECODE_TEMP(word);
    return EXIT_CODE_OK;
}

ExitCode vicor_readPout(float *val)
{
    RETURN_IF_ERR(enforcePage(VICOR_PAGE_TELEM));

    uint16_t word = 0;
    RETURN_IF_ERR(readWord(CMD_READ_POUT, &word));
    *val = DECODE_POUT(word);
    return EXIT_CODE_OK;
}

ExitCode vicor_readSerial()
{
    RETURN_IF_ERR(enforcePage(VICOR_PAGE_CONFIG));

    uint16_t word;
    RETURN_IF_ERR(readWord(0x9D, &word));
    LOG_INFO("%d", word);
    uint8_t serial[16];
    hw_i2c_memoryRead(&vicor_i2c, 0x9E, serial, sizeof(serial));
    LOG_INFO(
        "Serial: %X%X%X%X%X%X%X%X%X%X%X%X%X%X%X", serial[0], serial[1], serial[2], serial[3], serial[4], serial[5],
        serial[6], serial[7], serial[8], serial[9], serial[10], serial[11], serial[12], serial[13], serial[14],
        serial[15]);

    // model
    char model[18];
    hw_i2c_memoryRead(&vicor_i2c, 0x9A, (uint8_t *)model, sizeof(model));
    LOG_INFO("model: %s", model);
    return EXIT_CODE_OK;
}

ExitCode vicor_statusWord(uint16_t *status)
{
    RETURN_IF_ERR(enforcePage(VICOR_PAGE_CONFIG));

    return readWord(CMD_STATUS_WORD, status);
}

ExitCode vicor_statusIout(uint8_t *status)
{
    RETURN_IF_ERR(enforcePage(VICOR_PAGE_CONFIG));

    return readByte(CMD_STATUS_IOUT, status);
}

ExitCode vicor_statusInput(uint8_t *status)
{
    RETURN_IF_ERR(enforcePage(VICOR_PAGE_CONFIG));

    return readByte(CMD_STATUS_INPUT, status);
}

ExitCode vicor_statusTemp(uint8_t *status)
{
    RETURN_IF_ERR(enforcePage(VICOR_PAGE_CONFIG));

    return readByte(CMD_STATUS_TEMP, status);
}

ExitCode vicor_statusComm(uint8_t *status)
{
    RETURN_IF_ERR(enforcePage(VICOR_PAGE_CONFIG));

    return readByte(CMD_STATUS_CML, status);
}

ExitCode vicor_statusMfrSpecific(uint8_t *status)
{
    RETURN_IF_ERR(enforcePage(VICOR_PAGE_CONFIG));

    return readByte(CMD_STATUS_MFR_SPECIFIC, status);
}