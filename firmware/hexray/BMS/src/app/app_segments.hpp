#pragma once
#include <expected>
#include <utility>

#include "util_errorCodes.hpp"
#include "io_adbms.hpp"

// Minimum conversion times
inline constexpr uint8_t VOLT_CONV_TIME_MS      = 1U;
inline constexpr uint8_t AUX_CONV_TIME_MS       = 1U;
inline constexpr uint8_t OWC_CONVERSION_TIME_MS = 8U; // includes OW soak time, not just ADC

namespace app::segments
{
// Re-export ADBMS array aliases inside this namespace so app code can use them unqualified.
using io::adbms::Cells;
using io::adbms::CellResult;
using io::adbms::SegmentResult;
using io::adbms::ThermResult;
using io::adbms::Segments;
using io::adbms::Status;
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

// app_segments_config.hpp
namespace config
{
    void setBalanceConfig(const Cells<bool> &balance_config, const Cells<uint8_t> &pwm_duty, bool balancing_enabled);
    result<void> setThermistorConfig(ThermistorMux mux);
    result<void> configSync();
}; // namespace config

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
    void temps(const Therms<result<float>> &temps, const Therms<result<bool>> &therm_owc);
    void segVoltages(const Segments<result<float>> &seg_voltages);
    void status(const Status &status);
    void owc(const Cells<result<bool>> &owc_results);
} // namespace broadcast

// app_segments_state.cpp
namespace state
{
    enum class Bit : size_t
    {
        Voltage = 0,
        Temp    = 1,
        Status  = 2,
        Config  = 3
    };
    void reset(size_t seg, Bit bit);
    void resetAll(Bit bit);
    void set(size_t seg, Bit bit);
    void setAll(Bit bit);
    bool isOk(size_t seg);
    bool allOk();

    Cells<result<float>> getLatestVoltages();
    CellParam<float>     getMinCellVoltage();
    CellParam<float>     getMaxCellVoltage();
    CellParam<float>     getMaxCellTemperature();
    bool                 getCellOwc();
    bool                 getThermOwc();

    void setVoltageStats(const Cells<result<float>> &latest, CellParam<float> min, CellParam<float> max);
    void setTempStats(CellParam<float> max_temp, bool any_therm_owc);
    void setCellOwc(bool any_cell_owc);
} // namespace state

// aapp_segments_faults.cpp
namespace faults
{
    bool checkWarnings();
    bool checkFaults();
} // namespace faults

// app_segments_conversions.cpp
result<CellResult<float>>                             runVoltageConversion();
result<std::pair<ThermResult<float>, ThermResult<bool>>> runTempConversion();
result<SegmentResult<float>>                          runSegVoltageConversion();
result<Segments<io::adbms::StatusGroups>>          runStatusConversion();
result<CellResult<bool>>                              runCellOpenWireCheck();
} // namespace app::segments
