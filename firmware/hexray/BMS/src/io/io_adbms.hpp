/**
 * @file io_adbms.hpp
 * @brief Public interface for the Hexray BMS ADBMS driver.
 */
#pragma once

#include "util_errorCodes.hpp"

#include <array>
#include <cstdint>
#include <expected>

// Physical daisy-chain dimensions for this ECU.
inline constexpr uint8_t NUM_SEGMENTS            = 1;
inline constexpr uint8_t CELLS_PER_SEGMENT       = 14;
inline constexpr uint8_t THERMISTORS_PER_SEGMENT = 14;
inline constexpr uint8_t THERM_GPIOS_PER_SEGMENT = 8;

namespace io::adbms
{

enum class OpenWireSwitch
{
    OddChannels,
    EvenChannels
};

// Base array types. Kept inside io::adbms so generic names don't collide at global scope.
// Represents the bytes of a register group
using RegBuffer                               = std::array<uint8_t, REG_GROUP_SIZE>;
template <typename T> using Segments          = std::array<T, NUM_SEGMENTS>;
template <typename T> using SegmentCells      = std::array<T, CELLS_PER_SEGMENT>;
template <typename T> using SegmentTherms     = std::array<T, THERMISTORS_PER_SEGMENT>;
template <typename T> using SegmentThermGpios = std::array<T, THERM_GPIOS_PER_SEGMENT>;
template <typename T> using Cells             = Segments<SegmentCells<T>>;
template <typename T> using Therms            = Segments<SegmentTherms<T>>;
template <typename T> using ThermGpios        = Segments<SegmentThermGpios<T>>;

// move ts
/**
 * @return SegmentsResult<SegmentConfig> containing a result for each segment. If an error is raised in either the CFGA
 * or CFGB read for a segment, the SegmentConfig result for that segment will be an unexpected containing the error
 * code. If both reads succeed, the SegmentConfig result will contain the successfully read and parsed configuration.
 */

namespace write
{
    [[nodiscard]] result<void> pwmReg(const Segments<PWMConfig> &pwm_config);
    [[nodiscard]] result<void> configReg(const Segments<SegmentConfig> &config);
} // namespace write

// Measurement reads.
namespace read
{
    [[nodiscard]] Segments<result<SegmentConfig>> configReg();
    [[nodiscard]] Segments<result<PWMConfig>>     pwmReg();
    [[nodiscard]] Cells<result<uint16_t>>         cellVoltage();
    [[nodiscard]] Cells<result<uint16_t>>         filteredCellVoltage();
    [[nodiscard]] Segments<result<uint16_t>>      segVoltage();
    [[nodiscard]] ThermGpios<result<uint16_t>>    thermGpioVoltage();
    [[nodiscard]] Segments<StatusGroups>          status();
} // namespace read

// Conversion control.
namespace command
{
    [[nodiscard]] result<void> startCellsAdc();
    [[nodiscard]] result<void> startAuxAdc();
    [[nodiscard]] result<void> pollCellsAdc();
    [[nodiscard]] result<void> pollAuxAdc();
    [[nodiscard]] result<void> startBalance();
    [[nodiscard]] result<void> stopBalance();
    [[nodiscard]] result<void> wakeup();
    [[nodiscard]] result<void> owcCells(OpenWireSwitch owcSwitch);
} // namespace command

// Register clear helpers.
namespace clear
{
    [[nodiscard]] result<void> aux();
    [[nodiscard]] result<void> cell();
    [[nodiscard]] result<void> filteredCell();
    [[nodiscard]] result<void> stat();
} // namespace clear
} // namespace io::adbms
