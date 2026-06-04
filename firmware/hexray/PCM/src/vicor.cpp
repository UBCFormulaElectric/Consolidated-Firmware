#include "vicor.hpp"

#include "main.h"
#include "hw_i2c.hpp"
#include <cassert>

enum class VicorPage
{
    CONFIG  = 0X00,
    TELEM   = 0X01,
    UNKNOWN = 0X02,
};

static constexpr uint32_t I2C_ADDR       = 0x51;
static constexpr uint32_t I2C_TIMEOUT_MS = 1000;

static constexpr uint8_t CMD_PAGE                = 0x0;
static constexpr uint8_t CMD_OPERATION           = 0x1;
static constexpr uint8_t CMD_CLEAR_FAULTS        = 0x03;
static constexpr uint8_t CMD_STATUS_WORD         = 0x79;
static constexpr uint8_t CMD_STATUS_IOUT         = 0x7B;
static constexpr uint8_t CMD_STATUS_INPUT        = 0x7C;
static constexpr uint8_t CMD_STATUS_TEMP         = 0x7D;
static constexpr uint8_t CMD_STATUS_CML          = 0x7E;
static constexpr uint8_t CMD_STATUS_MFR_SPECIFIC = 0x80;
static constexpr uint8_t CMD_READ_VIN            = 0x88;
static constexpr uint8_t CMD_READ_IIN            = 0x89;
static constexpr uint8_t CMD_READ_VOUT           = 0x8B;
static constexpr uint8_t CMD_READ_IOUT           = 0x8C;
static constexpr uint8_t CMD_READ_TEMP           = 0x8D;
static constexpr uint8_t CMD_READ_POUT           = 0x96;

#define DECODE(m, Y, R, b) ((1.0f / m) * (Y * R - b))
#define DECODE_V(Y) (DECODE(1.0f, Y, 1e-1f, 0))
#define DECODE_IIN(Y) (DECODE(1.0f, Y, 1e-3f, 0))
#define DECODE_IOUT(Y) (DECODE(1.0f, Y, 1e-2f, 0))
#define DECODE_POUT(Y) (DECODE(1.0f, Y, 1e-0f, 0))
#define DECODE_TEMP(Y) (DECODE(1.0f, Y, 1e-0f, 0))

// static I2cBus    vicor_bus    = { .handle = &hi2c1 };
static hw::i2c::bus    vicor_bus(hi2c1);
static hw::i2c::device vicor_i2c{ vicor_bus, I2C_ADDR, I2C_TIMEOUT_MS };
static auto            current_page = VicorPage::UNKNOWN;

[[nodiscard]] const hw::i2c::bus &hw::i2c::getBusFromHandle(const I2C_HandleTypeDef *handle)
{
    assert(handle == &hi2c1);
    return vicor_bus;
}

static result<void> writeByte(const uint8_t cmd, const uint8_t data)
{
    std::array<uint8_t, 2> payload = { { cmd, data } };
    return vicor_i2c.transmit(payload);
}

// I think these are right but we don't need them for any of the commands below, so commenting them out.
// static result<void> writeWord(uint8_t cmd, uint16_t data)
// {
//     uint8_t payload[2];
//     payload[0] = cmd;
//     memcpy(&payload[1], &data, sizeof(data));
//     return hw_i2c_receive(&vicor_i2c, payload, sizeof(payload));
// }

static result<void> readByte(const uint8_t cmd, uint8_t &data)
{
    return vicor_i2c.memoryRead(cmd, std::span(&data, sizeof(uint8_t)));
}

static result<void> readWord(const uint8_t cmd, uint16_t &data)
{
    return vicor_i2c.memoryRead(cmd, std::span(reinterpret_cast<uint8_t *>(&data), sizeof(uint16_t)));
}

static result<void> enforcePage(const VicorPage page)
{
    assert(page == VicorPage::CONFIG || page == VicorPage::TELEM);

    if (page != current_page)
    {
        RETURN_IF_ERR(writeByte(CMD_PAGE, static_cast<uint8_t>(page)));
        current_page = page;
    }

    return {};
}

result<void> vicor_operation(const bool enable)
{
    RETURN_IF_ERR(enforcePage(VicorPage::CONFIG));

    return writeByte(CMD_OPERATION, static_cast<uint8_t>(enable << 7));
}

