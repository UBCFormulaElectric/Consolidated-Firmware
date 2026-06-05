#include "vicor.hpp"

#include "main.h"
#include "hw_i2c.hpp"
#include <cassert>
#include <cstring>

enum class VicorPage
{
    LV_SIDE = 0X00, // aka BCM
    HV_SIDE = 0X01, // aka BCM controller
    UNKNOWN = 0X02,
};

static constexpr uint32_t I2C_ADDR       = 0x51;
static constexpr uint32_t I2C_TIMEOUT_MS = 1000;

static constexpr uint8_t CMD_OPERATION = 0x1;

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
    static constexpr uint8_t CMD_PAGE = 0x0;
    assert(page == VicorPage::LV_SIDE || page == VicorPage::HV_SIDE);

    if (page != current_page)
    {
        RETURN_IF_ERR(writeByte(CMD_PAGE, static_cast<uint8_t>(page)));
        current_page = page;
    }

    return {};
}

result<void> vicor_operation(const bool enable)
{
    RETURN_IF_ERR(enforcePage(VicorPage::HV_SIDE));

    return writeByte(CMD_OPERATION, static_cast<uint8_t>(enable << 7));
}

result<bool> vicor_read_operation()
{
    RETURN_IF_ERR(enforcePage(VicorPage::LV_SIDE));

    uint8_t op;
    RETURN_IF_ERR(readByte(CMD_OPERATION, op));
    return (op & 0x80) != 0;
}

result<void> vicor_clearFaults()
{
    RETURN_IF_ERR(enforcePage(VicorPage::LV_SIDE));
    static constexpr uint8_t CMD_CLEAR_FAULTS = 0x03;

    constexpr uint8_t cmd = CMD_CLEAR_FAULTS;
    return vicor_i2c.transmit(std::span(&cmd, 1));
}

result<VicorCapability> vicor_capability()
{
    RETURN_IF_ERR(enforcePage(VicorPage::LV_SIDE));
    static constexpr uint8_t CMD_CAPABILITY = 0x19;

    VicorCapability capability{};
    RETURN_IF_ERR(readByte(CMD_CAPABILITY, reinterpret_cast<uint8_t &>(capability)));
    return capability;
}

static float DECODE(const float m, const uint16_t Y, const float R, const float b)
{
    return 1.0f / m * (static_cast<float>(Y) * R - b);
}
result<VicorPowerStats> vicor_readPowerStats()
{
    RETURN_IF_ERR(enforcePage(VicorPage::HV_SIDE));
    static constexpr uint8_t CMD_READ_VIN  = 0x88;
    static constexpr uint8_t CMD_READ_IIN  = 0x89;
    static constexpr uint8_t CMD_READ_VOUT = 0x8B;
    static constexpr uint8_t CMD_READ_IOUT = 0x8C;
    static constexpr uint8_t CMD_READ_TEMP = 0x8D;
    static constexpr uint8_t CMD_READ_POUT = 0x96;

    // buffers
    VicorPowerStats out{};
    uint16_t        word;

    RETURN_IF_ERR(readWord(CMD_READ_VIN, word));
    out.vin = DECODE(1.0f, word, 1e-1f, 0);

    RETURN_IF_ERR(readWord(CMD_READ_IIN, word));
    out.iin = DECODE(1.0f, word, 1e-3f, 0);

    RETURN_IF_ERR(readWord(CMD_READ_VOUT, word));
    out.vout = DECODE(1.0f, word, 1e-1f, 0);

    RETURN_IF_ERR(readWord(CMD_READ_IOUT, word));
    out.iout = DECODE(1.0f, word, 1e-2f, 0);

    RETURN_IF_ERR(readWord(CMD_READ_TEMP, word));
    out.temp = DECODE(1.0f, word, 1e-0f, 0);

    RETURN_IF_ERR(readWord(CMD_READ_POUT, word));
    out.pout = DECODE(1.0f, word, 1e-0f, 0);

    return out;
}

result<VicorStatus> vicor_status()
{
    RETURN_IF_ERR(enforcePage(VicorPage::LV_SIDE));
    static constexpr uint8_t CMD_STATUS_WORD = 0x79;

    VicorStatus status{};
    RETURN_IF_ERR(readWord(CMD_STATUS_WORD, reinterpret_cast<uint16_t &>(status)));
    return status;
}

result<VicorCurrentOutputStatus> vicor_statusIout()
{
    RETURN_IF_ERR(enforcePage(VicorPage::LV_SIDE));
    static constexpr uint8_t CMD_STATUS_IOUT = 0x7B;

    VicorCurrentOutputStatus status{};
    RETURN_IF_ERR(readByte(CMD_STATUS_IOUT, reinterpret_cast<uint8_t &>(status)));
    return status;
}

result<VicorInputStatus> vicor_statusInput()
{
    RETURN_IF_ERR(enforcePage(VicorPage::LV_SIDE));

    static constexpr uint8_t CMD_STATUS_INPUT = 0x7C;
    VicorInputStatus         status{};
    RETURN_IF_ERR(readByte(CMD_STATUS_INPUT, reinterpret_cast<uint8_t &>(status)));
    return status;
}

