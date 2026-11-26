/**
 * @file io_emeter_internal.hpp
 * @brief DS18B20 wire format: command bytes, scratchpad layout and the Maxim CRC8.
 *
 * Same role as io_adbms_internal.hpp -- everything here is a direct transcription of the datasheet
 * and has no business being visible outside the driver.
 */
#pragma once

#include "io_emeter.hpp"

#include <array>
#include <cstdint>
#include <span>

namespace io::emeter
{
// --- ROM commands (pg. 10-11) ------------------------------------------------------------------
inline constexpr uint8_t SEARCH_ROM   = 0xF0U;
inline constexpr uint8_t READ_ROM     = 0x33U; // single-drop only; collides with >1 device
inline constexpr uint8_t MATCH_ROM    = 0x55U;
inline constexpr uint8_t SKIP_ROM     = 0xCCU;
inline constexpr uint8_t ALARM_SEARCH = 0xECU;

// --- Function commands (Table 3) ---------------------------------------------------------------
inline constexpr uint8_t CONVERT_T         = 0x44U;
inline constexpr uint8_t WRITE_SCRATCHPAD  = 0x4EU;
inline constexpr uint8_t READ_SCRATCHPAD   = 0xBEU;
inline constexpr uint8_t RECALL_E2         = 0xB8U;
inline constexpr uint8_t READ_POWER_SUPPLY = 0xB4U;
// COPY_SCRATCHPAD [0x48] is intentionally absent -- see writeConfigAll() in io_emeter.hpp.

/** 1-Wire family code every DS18B20 reports in ROM byte 0 (pg. 8). */
inline constexpr uint8_t FAMILY_CODE = 0x28U;

/**
 * Power-on value of the temperature register: +85 degC (Table 1). A device holds this until its
 * first conversion completes, so reading it back means "no valid sample yet". This is the DS18B20
 * analogue of DEFAULT_REGISTER_VALUE (0x8000) in the ADBMS driver.
 */
inline constexpr int16_t POWER_ON_RESET_TEMP = 0x0550;

/** Scratchpad bytes 5 and 7 always read back as these fixed values (Figure 9). */
inline constexpr uint8_t RESERVED_BYTE_5 = 0xFFU;
inline constexpr uint8_t RESERVED_BYTE_7 = 0x10U;

/** We do not use Alarm Search, so TH/TL are free user bytes. Write them deterministically. */
inline constexpr uint8_t TH_UNUSED = 0x00U;
inline constexpr uint8_t TL_UNUSED = 0x00U;

/**
 * Configuration register (Figure 10): bit 7 is 0, bits 4..0 are 1, R1:R0 sit at bits 6:5.
 * Resolution's enumerators are already the R1:R0 pair, so this is just a shift.
 */
[[nodiscard]] constexpr uint8_t configByte(const Resolution res)
{
    return static_cast<uint8_t>(0x1FU | static_cast<uint8_t>(static_cast<uint8_t>(res) << 5U));
}
static_assert(configByte(Resolution::BITS_9) == 0x1FU);
static_assert(configByte(Resolution::BITS_10) == 0x3FU);
static_assert(configByte(Resolution::BITS_11) == 0x5FU);
static_assert(configByte(Resolution::BITS_12) == 0x7FU);

/** Inverse of configByte(): recovers the resolution a sample was actually taken at. */
[[nodiscard]] constexpr Resolution resolutionOf(const uint8_t config)
{
    return static_cast<Resolution>(config >> 5U & 0x03U);
}
static_assert(resolutionOf(configByte(Resolution::BITS_9)) == Resolution::BITS_9);
static_assert(resolutionOf(configByte(Resolution::BITS_12)) == Resolution::BITS_12);

/**
 * Below 12-bit resolution the low bits of the temperature register are undefined and hold stale
 * data (pg. 6): bit 0 at 11-bit, bits 1:0 at 10-bit, bits 2:0 at 9-bit. Clear them so a caller
 * cannot mistake leftovers for signal.
 */
[[nodiscard]] constexpr int16_t maskUndefinedBits(const int16_t raw, const Resolution res)
{
    constexpr std::array<uint16_t, 4> masks{ { 0xFFF8U, 0xFFFCU, 0xFFFEU, 0xFFFFU } };
    return static_cast<int16_t>(static_cast<uint16_t>(raw) & masks[static_cast<size_t>(res)]);
}
static_assert(maskUndefinedBits(0x0557, Resolution::BITS_9) == 0x0550);
static_assert(maskUndefinedBits(static_cast<int16_t>(0xFF5F), Resolution::BITS_9) ==
              static_cast<int16_t>(0xFF58)); // stays negative

/**
 * Wire layout of the nine scratchpad bytes (Figure 9). The DS18B20 transmits starting at the least
 * significant bit of byte 0, so this maps 1:1 onto the receive buffer with no reordering -- unlike
 * the ADBMS register groups, there is no endianness swap to do.
 */
struct __attribute__((packed)) Scratchpad
{
    uint8_t temp_lsb;
    uint8_t temp_msb;
    uint8_t th;
    uint8_t tl;
    uint8_t config;
    uint8_t reserved_5;
    uint8_t reserved_6;
    uint8_t reserved_7;
    uint8_t crc;