result<void> vicor_clearFaults()
{
    RETURN_IF_ERR(enforcePage(VicorPage::CONFIG));

    constexpr uint8_t cmd = CMD_CLEAR_FAULTS;
    return vicor_i2c.transmit(std::span(&cmd, 1));
}

result<float> vicor_readVin()
{
    RETURN_IF_ERR(enforcePage(VicorPage::TELEM));

    uint16_t word = 0;
    RETURN_IF_ERR(readWord(CMD_READ_VIN, word));
    return DECODE_V(word);
}

result<float> vicor_readIin()
{
    RETURN_IF_ERR(enforcePage(VicorPage::TELEM));

    uint16_t word = 0;
    RETURN_IF_ERR(readWord(CMD_READ_IIN, word));
    return DECODE_IIN(word);
}

result<float> vicor_readVout()
{
    RETURN_IF_ERR(enforcePage(VicorPage::TELEM));

    uint16_t word = 0;
    RETURN_IF_ERR(readWord(CMD_READ_VOUT, word));
    return DECODE_V(word);
}

result<float> vicor_readIout()
{
    RETURN_IF_ERR(enforcePage(VicorPage::TELEM));

    uint16_t word = 0;
    RETURN_IF_ERR(readWord(CMD_READ_IOUT, word));
    return DECODE_IOUT(word);
}

result<float> vicor_readTemp()
{
    RETURN_IF_ERR(enforcePage(VicorPage::TELEM));

    uint16_t word = 0;
    RETURN_IF_ERR(readWord(CMD_READ_TEMP, word));
    return DECODE_TEMP(word);
}

result<float> vicor_readPout()
{
    RETURN_IF_ERR(enforcePage(VicorPage::TELEM));

    uint16_t word = 0;
    RETURN_IF_ERR(readWord(CMD_READ_POUT, word));
    return DECODE_POUT(word);
}

result<void> vicor_readSerial()
{
    RETURN_IF_ERR(enforcePage(VicorPage::CONFIG));

    uint16_t word;
    RETURN_IF_ERR(readWord(0x9D, word));
    LOG_INFO("%d", word);
    uint8_t serial[16];
    LOG_IF_ERR(vicor_i2c.memoryRead(0x9E, std::span(serial, sizeof(serial))));
    LOG_INFO(
        "Serial: %X%X%X%X%X%X%X%X%X%X%X%X%X%X%X", serial[0], serial[1], serial[2], serial[3], serial[4], serial[5],
        serial[6], serial[7], serial[8], serial[9], serial[10], serial[11], serial[12], serial[13], serial[14],
        serial[15]);

    // model
    char model[18];
    LOG_IF_ERR(vicor_i2c.memoryRead(0x9A, std::span(reinterpret_cast<uint8_t *>(model), sizeof(model))));
    LOG_INFO("model: %s", model);
    return {};
}

result<uint16_t> vicor_statusWord()
{
    RETURN_IF_ERR(enforcePage(VicorPage::CONFIG));

    uint16_t status;
    RETURN_IF_ERR(readWord(CMD_STATUS_WORD, status));
    return status;
}

result<uint8_t> vicor_statusIout()
{
    RETURN_IF_ERR(enforcePage(VicorPage::CONFIG));

    uint8_t status;
    RETURN_IF_ERR(readByte(CMD_STATUS_IOUT, status));
    return status;
}

result<uint8_t> vicor_statusInput()
{
    RETURN_IF_ERR(enforcePage(VicorPage::CONFIG));

    uint8_t status;
    RETURN_IF_ERR(readByte(CMD_STATUS_INPUT, status));
    return status;
}

result<uint8_t> vicor_statusTemp()
{
    RETURN_IF_ERR(enforcePage(VicorPage::CONFIG));

    uint8_t status;
    RETURN_IF_ERR(readByte(CMD_STATUS_TEMP, status));
    return status;
}

result<uint8_t> vicor_statusComm()
{
    RETURN_IF_ERR(enforcePage(VicorPage::CONFIG));

    uint8_t status;
    RETURN_IF_ERR(readByte(CMD_STATUS_CML, status));
    return status;
}

result<uint8_t> vicor_statusMfrSpecific()
{
    RETURN_IF_ERR(enforcePage(VicorPage::CONFIG));

    uint8_t status;
    RETURN_IF_ERR(readByte(CMD_STATUS_MFR_SPECIFIC, status));
    return status;
}