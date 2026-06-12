#pragma once

#include "io_adbms.hpp"

#include <FreeRTOS.h>
#include <semphr.h>

#include <array>
#include <span>

using namespace std;

namespace io::adbms
{

// ============================== ADBMS6830 command set ==============================
// Transaction framing constants.
inline constexpr uint8_t CMD_BYTES = 2;
inline constexpr uint8_t PEC_BYTES = 2;

// Config
inline constexpr uint16_t WRCFGA = 0x0001U;
inline constexpr uint16_t WRCFGB = 0x0024U;
inline constexpr uint16_t RDCFGA = 0x0002U;
inline constexpr uint16_t RDCFGB = 0x0026U;

// Cell voltages (C-ADC) — A..E only; 14 cells → Group F unused.
inline constexpr uint16_t RDCVA = 0x0004U;
inline constexpr uint16_t RDCVB = 0x0006U;
inline constexpr uint16_t RDCVC = 0x0008U;
inline constexpr uint16_t RDCVD = 0x000AU;
inline constexpr uint16_t RDCVE = 0x0009U;

// Redundant cell voltages (S-ADC) — A..E only.
inline constexpr uint16_t RDSVA = 0x0003U;
inline constexpr uint16_t RDSVB = 0x0005U;
inline constexpr uint16_t RDSVC = 0x0007U;
inline constexpr uint16_t RDSVD = 0x000DU;
inline constexpr uint16_t RDSVE = 0x000EU;

// Filtered cell voltages — A..E only.
inline constexpr uint16_t RDFCA = 0x0012U;
inline constexpr uint16_t RDFCB = 0x0013U;
inline constexpr uint16_t RDFCC = 0x0014U;
inline constexpr uint16_t RDFCD = 0x0015U;
inline constexpr uint16_t RDFCE = 0x0016U;

// AUX & Status
inline constexpr uint16_t RDAUXA  = 0x0019U;
inline constexpr uint16_t RDAUXB  = 0x001AU;
inline constexpr uint16_t RDAUXC  = 0x001BU;
inline constexpr uint16_t RDAUXD  = 0x001FU;
inline constexpr uint16_t RDSTATA = 0x0030U;
inline constexpr uint16_t RDSTATB = 0x0031U;
inline constexpr uint16_t RDSTATC = 0x0032U;
inline constexpr uint16_t RDSTATD = 0x0033U;
inline constexpr uint16_t RDSTATE = 0x0034U;
inline constexpr uint16_t RDASALL = 0x0035U;

// PWM
inline constexpr uint16_t WRPWMA = 0x0020U;
inline constexpr uint16_t WRPWMB = 0x0021U;
inline constexpr uint16_t RDPWMA = 0x0022U;
inline constexpr uint16_t RDPWMB = 0x0023U;

// Clear
inline constexpr uint16_t CLRCELL = 0x0711U;
inline constexpr uint16_t CLRAUX  = 0x0712U;
inline constexpr uint16_t CLRSPIN = 0x0716U;
inline constexpr uint16_t CLRFLAG = 0x0717U;
inline constexpr uint16_t CLOVUV  = 0x0715U;
inline constexpr uint16_t CLRFC   = 0x0714U;

// Poll
inline constexpr uint16_t PLADC  = 0x0718U;
inline constexpr uint16_t PLCADC = 0x071CU;
inline constexpr uint16_t PLSADC = 0x071DU;
inline constexpr uint16_t PLAUX  = 0x071EU;
inline constexpr uint16_t PLAUX2 = 0x071FU;

// Discharge
inline constexpr uint16_t MUTE   = 0x0028U;
inline constexpr uint16_t UNMUTE = 0x0029U;

// Start
inline constexpr uint16_t ADCV_BASE  = 0x0270U;
inline constexpr uint16_t ADSV_BASE  = 0x0168U;
inline constexpr uint16_t ADAX_BASE  = 0x0410U;
inline constexpr uint16_t ADAX2_BASE = 0x0400U;

// Snapshot
inline constexpr uint16_t SNAP   = 0x002DU;
inline constexpr uint16_t UNSNAP = 0x002FU;

// Counter resets — RSTCC clears the command counter
inline constexpr uint16_t RSTCC = 0x002EU;

// ADCV and ADSV option bits
inline constexpr uint16_t RD   = 1U << 8; // redundant C + S ADC
inline constexpr uint16_t CONT = 1U << 7; // continuous mode
inline constexpr uint16_t DCP  = 1U << 4; // discharge during conversion
inline constexpr uint16_t RSTF = 1U << 2; // reset IIR filters
inline constexpr uint16_t OW1  = 1U << 1; // open wire
inline constexpr uint16_t OW0  = 1U << 0;

// ADAX and ADAX2 option bits
inline constexpr uint16_t OW  = 1U << 8; // open wire
inline constexpr uint16_t PUP = 1U << 7; // pull up or down current sources
inline constexpr uint16_t CH4 = 1U << 6; // channel select
inline constexpr uint16_t CH3 = 1U << 3;
inline constexpr uint16_t CH2 = 1U << 2;
inline constexpr uint16_t CH1 = 1U << 1;
inline constexpr uint16_t CH0 = 1U << 0;

// Poll commands return a packed readiness bitmap
inline constexpr uint32_t POLL_STATUS_READY  = __builtin_bswap32(0xFFFFFFFFU >> (2 * NUM_SEGMENTS));
inline constexpr uint8_t  POLL_RETRIES        = 5U;
inline constexpr uint8_t  POLL_RETRY_DELAY_MS = 3U;

inline constexpr uint16_t DEFAULT_REGISTER_VALUE = 0x8000;
// ==================================================================================

/**
 * Serializes access to the ADBMS SPI bus so the concurrent ADBMS tasks (voltages, configs, aux) can't interleave
 * transactions. Recursive because readRegGroup() re-enters sendCmd() via the RSTCC recovery path.
 */
SemaphoreHandle_t spiMutex();

class SpiLock
{
    SemaphoreHandle_t m;

