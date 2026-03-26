#include "io_adbms_internal.hpp"
#include "io_adbms.hpp"
#include "util_errorCodes.hpp"
#include "hw_spis.hpp"
#include <cstring>

using namespace std;

static constexpr uint16_t CRC10_POLY = 0x8F;
static constexpr uint16_t CRC15_POLY = 0x4599;

static constexpr std::array<uint16_t, 256> pecTable(const uint16_t poly, const uint16_t size)
{
    const auto MSB_SIZE  = static_cast<uint16_t>(1 << (size - 1));
    const auto MASK_SIZE = static_cast<uint16_t>((1 << (size + 1)) - 1);

    array<uint16_t, 256> out{};
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

static constexpr array<uint16_t, 256> pec10Table = pecTable(CRC10_POLY, 10);
static constexpr array<uint16_t, 256> pec15Table = pecTable(CRC15_POLY, 15);

static uint16_t swapEndianness(const uint16_t value)
{
    return static_cast<uint16_t>(value >> 8 | value << 8);
}

static uint16_t calculatePec10(const span<const uint8_t> data, const uint16_t cmdCount)
{
    uint16_t remainder = 16U;
    uint16_t address;

    for (size_t i = 0; i < data.size(); i++)
    {
        address   = ((remainder >> 2) ^ data[i]) & 0xFFU;
        remainder = (uint16_t)(((uint16_t)(remainder << 8)) ^ pec10Table[address]);
        remainder &= 0x03FFU;
    }

    remainder ^= ((uint16_t)cmdCount << 4);
    for (int bit = 0; bit < 6; bit++)
    {
        if (remainder & 0x0200)
            remainder = static_cast<uint16_t>((remainder << 1U) ^ CRC10_POLY);
        else
            remainder = static_cast<uint16_t>(remainder << 1U);
        remainder &= 0x03FF;
    }
    return remainder;
}

static uint16_t calculatePec15(const span<const uint8_t> data)
{
    uint16_t remainder = 16U;
    uint16_t address;

    for (size_t i = 0; i < data.size(); i++)
    {
        address   = ((remainder >> 7) ^ data[i]) & 0xFFU;
        remainder = (uint16_t)(((uint16_t)(remainder << 8)) ^ pec15Table[address]);
    }
    return static_cast<uint16_t>(remainder << 1U);
}

static bool checkDataPec(const span<const uint8_t> data, const uint16_t cmdCount, const uint16_t expected)
{
    return calculatePec10(data, cmdCount) == swapEndianness(expected);
}

struct __attribute__((packed)) RegGroupPayload
{
    array<uint8_t, io::adbms::REG_GROUP_SIZE> data;
    uint16_t                                  pec10;
};

struct __attribute__((packed)) TxCmd
{
    uint16_t cmd;
    uint16_t pec15;
    explicit TxCmd(const uint16_t _cmd)
    {
        cmd   = swapEndianness(_cmd);
        pec15 = swapEndianness(calculatePec15({ reinterpret_cast<const uint8_t *>(&cmd), sizeof(cmd) }));
    }
};

struct __attribute__((packed)) TxCmdPayload
{
    TxCmd                                    tx_cmd;
    array<RegGroupPayload, io::NUM_SEGMENTS> payload;
};

namespace io::adbms
{

expected<void, ErrorCode> sendCmd(const uint16_t cmd)
{
    const TxCmd tx_cmd{ cmd };
    return hw::spi::adbms_spi_ls.transmit({ reinterpret_cast<const uint8_t *>(&tx_cmd), sizeof(tx_cmd) });
}

expected<void, ErrorCode> poll(const uint16_t cmd, span<uint8_t> poll_buf)
{
    const TxCmd tx_cmd{ cmd };
    return hw::spi::adbms_spi_ls.transmitThenReceive(
        { reinterpret_cast<const uint8_t *>(&tx_cmd), sizeof(tx_cmd) }, poll_buf);
}

void readRegGroup(
    const uint16_t                                       cmd,
    array<array<uint8_t, REG_GROUP_SIZE>, NUM_SEGMENTS> &regs,
    array<expected<void, ErrorCode>, NUM_SEGMENTS>      &comm_success)
{

    const TxCmd                          tx_cmd{ cmd };
    array<RegGroupPayload, NUM_SEGMENTS> rx_buffer{};

    const auto tx_status = hw::spi::adbms_spi_ls.transmitThenReceive(
        { reinterpret_cast<const uint8_t *>(&tx_cmd), sizeof(tx_cmd) },
        { reinterpret_cast<uint8_t *>(rx_buffer.data()), sizeof(rx_buffer) });

    if (!tx_status)
    {
        regs.fill({});
        comm_success.fill(tx_status);
        return;
    }

    if (cmd == RDAUXA) {
        [[maybe_unused]] int j = 0;
    }
    if (cmd == RDAUXB) {
        [[maybe_unused]] int j = 0;
    }
    if (cmd == RDAUXC) {
        [[maybe_unused]] int j = 0;
    }
    if (cmd == RDAUXD) {
        [[maybe_unused]] int j = 0;
    }

    for (size_t segment = 0U; segment < NUM_SEGMENTS; ++segment)
    {
        const auto &payload      = rx_buffer[segment];
        const auto  cmd_count    = static_cast<uint16_t>(swapEndianness(payload.pec10) >> 10U);
        const auto  expected_pec = static_cast<uint16_t>(payload.pec10 & 0xFF03U);

        if (checkDataPec(payload.data, cmd_count, expected_pec))
        {
            regs[segment]         = payload.data;
            comm_success[segment] = {};
        }
        else
        {
            regs[segment].fill(0U);
            comm_success[segment] = unexpected(ErrorCode::CHECKSUM_FAIL);
        }
    }
}

expected<void, ErrorCode>
    writeRegGroup(const uint16_t cmd, const array<array<uint8_t, REG_GROUP_SIZE>, NUM_SEGMENTS> &regs)
{
    TxCmdPayload tx_buffer{ TxCmd{ cmd }, {} };

    for (size_t segment = 0U; segment < NUM_SEGMENTS; ++segment)
    {
        auto &payload = tx_buffer.payload[segment];
        payload.data  = regs[segment];
        payload.pec10 = static_cast<uint16_t>(swapEndianness(calculatePec10(payload.data, 0U) & 0x03FFU));
    }

    return hw::spi::adbms_spi_ls.transmit({ reinterpret_cast<const uint8_t *>(&tx_buffer), sizeof(tx_buffer) });
}

} // namespace io::adbms
// namespace io::adbms
