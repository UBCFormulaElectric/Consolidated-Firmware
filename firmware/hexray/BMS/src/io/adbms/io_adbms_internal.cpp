#include "io_adbms_internal.hpp"
#include "io_adbms.hpp"
#include "util_errorCodes.hpp"
#include "hw_spis.hpp"

#include <cstring>

namespace
{
struct CmdPayload
{
    uint16_t cmd;
    uint16_t pec15;
    CmdPayload(const uint16_t _cmd, const uint16_t _pec15) : cmd(_cmd), pec15(_pec15) {}
};

struct RegGroupPayload
{
    std::array<uint8_t, io::adbms::REG_GROUP_SIZE> data;
    uint16_t                                       pec10;
};

static_assert(sizeof(CmdPayload) == io::adbms::CMD_BYTES + io::adbms::PEC_BYTES);
static_assert(sizeof(RegGroupPayload) == io::adbms::REG_GROUP_SIZE + io::adbms::PEC_BYTES);
} // namespace

namespace io::adbms
{

static uint16_t swapEndianness(const uint16_t value)
{
    return static_cast<uint16_t>(value >> 8 | value << 8);
}

static constexpr uint16_t CRC10_POLY = 0x8F;
static constexpr uint16_t CRC10_INIT = 0x10;
static constexpr uint16_t CRC15_POLY = 0x4599;
static constexpr uint16_t CRC15_INIT = 0x10;

static constexpr std::array<uint16_t, 256> get_pec_table(const uint16_t poly, const uint16_t size)
{
    const auto MSB_SIZE  = static_cast<uint16_t>(1 << (size - 1));
    const auto MASK_SIZE = static_cast<uint16_t>((1 << (size + 1)) - 1);

    std::array<uint16_t, 256> out{};
    for (uint16_t i = 0; i < 256; i++)
    {
        auto r = static_cast<uint16_t>(i << (size - 8)); // r = 4 * i
        for (int b = 7; b >= 0; --b)
        {
            if (r & MSB_SIZE)
            { // check msb is set
                r = static_cast<uint16_t>(r << 1);
                r ^= poly;
            }
            else
            {
                r = static_cast<uint16_t>(r << 1);
            }
        }
        out[i] = r & MASK_SIZE;
    }
    return out;
}
static constexpr std::array<uint16_t, 256> pec10Table = get_pec_table(CRC10_POLY, 10);
static constexpr std::array<uint16_t, 256> pec15Table = get_pec_table(CRC15_POLY, 15);

uint16_t calculatePec10(const std::span<const uint8_t> data)
{
    uint16_t r = CRC10_INIT;
    for (const auto d : data)
    {
        const auto addy = static_cast<uint8_t>(r >> 2 ^ d);
        r               = static_cast<uint16_t>(static_cast<uint16_t>(r << 8) ^ pec10Table[addy]);
        r &= (1 << 10) - 1;
    }
    return static_cast<uint16_t>(r << 1);
}
uint16_t calculatePec15(const uint8_t *data, const size_t len)
{
    uint16_t r = CRC15_INIT;
    for (size_t i = 0; i < len; i++)
    {
        const auto addy = static_cast<uint8_t>(r >> 7 ^ data[i]);
        r               = static_cast<uint16_t>(static_cast<uint16_t>(r << 8) ^ pec15Table[addy]);
    }
    return static_cast<uint16_t>(r << 1);
}

static uint16_t buildDataPec(const std::span<const uint8_t> data)
{
    return swapEndianness(calculatePec10(data));
}

static bool checkDataPec(const std::span<const uint8_t> data, const uint16_t expected)
{
    return buildDataPec(data) == expected;
}

static CmdPayload buildCmd(uint16_t command)
{
    return { swapEndianness(command),
             swapEndianness(calculatePec15(reinterpret_cast<uint8_t *>(&command), sizeof(command))) };
}

std::expected<void, ErrorCode> sendCmd(const uint16_t cmd)
{
    const CmdPayload tx_cmd = buildCmd(cmd);
    return hw::spi::adbms_spi_ls.transmit({ reinterpret_cast<const uint8_t *>(&tx_cmd), sizeof(tx_cmd) });
}

std::expected<void, ErrorCode> poll(const uint16_t cmd, const std::span<uint8_t> poll_buf)
{
    const CmdPayload tx_cmd = buildCmd(cmd);
    return hw::spi::adbms_spi_ls.transmitThenReceive(
        { reinterpret_cast<const uint8_t *>(&tx_cmd), sizeof(tx_cmd) }, poll_buf);
}

void readRegGroup(
    const uint16_t                                                 cmd,
    std::array<std::array<uint8_t, REG_GROUP_SIZE>, NUM_SEGMENTS> &regs,
    std::array<std::expected<void, ErrorCode>, NUM_SEGMENTS>      &comm_success)
{
    std::memset(regs.data(), 0, sizeof(regs));

    const CmdPayload                                 tx_cmd = buildCmd(cmd);
    static std::array<RegGroupPayload, NUM_SEGMENTS> rx_buffer;

    if (const auto tx_status = hw::spi::adbms_spi_ls.transmitThenReceive(
            { reinterpret_cast<const uint8_t *>(&tx_cmd), sizeof(tx_cmd) },
            { reinterpret_cast<uint8_t *>(rx_buffer.data()), sizeof(rx_buffer) });
        not tx_status)
    {
        for (uint8_t segment = 0U; segment < NUM_SEGMENTS; segment++)
        {
            comm_success[segment] = tx_status;
        }
        return;
    }

    for (uint8_t segment = 0U; segment < NUM_SEGMENTS; segment++)
    {
        const auto &[data, pec10] = rx_buffer[segment];
        if (checkDataPec(data, swapEndianness(pec10)))
        {
            regs[segment]         = data;
            comm_success[segment] = {};
        }
        else
        {
            comm_success[segment] = std::unexpected(ErrorCode::CHECKSUM_FAIL);
        }
    }
}

std::expected<void, ErrorCode>
    writeRegGroup(const uint16_t cmd, const std::array<std::array<uint8_t, REG_GROUP_SIZE>, NUM_SEGMENTS> regs)
{
    struct
    {
        CmdPayload                                tx_cmd;
        std::array<RegGroupPayload, NUM_SEGMENTS> payload;
    } tx_buffer{ buildCmd(cmd), {} };
    for (uint8_t segment = 0U; segment < NUM_SEGMENTS; segment++)
    {
        tx_buffer.payload[segment].data  = regs[segment];
        tx_buffer.payload[segment].pec10 = buildDataPec(tx_buffer.payload[segment].data);
    }
    return hw::spi::adbms_spi_ls.transmit({ reinterpret_cast<uint8_t *>(&tx_buffer), sizeof(tx_buffer) });
}
} // namespace io::adbms
