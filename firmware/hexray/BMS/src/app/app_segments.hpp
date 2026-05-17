#pragma once
#include <expected>

#include "util_errorCodes.hpp"
#include "io_adbms.hpp"

inline constexpr uint8_t VOLT_CONV_TIME_MS      = 2U;  // check later
inline constexpr uint8_t AUX_CONV_TIME_MS       = 10U; // check later
inline constexpr uint8_t OWC_CONVERSION_TIME_MS = 8U;  // check later

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

// app_segments_config.hpp
namespace config
{
    void setBalanceConfig(const Cells<bool> &balance_config, const Cells<uint8_t> &pwm_duty, bool balancing_enabled);
    std::expected<void, ErrorCode> setThermistorConfig(ThermistorMux mux);
    std::expected<void, ErrorCode> configSync();
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
    void                                   cellVoltages(const Cells<std::expected<float, ErrorCode>> &voltages);
    void                                   temps(const Therms<std::expected<float, ErrorCode>> &temps);
    void                                   segVoltages(const Segments<std::expected<float, ErrorCode>> &seg_voltages);
    void                                   status(const Status &status);
    void                                   owc(const Cells<std::expected<bool, ErrorCode>> &owc_results);
    Cells<std::expected<float, ErrorCode>> getLatestVoltages();
    CellParam<float>                       getMinCellVoltage();
    CellParam<float>                       getMaxCellVoltage()
} // namespace broadcast

// app_segments_health.cpp
namespace health
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
} // namespace health

// aapp_segments_faults.cpp
namespace faults
{
    void init();
    bool checkWarnings();
    bool checkFaults();
} // namespace faults

// app_segments_conversions.cpp
// note that the first expected wrap tells you if the request for the command succeeded
// subseqeunt errors are
std::expected<Cells<std::expected<float, ErrorCode>>, ErrorCode>    runVoltageConversion();
std::expected<Therms<std::expected<float, ErrorCode>>, ErrorCode>   runTempConversion();
std::expected<Segments<std::expected<float, ErrorCode>>, ErrorCode> runSegVoltageConversion();
std::expected<Segments<io::adbms::StatusGroups>, ErrorCode>         runStatusConversion();
std::expected<Cells<std::expected<bool, ErrorCode>>, ErrorCode>     runCellOpenWireCheck();
} // namespace app::segments
