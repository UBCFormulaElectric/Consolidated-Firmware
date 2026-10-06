#pragma once
#include <array>
#include <bitset>
#include <expected>
#include <utility>

#include "util_errorCodes.hpp"
#include "app_pack.hpp"
#include "io_adbms.hpp"
#include "io_semaphore.hpp"

inline constexpr uint8_t MAX_NUM_SEGMENTS = 10U;

inline constexpr uint8_t CELL_CONV_TIME_MS           = 2U;
inline constexpr uint8_t SECONDARY_CELL_CONV_TIME_MS = 8U;
inline constexpr uint8_t AUX_CONV_TIME_MS            = 18U;

extern io::semaphore spi_bus_lock;
extern io::semaphore shared_lock;

inline constexpr uint32_t SYSVIEW_MARKER_CONFIG_LOCK = 3;
inline constexpr uint32_t SYSVIEW_MARKER_SHARED_LOCK = 2;
namespace app::segments
{
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
    void setBalanceConfig(
        const io::adbms::Cells<bool>    &balance_config,
        const io::adbms::Cells<uint8_t> &pwm_duty,
        bool                             balancing_enabled);
    void setThermistorConfig(ThermistorMux mux);
    /**
     * Synchronizes the in-memory config with the ADBMS chips.
     */
    [[nodiscard]] io::adbms::Segments<result<bool>> sync();
} // namespace config

// app_segments_health.cpp
namespace health
{
    enum class ErrorBit : size_t
    {
        CELL_ADC_START,
        AUX_ADC_START,
        AUX_ADC_POLL,
        OWC_ADC_START,
        CELL_VOLTAGE,
        CELL_OWC_VOLTAGE,
        THERM_VOLTAGE,
        SEG_VOLTAGE,
        STATUS,
        CONFIG,
        UNREACHABLE,
        NUM_ERROR_BITS
    };

    inline constexpr size_t NUM_HEALTH_BITS = static_cast<size_t>(ErrorBit::NUM_ERROR_BITS);

    using Snapshot = std::array<std::bitset<NUM_HEALTH_BITS>, MAX_NUM_SEGMENTS>;

    void     setOrReset(size_t seg, ErrorBit bit, bool has_error);
    void     setOrResetAll(ErrorBit bit, bool has_error);
    bool     getAnyError(size_t seg);
    Snapshot getAll();
} // namespace health

// app_segments_shared.cpp
namespace shared
{
    io::adbms::Cells<result<float>> getLatestVoltages();
    CellParam<float>                getMinCellVoltage();
    CellParam<float>                getMaxCellVoltage();
    CellParam<float>                getMinCellTemperature();
    CellParam<float>                getMaxCellTemperature();
    io::adbms::Cells<result<bool>>  getLatestCellOwcOk();
    io::adbms::Therms<result<bool>> getLatestThermOwcOk();
    SegmentParam<float>             getMinSegmentVoltage();
    SegmentParam<float>             getMaxSegmentVoltage();
    result<float>                   getPackVoltage();

    void setCellOwcOk(const io::adbms::Cells<result<bool>> &owc);
    void setVoltageStats(const io::adbms::Cells<result<float>> &latest);
    void setThermistorOwcOk(const io::adbms::Therms<result<bool>> &owc);
    void setTemperatureStats(const io::adbms::Therms<result<float>> &latest);
    void setSegmentVoltageStats(const io::adbms::Segments<result<float>> &latest);
} // namespace shared

// app_segments_alerts.cpp
namespace alerts
{
    void init();
    bool tick();
} // namespace alerts

namespace conversion
{
    io::adbms::Cells<result<float>>                 cellVoltage();
    io::adbms::ThermGpios<result<float>>            thermVoltage();
    io::adbms::Segments<result<float>>              segVoltage();
    io::adbms::Segments<io::adbms::StatusGroupsRes> status();
    io::adbms::Cells<result<float>>                 cellOwcVoltages();
} // namespace conversion

// app_segments_calculation.cpp
namespace calculate
{
    io::adbms::Cells<result<bool>> cellOwcOk(
        const std::array<io::adbms::Cells<result<float>>, static_cast<size_t>(io::adbms::OpenWireSwitch::CHANNEL_COUNT)>
            &owc_voltages);
    io::adbms::Therms<result<float>> thermTemps(
        const std::array<io::adbms::ThermGpios<result<float>>, static_cast<size_t>(ThermistorMux::THERMISTOR_MUX_COUNT)>
            &therm_voltages);
    io::adbms::Therms<result<bool>> thermOwcOk(
        const std::array<io::adbms::ThermGpios<result<float>>, static_cast<size_t>(ThermistorMux::THERMISTOR_MUX_COUNT)>
            &therm_voltages);
} // namespace calculate
} // namespace app::segments