result<VicorTempStatus> vicor_statusTemp()
{
    RETURN_IF_ERR(enforcePage(VicorPage::LV_SIDE));

    static constexpr uint8_t CMD_STATUS_TEMP = 0x7D;
    VicorTempStatus          status{};
    RETURN_IF_ERR(readByte(CMD_STATUS_TEMP, reinterpret_cast<uint8_t &>(status)));
    return status;
}

result<VicorCommStatus> vicor_statusComm()
{
    RETURN_IF_ERR(enforcePage(VicorPage::LV_SIDE));

    static constexpr uint8_t CMD_STATUS_CML = 0x7E;
    VicorCommStatus          status{};
    RETURN_IF_ERR(readByte(CMD_STATUS_CML, reinterpret_cast<uint8_t &>(status)));
    return status;
}

result<VicorStatusMFRSpecific> vicor_statusMfrSpecific()
{
    RETURN_IF_ERR(enforcePage(VicorPage::LV_SIDE));

    static constexpr uint8_t CMD_STATUS_MFR_SPECIFIC = 0x80;
    VicorStatusMFRSpecific   status{};
    RETURN_IF_ERR(readByte(CMD_STATUS_MFR_SPECIFIC, reinterpret_cast<uint8_t &>(status)));
    return status;
}

result<VicorMetadata> vicor_metadata()
{
    static constexpr uint8_t CMD_PMBUS_REVISION = 0x98;
    static constexpr uint8_t CMD_MFR_ID         = 0x99;
    static constexpr uint8_t CMD_MFR_MODEL      = 0x9A;
    static constexpr uint8_t CMD_MFR_REVISION   = 0x9B;
    static constexpr uint8_t CMD_MFR_LOCATION   = 0x9C;
    static constexpr uint8_t CMD_MFR_DATE       = 0x9D;
    static constexpr uint8_t CMD_MFR_SERIAL     = 0x9E;
    RETURN_IF_ERR(enforcePage(VicorPage::LV_SIDE));

    VicorMetadata out{};
    uint8_t       buf[20]{};

    static_assert(sizeof(out.pmbus_version) == 1, "pmbus_version should be 16 bytes");
    RETURN_IF_ERR(readByte(CMD_PMBUS_REVISION, out.pmbus_version));

    static_assert(sizeof(out.id) == 2 + 1, "mfr_id should be 2 bytes");
    RETURN_IF_ERR(vicor_i2c.memoryRead(CMD_MFR_ID, std::span(buf, 2 + 1)));
    memcpy(out.id, buf + 1, sizeof(out.id) - 1); // first byte is 0x12, the rest are the actual data

    static_assert(sizeof(out.part_number) == 18 + 1, "model should be 18 bytes");
    RETURN_IF_ERR(vicor_i2c.memoryRead(CMD_MFR_MODEL, std::span(buf, 18 + 1)));
    memcpy(out.part_number, buf + 1, sizeof(out.part_number) - 1);

    static_assert(sizeof(out.revision) == 18 + 1, "revision should be 18 bytes");
    RETURN_IF_ERR(vicor_i2c.memoryRead(CMD_MFR_REVISION, std::span(buf, 18 + 1)));
    memcpy(out.revision, buf + 1, sizeof(out.revision) - 1);

    static_assert(sizeof(out.location) == 2 + 1, "location should be 2 bytes");
    RETURN_IF_ERR(vicor_i2c.memoryRead(CMD_MFR_LOCATION, std::span(buf, 2 + 1)));
    memcpy(out.location, buf + 1, sizeof(out.location) - 1);

    static_assert(sizeof(out.date) == 4 + 1, "date should be 4 bytes");
    RETURN_IF_ERR(vicor_i2c.memoryRead(CMD_MFR_DATE, std::span(buf, 4 + 1)));
    memcpy(out.date, buf + 1, sizeof(out.date) - 1);

    static_assert(sizeof(out.serial_num) == 16 + 1, "serial should be 16 bytes");
    RETURN_IF_ERR(vicor_i2c.memoryRead(CMD_MFR_SERIAL, std::span(buf, 16 + 1)));
    memcpy(out.serial_num, buf + 1, sizeof(out.serial_num) - 1);

    return out;
}

result<VicorLimits> vicor_limits()
{
    RETURN_IF_ERR(enforcePage(VicorPage::LV_SIDE));

    static constexpr uint8_t CMD_MFR_VIN_MIN  = 0xA0;
    static constexpr uint8_t CMD_MFR_VIN_MAX  = 0xA1;
    static constexpr uint8_t CMD_MFR_VOUT_MIN = 0xA4;
    static constexpr uint8_t CMD_MFR_VOUT_MAX = 0xA5;
    static constexpr uint8_t CMD_MFR_IOUT_MAX = 0xA6;
    static constexpr uint8_t CMD_MFR_POUT_MAX = 0xA7;

    VicorLimits limits{};
    RETURN_IF_ERR(readWord(CMD_MFR_VIN_MIN, limits.vin_min));
    RETURN_IF_ERR(readWord(CMD_MFR_VIN_MAX, limits.vin_max));
    RETURN_IF_ERR(readWord(CMD_MFR_VOUT_MIN, limits.vout_min));
    RETURN_IF_ERR(readWord(CMD_MFR_VOUT_MAX, limits.vout_max));
    RETURN_IF_ERR(readWord(CMD_MFR_IOUT_MAX, limits.iout_max));
    RETURN_IF_ERR(readWord(CMD_MFR_POUT_MAX, limits.pout_max));
    return limits;
}