    /** Temperature register: 16-bit sign-extended two's complement (Figure 4). */
    [[nodiscard]] int16_t rawTemp() const
    {
        return static_cast<int16_t>(static_cast<uint16_t>(temp_lsb) | static_cast<uint16_t>(temp_msb) << 8U);
    }

    [[nodiscard]] std::span<const uint8_t> bytes() const
    {
        return { reinterpret_cast<const uint8_t *>(this), sizeof(Scratchpad) };
    }
};
static_assert(sizeof(Scratchpad) == 9);

/**
 * Maxim CRC8 table, X^8 + X^5 + X^4 + 1 (pg. 9). 1-Wire is least-significant-bit-first, so this is
 * the reflected form of the 0x31 polynomial, 0x8C. Built at compile time the same way the ADBMS
 * PEC tables are in io_adbms_internal.cpp.
 */
consteval std::array<uint8_t, 256> crc8Table()
{
    constexpr uint8_t REFLECTED_POLY = 0x8CU;

    std::array<uint8_t, 256> out{};
    for (uint16_t i = 0U; i < 256U; ++i)
    {
        auto crc = static_cast<uint8_t>(i);
        for (uint8_t bit = 0U; bit < 8U; ++bit)
        {
            crc = crc & 1U ? static_cast<uint8_t>(crc >> 1U ^ REFLECTED_POLY) : static_cast<uint8_t>(crc >> 1U);
        }
        out[i] = crc;
    }
    return out;
}
inline constexpr std::array<uint8_t, 256> CRC8_TABLE = crc8Table();

/**
 * @param bytes the message, with the device's own CRC byte appended
 * @return 0 when the message is intact.
 *
 * Running the stored CRC back through the generator leaves the register at zero, exactly as the
 * datasheet describes (pg. 9: "if the re-calculated CRC was correct, the shift register will
 * contain all 0s"). So callers check `crc8(...) == 0` over the whole message rather than
 * recomputing and comparing.
 */
[[nodiscard]] constexpr uint8_t crc8(const std::span<const uint8_t> bytes)
{
    uint8_t crc = 0U;
    for (const uint8_t byte : bytes)
    {
        crc = CRC8_TABLE[static_cast<uint8_t>(crc ^ byte)];
    }
    return crc;
}

// Maxim Application Note 27's worked example, as a build-time regression test.
static_assert(crc8(std::array<uint8_t, 7>{ { 0x02, 0x1C, 0xB8, 0x01, 0x00, 0x00, 0x00 } }) == 0xA2U);
static_assert(crc8(std::array<uint8_t, 8>{ { 0x02, 0x1C, 0xB8, 0x01, 0x00, 0x00, 0x00, 0xA2 } }) == 0U);
// A bus stuck low reads as all zeros, and all zeros is CRC-clean. The CRC cannot catch this on its
// own, which is why readOne() also checks the reserved bytes.
static_assert(crc8(std::array<uint8_t, 9>{}) == 0U);

} // namespace io::emeter
