#pragma once
#include <array>
#include <expected>
#include <utility>

#include "util_errorCodes.hpp"
#include "io_adbms.hpp"
#include "hw_notify.hpp"

// Minimum conversion times
inline constexpr uint8_t CELL_CONV_TIME_MS           = 1U;
inline constexpr uint8_t SECONDARY_CELL_CONV_TIME_MS = 8U;
inline constexpr uint8_t AUX_CONV_TIME_MS            = 1U;

namespace app::segments
{
// Re-export ADBMS array aliases inside this namespace so app code can use them unqualified.
using io::adbms::Cells;
using io::adbms::Segments;
using io::adbms::ThermGpios;
using io::adbms::Therms;

// Thermistor bank selected during AUX conversions.
enum class ThermistorMux : size_t
{
    THERMISTOR_MUX_0_7,
    THERMISTOR_MUX_8_13,
    THERMISTOR_MUX_COUNT,
};

// Identifies one cell and its latest measured values.
template <typename T> struct CellParam
{
    uint8_t segment;
    uint8_t cell;
    T       value;
    auto    operator<=>(const CellParam &other) const { return value <=> other.value; };
};

// app_segments_config.cpp
namespace config
{
    void setBalanceConfig(const Cells<bool> &balance_config, const Cells<uint8_t> &pwm_duty, bool balancing_enabled);
    void setThermistorConfig(ThermistorMux mux);
    /**
     * Synchronizes the in-memory config with the ADBMS chips.
     */
    [[nodiscard]] Segments<result<bool>> sync();
} // namespace config

// app_segments_balancing.cpp
namespace balancing
{
    void init();
    void tick();
    void disable();
} // namespace balancing

// app_segments_broadcast.cpp
namespace broadcast
{
    void cellVoltages(const Cells<result<float>> &voltages);
    void thermTemps(const Therms<result<float>> &temps);
    void thermOwc(const Therms<result<bool>> &therm_owc);
    void segVoltages(const Segments<result<float>> &seg_voltages);
    void status(const Segments<io::adbms::StatusGroupsRes> &status);
    void cellOwc(const Cells<result<bool>> &owc_results);
    void cellVoltagesPollErr();
    void cellOwcPollErr();
    void thermTempsPollErr();
    void thermOwcPollErr();
} // namespace broadcast

// app_segments_health.cpp
namespace health
{
    enum class ErrorBit : size_t
    {
        CELL_ADC_POLL,
        AUX_ADC_POLL,
        OWC_ADC_POLL,
        CELL_VOLTAGE,
        CELL_OWC_VOLTAGE,
        THERM_VOLTAGE,
        SEG_VOLTAGE,
        STATUS,
        CONFIG,
        NUM_ERROR_BITS
    };

    void reset(size_t seg, ErrorBit bit);
    void resetAll(ErrorBit bit);
    void set(size_t seg, ErrorBit bit);
    void setAll(ErrorBit bit);
    void setOrReset(size_t seg, ErrorBit bit, bool has_error);
    bool isOk(size_t seg);
    bool allOk();

    // Cells<result<float>> getLatestVoltages();
    // CellParam<float>     getMinCellVoltage();
    // CellParam<float>     getMaxCellVoltage();
    // CellParam<float>     getMaxCellTemperature();
    // bool                 getCellOwc();
    // bool                 getThermOwc();
    // void setVoltageStats(const Cells<result<float>> &latest, CellParam<float> min, CellParam<float> max);
    // void setMaxCellTemperature(CellParam<float> max_temp);
    // void setThermOwc(bool any_therm_owc);
    // void setCellOwc(bool any_cell_owc);
} // namespace health

// app_segments_faults.cpp
// namespace faults
// {
//     bool checkWarnings();
//     bool checkFaults();
// } // namespace faults

// app_segments_conversions.cpp
namespace startPoll
{
    [[nodiscard]] result<void> secondaryCellAdc(io::adbms::OpenWireSwitch owcSwitch);
    [[nodiscard]] result<void> cellAdc();
    [[nodiscard]] result<void> auxAdc(ThermistorMux mux);
} // namespace startPoll

namespace conversion
{
    Cells<result<float>>                 cellVoltage();
    ThermGpios<result<float>>            thermVoltage(ThermistorMux mux);
    Segments<result<float>>              segVoltage();
    Segments<io::adbms::StatusGroupsRes> status();
    Cells<result<float>>                 cellOwcVoltages(io::adbms::OpenWireSwitch channel);
} // namespace conversion

// app_segments_calculation.cpp
namespace calculate
{
    Cells<result<bool>> cellOwc(
        const std::array<result<Cells<result<float>>>, static_cast<size_t>(io::adbms::OpenWireSwitch::CHANNEL_COUNT)>
            &owc_voltages);
    Therms<result<float>> thermTemps(
        const std::array<result<ThermGpios<result<float>>>, static_cast<size_t>(ThermistorMux::THERMISTOR_MUX_COUNT)>
            &therm_voltages);
    Therms<result<bool>> thermOwc(
        const std::array<result<ThermGpios<result<float>>>, static_cast<size_t>(ThermistorMux::THERMISTOR_MUX_COUNT)>
            &therm_voltages);
} // namespace calculate
} // namespace app::segments
