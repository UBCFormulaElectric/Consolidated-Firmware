#include "io_adbms_internal.hpp"
#include "io_adbms.hpp"
#include "io_log.hpp"
#include "util_errorCodes.hpp"
#include "hw_spis.hpp"
#include <cstring>

using namespace std;

namespace
{
constexpr std::array<uint16_t, 256> pecTable(const uint16_t poly, const uint16_t size)
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

constexpr uint16_t             CRC10_POLY = 0x8F;
constexpr uint16_t             CRC15_POLY = 0x4599;
constexpr array<uint16_t, 256> pec10Table = pecTable(CRC10_POLY, 10);
constexpr array<uint16_t, 256> pec15Table = pecTable(CRC15_POLY, 15);

uint16_t swapEndianness(const uint16_t value)
{
    return static_cast<uint16_t>(value >> 8 | value << 8);
}

uint16_t calculatePec10(const span<const uint8_t> data, const uint16_t cmdCount)
{
    uint16_t remainder = 16U;

    for (const unsigned char i : data)
    {
        const uint16_t address = (remainder >> 2 ^ i) & 0xFFU;
        remainder              = static_cast<uint16_t>(static_cast<uint16_t>(remainder << 8) ^ pec10Table[address]);
        remainder &= 0x03FFU;
    }

    remainder ^= static_cast<uint16_t>(cmdCount) << 4;
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

uint16_t calculatePec15(const span<const uint8_t> data)
{
    uint16_t remainder = 16U;

    for (const unsigned char i : data)
    {
        const uint16_t address = ((remainder >> 7) ^ i) & 0xFFU;
        remainder              = static_cast<uint16_t>(static_cast<uint16_t>(remainder << 8) ^ pec15Table[address]);
    }
    return static_cast<uint16_t>(remainder << 1U);
}

bool checkDataPec(const span<const uint8_t> data, const uint16_t cmdCount, const uint16_t expected)
{
    return calculatePec10(data, cmdCount) == swapEndianness(expected);
}
} // namespace

struct __attribute__((packed)) RegGroupPayload
{
    array<uint8_t, REG_GROUP_SIZE> data;
    uint16_t                       pec10;
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
    [[nodiscard]] span<uint8_t>       into_span() { return { reinterpret_cast<uint8_t *>(this), sizeof(TxCmd) }; }
    [[nodiscard]] span<const uint8_t> into_span() const
    {
        return { reinterpret_cast<const uint8_t *>(this), sizeof(TxCmd) };
    }
};

struct __attribute__((packed)) TxCmdPayload
{
    // ReSharper disable once CppDeclaratorNeverUsed
    TxCmd                                tx_cmd;
    array<RegGroupPayload, NUM_SEGMENTS> payload;

    [[nodiscard]] span<uint8_t> into_span() { return { reinterpret_cast<uint8_t *>(this), sizeof(TxCmdPayload) }; }
};

namespace io::adbms
{

array<array<uint8_t, REG_GROUP_SIZE>, NUM_SEGMENTS> shared_reg_group;
array<expected<void, ErrorCode>, NUM_SEGMENTS>      shared_reg_group_success;

expected<void, ErrorCode> sendCmd(const uint16_t cmd)
{
    const TxCmd tx_cmd{ cmd };
    return adbms_spi_ls.transmit(tx_cmd.into_span());
}

expected<void, ErrorCode> poll(const uint16_t cmd, const span<uint8_t> poll_buf)
{
    const TxCmd tx_cmd{ cmd };
    return adbms_spi_ls.transmitThenReceive({ reinterpret_cast<const uint8_t *>(&tx_cmd), sizeof(tx_cmd) }, poll_buf);
}

array<expected<array<uint8_t, REG_GROUP_SIZE>, ErrorCode>, NUM_SEGMENTS> readRegGroup(const uint16_t cmd)
{
    array<expected<array<uint8_t, REG_GROUP_SIZE>, ErrorCode>, NUM_SEGMENTS> regs;
    const TxCmd                                                              tx_cmd{ cmd };
    array<RegGroupPayload, NUM_SEGMENTS>                                     rx_buffer{};

    const auto comm_status = adbms_spi_ls.transmitThenReceive(
        tx_cmd.into_span(), { reinterpret_cast<uint8_t *>(rx_buffer.data()), sizeof(rx_buffer) });
    if (!comm_status)
    {
        regs.fill(std::unexpected(comm_status.error()));
        return regs;
    }

    for (size_t segment = 0U; segment < NUM_SEGMENTS; ++segment)
    {
        const auto &[payload_data, payload_pec10] = rx_buffer[segment];
        const auto cmd_count                      = static_cast<uint16_t>(swapEndianness(payload_pec10) >> 10U);

        if (const auto expected_pec = static_cast<uint16_t>(payload_pec10 & 0xFF03U);
            checkDataPec(payload_data, cmd_count, expected_pec))
        {
            regs[segment] = payload_data;
        }
        else
        {
            regs[segment] = unexpected(ErrorCode::CHECKSUM_FAIL);
        }
    }
    return regs;
}

expected<void, ErrorCode>
    writeRegGroup(const uint16_t cmd, const array<array<uint8_t, REG_GROUP_SIZE>, NUM_SEGMENTS> &regs)
{
    TxCmdPayload tx_buffer{ TxCmd{ cmd }, {} };

    for (size_t segment = 0U; segment < NUM_SEGMENTS; ++segment)
    {
        auto &[data, pec10] = tx_buffer.payload[segment];
        data                = regs[segment];
        pec10               = swapEndianness(calculatePec10(data, 0U) & 0x03FFU);
    }

    return adbms_spi_ls.transmit(tx_buffer.into_span());
}

} // namespace io::adbms
