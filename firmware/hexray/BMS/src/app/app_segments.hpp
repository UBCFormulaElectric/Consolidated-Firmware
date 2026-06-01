#pragma once
#include <array>
#include <expected>
#include <utility>

#include "util_errorCodes.hpp"
#include "io_adbms.hpp"
#include "io_semaphore.hpp"

// Minimum conversion times
inline constexpr uint8_t CELL_CONV_TIME_MS           = 2U; 
inline constexpr uint8_t SECONDARY_CELL_CONV_TIME_MS = 8U; 
inline constexpr uint8_t AUX_CONV_TIME_MS            = 18U;

extern io::semaphore spi_bus_lock;
extern io::semaphore health_lock;

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

// Identifies one segment and its latest measured values.
template <typename T> struct SegmentParam
{
    uint8_t segment;
    T       value;
    auto    operator<=>(const SegmentParam &other) const { return value <=> other.value; };
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
    namespace debug
    {
        void cellVoltages(const Cells<result<float>> &voltages, const result<void> &poll_ok);
        void thermTemps(const Therms<result<float>> &temps, const result<void> &poll_ok);
        void thermOwc(const Therms<result<bool>> &therm_owc, const result<void> &poll_ok);
        void segVoltages(const Segments<result<float>> &seg_voltages);
        void status(const Segments<io::adbms::StatusGroupsRes> &status);
        void cellOwc(const Cells<result<bool>> &owc_results, const result<void> &poll_ok);
    } // namespace debug
    void segmentHealthError();
    void voltageStats();
    void temperatureStats();
    void segmentVoltageStats();
    void cmdCountMismatch();
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

    void resetAll(ErrorBit bit);
    void setAll(ErrorBit bit);
    void setOrReset(size_t seg, ErrorBit bit, bool has_error);
    bool getError(size_t seg, ErrorBit bit);
    bool getAnyError(size_t seg);
} // namespace health

// app_segments_shared.cpp
namespace shared
{
    Cells<result<float>>    getLatestVoltages();
    CellParam<float>        getMinCellVoltage();
    CellParam<float>        getMaxCellVoltage();
    Therms<result<float>>   getLatestTemperatures();
    CellParam<float>        getMinCellTemperature();
    CellParam<float>        getMaxCellTemperature();
    Cells<result<bool>>     getLatestCellOwc();
    Therms<result<bool>>    getLatestThermOwc();
    Segments<result<float>> getLatestSegmentVoltages();
    SegmentParam<float>     getMinSegmentVoltage();
    SegmentParam<float>     getMaxSegmentVoltage();

    void setVoltageStats(const Cells<result<float>> latest, const Cells<result<bool>> owc);
    void setTemperatureStats(const Therms<result<float>> latest, const Therms<result<bool>> owc);
    void setSegmentVoltageStats(const Segments<result<float>> latest);
} // namespace shared

// app_segments_alerts.cpp
namespace alerts
{
    void init();
    // Evaluates all warning/fault/info conditions through their debounce timers and
    // calls the matching app::can_alerts setters. Returns true iff at least one
    // fault entry is currently firing — drive bms_ok_latch off this directly.
    bool tick();
} // namespace alerts

// app_segments_conversions.cpp
namespace startPoll
{
    [[nodiscard]] result<void> secondaryCellAdc(io::adbms::OpenWireSwitch owcSwitch);
    [[nodiscard]] result<void> cellAdc();
    [[nodiscard]] result<void> auxAdc();
} // namespace startPoll

namespace conversion
{
    Cells<result<float>>                 cellVoltage();
    ThermGpios<result<float>>            thermVoltage();
    Segments<result<float>>              segVoltage();
    Segments<io::adbms::StatusGroupsRes> status();
    Cells<result<float>>                 cellOwcVoltages();
} // namespace conversion

// app_segments_calculation.cpp
namespace calculate
{
    Cells<result<bool>> cellOwc(
        const std::array<Cells<result<float>>, static_cast<size_t>(io::adbms::OpenWireSwitch::CHANNEL_COUNT)>
            &owc_voltages);
    Therms<result<float>> thermTemps(
        const std::array<ThermGpios<result<float>>, static_cast<size_t>(ThermistorMux::THERMISTOR_MUX_COUNT)>
            &therm_voltages);
    Therms<result<bool>> thermOwc(
        const std::array<ThermGpios<result<float>>, static_cast<size_t>(ThermistorMux::THERMISTOR_MUX_COUNT)>
            &therm_voltages);
} // namespace calculate
} // namespace app::segments
