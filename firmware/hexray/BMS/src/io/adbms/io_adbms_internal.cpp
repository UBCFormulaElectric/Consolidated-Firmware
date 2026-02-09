#include "io_adbms_internal.hpp"
#include "io_adbms.hpp"
#include "util_errorCodes.hpp"
#include "hw_spis.hpp"

#include <cstring>

struct RegGroupPayload
{
    std::array<uint8_t, io::adbms::REG_GROUP_SIZE> data;
    uint16_t                                       pec10;
};
static_assert(sizeof(RegGroupPayload) == io::adbms::REG_GROUP_SIZE + io::adbms::PEC_BYTES);

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
    static const uint16_t pec10Table[256] = {
    0x000, 0x48F, 0x11E, 0x591, 0x23C, 0x6B3, 0x322, 0x7AD, 0x4F7, 0x078, 0x5E9, 0x166, 0x6CB, 0x244, 0x7D5, 0x35A, 
    0x1EE, 0x561, 0x0F0, 0x47F, 0x3D2, 0x75D, 0x2CC, 0x643, 0x519, 0x196, 0x407, 0x088, 0x725, 0x3AA, 0x63B, 0x2B4, 
    0x3DC, 0x753, 0x2C2, 0x64D, 0x1E0, 0x56F, 0x0FE, 0x471, 0x72B, 0x3A4, 0x635, 0x2BA, 0x517, 0x198, 0x409, 0x086, 
    0x232, 0x6BD, 0x32C, 0x7A3, 0x00E, 0x481, 0x110, 0x59F, 0x6C5, 0x24A, 0x7DB, 0x354, 0x4F9, 0x076, 0x5E7, 0x168, 
    0x737, 0x3B8, 0x629, 0x2A6, 0x50B, 0x184, 0x415, 0x09A, 0x3C0, 0x74F, 0x2DE, 0x651, 0x1FC, 0x573, 0x0E2, 0x46D, 
    0x6D9, 0x256, 0x7C7, 0x348, 0x4E5, 0x06A, 0x5FB, 0x174, 0x22E, 0x6A1, 0x330, 0x7BF, 0x012, 0x49D, 0x10C, 0x583, 
    0x4EB, 0x064, 0x5F5, 0x17A, 0x6D7, 0x258, 0x7C9, 0x346, 0x01C, 0x493, 0x102, 0x58D, 0x220, 0x6AF, 0x33E, 0x7B1, 
    0x505, 0x18A, 0x41B, 0x094, 0x739, 0x3B6, 0x627, 0x2A8, 0x1F2, 0x57D, 0x0EC, 0x463, 0x3CE, 0x741, 0x2D0, 0x65F, 
    0x6E1, 0x26E, 0x7FF, 0x370, 0x4DD, 0x052, 0x5C3, 0x14C, 0x216, 0x699, 0x308, 0x787, 0x02A, 0x4A5, 0x134, 0x5BB, 
    0x70F, 0x380, 0x611, 0x29E, 0x533, 0x1BC, 0x42D, 0x0A2, 0x3F8, 0x777, 0x2E6, 0x669, 0x1C4, 0x54B, 0x0DA, 0x455, 
    0x53D, 0x1B2, 0x423, 0x0AC, 0x701, 0x38E, 0x61F, 0x290, 0x1CA, 0x545, 0x0D4, 0x45B, 0x3F6, 0x779, 0x2E8, 0x667, 
    0x4D3, 0x05C, 0x5CD, 0x142, 0x6EF, 0x260, 0x7F1, 0x37E, 0x024, 0x4AB, 0x13A, 0x5B5, 0x218, 0x697, 0x306, 0x789, 
    0x1D6, 0x559, 0x0C8, 0x447, 0x3EA, 0x765, 0x2F4, 0x67B, 0x521, 0x1AE, 0x43F, 0x0B0, 0x71D, 0x392, 0x603, 0x28C, 
    0x038, 0x4B7, 0x126, 0x5A9, 0x204, 0x68B, 0x31A, 0x795, 0x4CF, 0x040, 0x5D1, 0x15E, 0x6F3, 0x27C, 0x7ED, 0x362, 
    0x20A, 0x685, 0x314, 0x79B, 0x036, 0x4B9, 0x128, 0x5A7, 0x6FD, 0x272, 0x7E3, 0x36C, 0x4C1, 0x04E, 0x5DF, 0x150, 
    0x3E4, 0x76B, 0x2FA, 0x675, 0x1D8, 0x557, 0x0C6, 0x449, 0x713, 0x39C, 0x60D, 0x282, 0x52F, 0x1A0, 0x431, 0x0BE
    };

    uint16_t remainder = 16U;
    uint16_t address;

    for (size_t i = 0; i < len; i++)
{
    address   = ((remainder >> 2) ^ data[i]) & 0xFFU;
    remainder = (uint16_t)(((uint16_t)(remainder << 8)) ^ pec10Table[address]);
    remainder &= 0x03FFU;  
}

    return remainder;
}

static uint16_t buildDataPec(const std::span<const uint8_t> data)
{
    return swapEndianness(calculatePec10(data));
}

static bool checkDataPec(const std::span<const uint8_t> data, const uint16_t expected)
{
    return buildDataPec(data) == expected;
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
        TxCmd                                     tx_cmd;
        std::array<RegGroupPayload, NUM_SEGMENTS> payload;
    } tx_buffer{ TxCmd{ cmd }, {} };
    for (uint8_t segment = 0U; segment < NUM_SEGMENTS; segment++)
    {
        tx_buffer.payload[segment].data  = regs[segment];
        tx_buffer.payload[segment].pec10 = buildDataPec(tx_buffer.payload[segment].data);
    }
    return hw::spi::adbms_spi_ls.transmit({ reinterpret_cast<uint8_t *>(&tx_buffer), sizeof(tx_buffer) });
}
} // namespace io::adbms
