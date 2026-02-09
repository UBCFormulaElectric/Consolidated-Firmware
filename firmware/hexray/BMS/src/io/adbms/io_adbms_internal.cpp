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

static uint16_t calculatePec15(const uint8_t *data, const size_t len)
{
    static const uint16_t pec15Table[256] = {
        0x0000, 0xC599, 0xCEAB, 0x0B32, 0xD8CF, 0x1D56, 0x1664, 0xD3FD, 0xF407, 0x319E, 0x3AAC, 0xFF35, 0x2CC8, 0xE951,
        0xE263, 0x27FA, 0xAD97, 0x680E, 0x633C, 0xA6A5, 0x7558, 0xB0C1, 0xBBF3, 0x7E6A, 0x5990, 0x9C09, 0x973B, 0x52A2,
        0x815F, 0x44C6, 0x4FF4, 0x8A6D, 0x5B2E, 0x9EB7, 0x9585, 0x501C, 0x83E1, 0x4678, 0x4D4A, 0x88D3, 0xAF29, 0x6AB0,
        0x6182, 0xA41B, 0x77E6, 0xB27F, 0xB94D, 0x7CD4, 0xF6B9, 0x3320, 0x3812, 0xFD8B, 0x2E76, 0xEBEF, 0xE0DD, 0x2544,
        0x02BE, 0xC727, 0xCC15, 0x098C, 0xDA71, 0x1FE8, 0x14DA, 0xD143, 0xF3C5, 0x365C, 0x3D6E, 0xF8F7, 0x2B0A, 0xEE93,
        0xE5A1, 0x2038, 0x07C2, 0xC25B, 0xC969, 0x0CF0, 0xDF0D, 0x1A94, 0x11A6, 0xD43F, 0x5E52, 0x9BCB, 0x90F9, 0x5560,
        0x869D, 0x4304, 0x4836, 0x8DAF, 0xAA55, 0x6FCC, 0x64FE, 0xA167, 0x729A, 0xB703, 0xBC31, 0x79A8, 0xA8EB, 0x6D72,
        0x6640, 0xA3D9, 0x7024, 0xB5BD, 0xBE8F, 0x7B16, 0x5CEC, 0x9975, 0x9247, 0x57DE, 0x8423, 0x41BA, 0x4A88, 0x8F11,
        0x057C, 0xC0E5, 0xCBD7, 0x0E4E, 0xDDB3, 0x182A, 0x1318, 0xD681, 0xF17B, 0x34E2, 0x3FD0, 0xFA49, 0x29B4, 0xEC2D,
        0xE71F, 0x2286, 0xA213, 0x678A, 0x6CB8, 0xA921, 0x7ADC, 0xBF45, 0xB477, 0x71EE, 0x5614, 0x938D, 0x98BF, 0x5D26,
        0x8EDB, 0x4B42, 0x4070, 0x85E9, 0x0F84, 0xCA1D, 0xC12F, 0x04B6, 0xD74B, 0x12D2, 0x19E0, 0xDC79, 0xFB83, 0x3E1A,
        0x3528, 0xF0B1, 0x234C, 0xE6D5, 0xEDE7, 0x287E, 0xF93D, 0x3CA4, 0x3796, 0xF20F, 0x21F2, 0xE46B, 0xEF59, 0x2AC0,
        0x0D3A, 0xC8A3, 0xC391, 0x0608, 0xD5F5, 0x106C, 0x1B5E, 0xDEC7, 0x54AA, 0x9133, 0x9A01, 0x5F98, 0x8C65, 0x49FC,
        0x42CE, 0x8757, 0xA0AD, 0x6534, 0x6E06, 0xAB9F, 0x7862, 0xBDFB, 0xB6C9, 0x7350, 0x51D6, 0x944F, 0x9F7D, 0x5AE4,
        0x8919, 0x4C80, 0x47B2, 0x822B, 0xA5D1, 0x6048, 0x6B7A, 0xAEE3, 0x7D1E, 0xB887, 0xB3B5, 0x762C, 0xFC41, 0x39D8,
        0x32EA, 0xF773, 0x248E, 0xE117, 0xEA25, 0x2FBC, 0x0846, 0xCDDF, 0xC6ED, 0x0374, 0xD089, 0x1510, 0x1E22, 0xDBBB,
        0x0AF8, 0xCF61, 0xC453, 0x01CA, 0xD237, 0x17AE, 0x1C9C, 0xD905, 0xFEFF, 0x3B66, 0x3054, 0xF5CD, 0x2630, 0xE3A9,
        0xE89B, 0x2D02, 0xA76F, 0x62F6, 0x69C4, 0xAC5D, 0x7FA0, 0xBA39, 0xB10B, 0x7492, 0x5368, 0x96F1, 0x9DC3, 0x585A,
        0x8BA7, 0x4E3E, 0x450C, 0x8095
    };

    uint16_t remainder = 16U;

    for (size_t i = 0; i < len; i++)
    {
        const uint16_t address = (remainder >> 7 ^ data[i]) & 0xFFU;
        remainder              = static_cast<uint16_t>(static_cast<uint16_t>(remainder << 8) ^ pec15Table[address]);
    }
    return static_cast<uint16_t>(remainder << 1);
}

static constexpr uint16_t CRC10_POLY = 0x8F;
static constexpr uint16_t CRC10_INIT = 0x10;
static constexpr uint16_t MSB_10     = 1 << (10 - 1);
static constexpr uint16_t MASK_10    = (1 << 10) - 1;

static constexpr std::array<uint16_t, 256> get_pec10_table()
{
    std::array<uint16_t, 256> out{};
    for (uint16_t i = 0; i < 256; i++)
    {
        auto r = static_cast<uint16_t>(i << 2); // r = 4 * i
        for (int b = 7; b >= 0; --b)
        {
            if (r & MSB_10)
            { // check msb is set
                r = static_cast<uint16_t>(r << 1);
                r ^= CRC10_POLY;
            }
            else
            {
                r = static_cast<uint16_t>(r << 1);
            }
        }
        out[i] = r & MASK_10;
    }
    return out;
}
static constexpr std::array<uint16_t, 256> pec10Table = get_pec10_table();

uint16_t calculatePec10(const std::span<const uint8_t> data)
{
    uint16_t r = CRC10_INIT;
    for (const auto d : data)
    {
        const auto addy = static_cast<uint8_t>(r >> 2 ^ d);
        r               = static_cast<uint16_t>(static_cast<uint16_t>(r << 8) ^ pec10Table[addy]);
        r &= MASK_10;
    }
    return r;
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

    const CmdPayload       tx_cmd = buildCmd(cmd);
    static RegGroupPayload rx_buffer[NUM_SEGMENTS];

    if (const auto tx_status = hw::spi::adbms_spi_ls.transmitThenReceive(
            { reinterpret_cast<const uint8_t *>(&tx_cmd), sizeof(tx_cmd) },
            { reinterpret_cast<uint8_t *>(&rx_buffer), sizeof(rx_buffer) });
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
