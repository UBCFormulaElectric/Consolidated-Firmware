/**
 * @file io_emeter.hpp
 * @brief Public interface for the Hexray BMS energy-meter temperature chain.
 *
 * The chain is a set of DS18B20 1-Wire digital thermometers running in parasite power mode (VDD
 * grounded, power stolen from DQ). They are the same sensors the Formula Student energy meter
 * reports over CAN, so app_emeter cross-checks these against the ADBMS thermistors.
 *
 * Layering mirrors the ADBMS driver: hw::onewire moves bits, this layer speaks DS18B20, and
 * app_emeter owns degrees, statistics and CAN.
 *
 * The caller drives the measurement cycle, because the conversion wait is long enough that it
 * belongs at the job level rather than buried in a driver:
 *
 *     writeConfigAll();                              // ~3ms
 *     startConvertAll();                             // ~2ms, leaves the strong pullup on
 *     io::time::delay(conversionTimeMs(RESOLUTION)); // the only hoistable delay in the driver
 *     endConvert();
 *     const auto temps = readRawTemps();             // ~11.6ms per sensor
 */
#pragma once

#include "util_errorCodes.hpp"

#include <array>
#include <cstdint>

// Number of DS18B20s on the accumulator 1-Wire chain. The energy meter's CAN Temperature message
// carries NumSensor as 5 bits, so 32 is the hard ceiling.
inline constexpr uint8_t NUM_EMETER_SENSORS = 20;

namespace io::emeter
{
/** 64-bit lasered ROM code: [0] family code, [1..6] serial number, [7] CRC (Figure 8). */
using RomCode = std::array<uint8_t, 8>;

template <typename T> using Sensors = std::array<T, NUM_EMETER_SENSORS>;

/** Table 2. The values are the R1:R0 bit pair, so they index the config register directly. */
enum class Resolution : uint8_t
{
    BITS_9  = 0b00, // 0.5    degC
    BITS_10 = 0b01, // 0.25   degC
    BITS_11 = 0b10, // 0.125  degC
    BITS_12 = 0b11, // 0.0625 degC
};

/**
 * We run 9-bit deliberately. The energy meter reports temperature over CAN at 0.5 degC granularity
 * (EMeter_tx.json: "degC = raw * 0.5"), so finer resolution buys nothing we can compare against,
 * and it cuts the conversion from 750ms to 93.75ms. That is 8x less time holding the strong pullup
 * with every device on the bus drawing up to 1.5mA at once -- a real reliability win on parasite
 * power, and it keeps a 32-sensor chain comfortably inside a 2s period.
 */
inline constexpr Resolution RESOLUTION = Resolution::BITS_9;

/**
 * Table 2: worst-case conversion time for a resolution. The caller owns this delay -- see the
 * cycle sketch above.
 */
[[nodiscard]] constexpr uint32_t conversionTimeMs(const Resolution res)
{
    // 93.75 / 187.5 / 375 / 750, rounded up to whole ms.
    constexpr std::array<uint32_t, 4> times{ { 94U, 188U, 375U, 750U } };
    return times[static_cast<size_t>(res)];
}

/**
 * The chain in physical order along the accumulator loom. Index here is the index that goes out
 * over CAN, so this order has to match the thermistor map app_segments uses. Defined in
 * io_emeter.cpp.
 */
extern const Sensors<RomCode> sensor_rom_codes;

/**
 * Validates sensor_rom_codes and confirms every device is parasite powered.
 * Also writes the resolution config, so the first cycle after init() is already valid.
 * @return CHECKSUM_FAIL on a bad ROM code, ERROR if a device reports an external VDD,
 *         TIMEOUT if nothing on the bus answers a reset.
 */
[[nodiscard]] result<void> init();

/**
 * Broadcasts TH/TL/config to every device via Skip ROM. Cheap (~3ms) and self-healing, so the job
 * layer can call it every cycle.
 *
 * @note This writes the *scratchpad* only, never EEPROM. Copy Scratchpad [0x48] would cost a
 *       second 10ms strong-pullup window and burn one of the part's 50k write cycles, and we would
 *       gain nothing: the bus idles high, so the devices stay powered and hold their scratchpad.
 */
[[nodiscard]] result<void> writeConfigAll(Resolution res = RESOLUTION);

/**
 * Starts a simultaneous conversion on every device (Skip ROM + Convert T) and turns on the strong
 * pullup, which the devices need to have enough current to convert on parasite power.
 *
 * One Skip ROM converts the whole chain in parallel, so the wait is one conversion time, not N.
 *
 * After this returns the bus is held by the pullup and unusable: wait
 * conversionTimeMs(), then call endConvert(). readRawTemps() reports BUSY until you do.
 */
[[nodiscard]] result<void> startConvertAll();

/** Releases the strong pullup and re-opens the bus for reads. Safe to call when idle. */
void endConvert();

/**
 * Reads every scratchpad and returns the raw, sign-extended temperature registers, with the bits
 * the configured resolution leaves undefined masked off (pg. 6). app_emeter scales to degrees.
 *
 * Per sensor rather than all-or-nothing, so one dead sensor does not blind the other nineteen.
 * @return per sensor: CHECKSUM_FAIL on a bad scratchpad CRC, INVALID_READING if the device has not
 *         completed a conversion since power-up, TIMEOUT if it does not answer, BUSY if a
 *         conversion is still in flight.
 */
[[nodiscard]] Sensors<result<int16_t>> readRawTemps();

/**
 * Bring-up only: reads the ROM code of the one device on the bus (Read ROM [0x33]) and logs it in
 * a form you can paste straight into sensor_rom_codes.
 *
 * Put exactly one sensor on the bus at a time. Read ROM has no addressing, so with several devices
 * connected they all answer simultaneously and you get the wired-AND of their codes, which the CRC
 * check here rejects. Call it from chimera.cpp; it is not part of the measurement cycle.
 */
[[nodiscard]] result<RomCode> readSingleRom();
} // namespace io::emeter