  public:
    SpiLock() : m(spiMutex()) { xSemaphoreTakeRecursive(m, portMAX_DELAY); }
    ~SpiLock() { xSemaphoreGiveRecursive(m); }

    SpiLock(const SpiLock &)            = delete;
    SpiLock &operator=(const SpiLock &) = delete;
    SpiLock(SpiLock &&)                 = delete;
    SpiLock &operator=(SpiLock &&)      = delete;
};

// consteval (implicitly inline) PEC lookup-table generator shared by the wire-format classes below.
consteval std::array<uint16_t, 256> pecTable(const uint16_t poly, const uint16_t size)
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

template <std::integral T> T swapEndianness(const T value)
{
    T out = 0;
    for (size_t i = 0; i < sizeof(T); ++i)
    {
        out = static_cast<T>(out << 8);
        out |= static_cast<T>(value >> (i * 8) & 0xFFU);
    }
    return out;
}

/**
 * This represents the wire representation of a register group read from the chip, which consists of the raw bytes
 * followed by the PEC10.
 */
class __attribute__((packed)) RegGroupPayload
{
    static constexpr uint16_t             CRC10_POLY = 0x8F;
    static constexpr array<uint16_t, 256> pec10Table = pecTable(CRC10_POLY, 10);
    RegBuffer                             data;
    // when the data is in here, it is in wire order (big endian). Please make sure to swap endianness on read or write
    uint16_t pec10_cmd_count;

    /**
     * @return This will return a 10 bit number
     */
    [[nodiscard]] uint16_t calculatePec10() const
    {
        uint16_t remainder = 16U;

        for (const uint8_t i : data)
        {
            const uint16_t address = (remainder >> 2 ^ i) & 0xFFU;
            remainder              = static_cast<uint16_t>(static_cast<uint16_t>(remainder << 8) ^ pec10Table[address]);
            remainder &= 0x03FFU; // pec10 keeps only 10 bits
        }

        remainder ^= static_cast<uint16_t>(cmd_count() << 4);
        for (int bit = 0; bit < 6; bit++)
        {
            if (remainder & 0x0200) // check top bit
                remainder = static_cast<uint16_t>((remainder << 1U) ^ CRC10_POLY);
            else
                remainder = static_cast<uint16_t>(remainder << 1U);
            remainder &= 0x03FF; // ensure remainder stays 10 bits
        }
        return remainder;
    }

  public:
    // constructor for building reggrouppayloads when sending
    explicit RegGroupPayload(const array<uint8_t, REG_GROUP_SIZE> &_data)
      : data(_data), pec10_cmd_count(swapEndianness(calculatePec10()))
    {
    }
    // default for recieving
    RegGroupPayload() : data{}, pec10_cmd_count(0) {}

    [[nodiscard]] bool checksum() const { return calculatePec10() == pec10(); }

    [[nodiscard]] uint16_t pec10() const
    {
        // pec10 is stored in the lower 10 bits of the pec10_cmd_count field
        return static_cast<uint16_t>(swapEndianness(pec10_cmd_count) & 0x03FFU);
    }
    [[nodiscard]] uint8_t cmd_count() const
    {
        // cmd_count is stored in the upper 6 bits of the pec10_cmd_count field
        return static_cast<uint8_t>(swapEndianness(pec10_cmd_count) >> 10U);
    }
    [[nodiscard]] const RegBuffer &getData() const { return data; }
};
static_assert(sizeof(RegGroupPayload) == REG_GROUP_SIZE + 2); // 2 bytes for PEC10 and cmd_count

class __attribute__((packed)) Cmd
{
    static constexpr uint16_t             CRC15_POLY = 0x4599;
    static constexpr array<uint16_t, 256> pec15Table = pecTable(CRC15_POLY, 15);

    // note that both are stored in big endian on the wire, so swap endianness when reading and writing
    uint16_t cmd;
    uint16_t pec15;

    [[nodiscard]] uint16_t calculatePec15() const
    {
        uint16_t remainder = 16U;

        for (const uint8_t i :
             { static_cast<uint8_t>(cmd), static_cast<uint8_t>(cmd >> 8) }) // note that the endianness is swapped
        {
            const uint16_t address = ((remainder >> 7) ^ i) & 0xFFU;
            remainder              = static_cast<uint16_t>(static_cast<uint16_t>(remainder << 8) ^ pec15Table[address]);
        }
        return static_cast<uint16_t>(remainder << 1U);
    }

  public:
    explicit Cmd(const uint16_t _cmd)
    {
        cmd   = swapEndianness(_cmd);
        pec15 = swapEndianness(calculatePec15());
    }
    [[nodiscard]] span<const uint8_t> into_span() const
    {
        return { reinterpret_cast<const uint8_t *>(this), sizeof(Cmd) };
    }
};
static_assert(sizeof(Cmd) == 4); // 2 bytes for cmd and 2 bytes for PEC15

struct __attribute__((packed)) WriteCmd
{
    Cmd                       tx_cmd;
    Segments<RegGroupPayload> payload;

    explicit WriteCmd(const uint16_t cmd, const Segments<RegBuffer> regs) : tx_cmd(cmd)
    {
        for (size_t segment = 0U; segment < NUM_SEGMENTS; ++segment)
        {
            payload[segment] = RegGroupPayload{ regs[segment] };
        }
    }

    [[nodiscard]] span<uint8_t> into_span() { return { reinterpret_cast<uint8_t *>(this), sizeof(WriteCmd) }; }
};

} // namespace io::adbms
