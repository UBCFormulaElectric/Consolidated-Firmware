#include "io_adbms_internal.hpp"
#include "io_adbms.hpp"
#include "util_errorCodes.hpp"
#include "hw_spis.hpp"

#include <cstring>

struct __attribute__((packed)) RegGroupPayload
{
    std::array<uint8_t, io::adbms::REG_GROUP_SIZE> data;
    uint16_t                                       cmdCount : 6;
    uint16_t                                       pec10 : 10;
};
static_assert(sizeof(RegGroupPayload) == io::adbms::REG_GROUP_SIZE + io::adbms::PEC_BYTES);

namespace io::adbms
{

static uint16_t swapEndianness(const uint16_t value)
{
    return static_cast<uint16_t>(value >> 8 | value << 8);
}

static constexpr uint16_t CRC10_POLY = 0x8F;
static constexpr uint16_t CRC15_POLY = 0x4599;

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

uint16_t calculatePec10(const std::span<const uint8_t> data, const uint16_t cmdCount)
{
    uint16_t remainder = 16U;
    uint16_t address;

    for (size_t i = 0; i < data.size(); i++) {
        address   = ((remainder >> 2) ^ data[i]) & 0xFFU;
        remainder = (uint16_t)(((uint16_t)(remainder << 8)) ^ pec10Table[address]);
        remainder &= 0x03FFU;  
    }

    remainder ^= ((uint16_t)cmdCount<< 4);
    for (int bit = 0; bit < 6; bit++) {
        if (remainder & 0x0200)
            remainder = static_cast<uint16_t>((remainder << 1U) ^ CRC10_POLY);
        else
            remainder = static_cast<uint16_t>(remainder << 1U);
        remainder &= 0x03FF;
    }
    return remainder;
}

uint16_t calculatePec15(const std::span<const uint8_t> data)
{
    uint16_t remainder = 16U;
    uint16_t address;

    for (size_t i = 0; i < data.size(); i++) {
        address   = ((remainder >> 7) ^ data[i]) & 0xFFU;
        remainder = (uint16_t)(((uint16_t)(remainder << 8)) ^ pec15Table[address]);
    }
    return static_cast<uint16_t>(remainder << 1U);
}

static bool checkDataPec(const std::span<const uint8_t> data, const uint16_t cmdCount, const uint16_t expected)
{
    return swapEndianness(calculatePec10(data, cmdCount)) == expected;
}

struct TxCmd
{
    uint16_t cmd;
    uint16_t pec15;
    explicit TxCmd(const uint16_t _cmd)
    {
        cmd   = swapEndianness(_cmd);
        pec15 = swapEndianness(calculatePec15({ reinterpret_cast<const uint8_t *>(&cmd), sizeof(cmd) }));
    }
};
static_assert(sizeof(TxCmd) == CMD_BYTES + PEC_BYTES);

std::expected<void, ErrorCode> sendCmd(const uint16_t cmd)
{
    const TxCmd tx_cmd{ cmd };
    return hw::spi::adbms_spi_ls.transmit({ reinterpret_cast<const uint8_t *>(&tx_cmd), sizeof(tx_cmd) });
}

std::expected<void, ErrorCode> poll(const uint16_t cmd, const std::span<uint8_t> poll_buf)
{
    const TxCmd tx_cmd{ cmd };
    return hw::spi::adbms_spi_ls.transmitThenReceive(
        { reinterpret_cast<const uint8_t *>(&tx_cmd), sizeof(tx_cmd) }, poll_buf);
}

void readRegGroup(
    const uint16_t                                                 cmd,
    std::array<std::array<uint8_t, REG_GROUP_SIZE>, NUM_SEGMENTS> &regs,
    std::array<std::expected<void, ErrorCode>, NUM_SEGMENTS>      &comm_success)
{
    std::memset(regs.data(), 0, sizeof(regs));

    const TxCmd                                      tx_cmd{ cmd };
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
        const auto &payload = rx_buffer[segment];
        if (checkDataPec(payload.data, payload.cmdCount, swapEndianness(payload.pec10)))
        {
            regs[segment]         = payload.data;
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
        TxCmd                                     tx_cmd;
        std::array<RegGroupPayload, NUM_SEGMENTS> payload;
    } tx_buffer{ TxCmd{ cmd }, {} };

    for (uint8_t segment = 0U; segment < NUM_SEGMENTS; segment++)
    {
        tx_buffer.payload[segment].data  = regs[segment];
        tx_buffer.payload[segment].pec10 = static_cast<uint16_t>(
            swapEndianness(calculatePec10(tx_buffer.payload[segment].data, tx_buffer.payload[segment].cmdCount)) &
            0x03FFU);
    }
    return hw::spi::adbms_spi_ls.transmit({ reinterpret_cast<uint8_t *>(&tx_buffer), sizeof(tx_buffer) });
}
} // namespace io::adbms
