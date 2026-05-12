#pragma once
#include <expected>

#include "util_errorCodes.hpp"
#include "io_adbms.hpp"

namespace app::segments
{
// Thermistor bank selected during AUX conversions.
enum class ThermistorMux : size_t
{
    THERMISTOR_MUX_0_7,
    THERMISTOR_MUX_8_13,
    THERMISTOR_MUX_COUNT,
};

// app_segments_config.hpp
namespace config
{
    void                           setBalanceConfig(const Cells<bool> &balance_config, bool balancing_enabled);
    void                           setPwmConfig(const Cells<uint8_t> &pwm_duty);
    void                           setThermistorConfig(ThermistorMux mux);
    std::expected<void, ErrorCode> configSync();
    std::expected<void, ErrorCode> upload();
}; // namespace config

// app_segments_balancing.cpp
namespace balancing
{
    void init();
    void enable();
    void disable();
} // namespace balancing

// app_segments_broadcast.cpp
namespace broadcast
{
    void cellVoltages(const Cells<float> &voltages, const CellSuccess &voltages_success);
    void filteredCellVoltages(const Cells<float> &voltages, const CellSuccess &voltages_success);
    void aux(const Therms<float> &temps, const ThermSuccess &temps_success);
    void temps(const Therms<float> &temps, const ThermSuccess &temps_success);
    void segVoltages(const Segments<float> &seg_voltages, const SegmentSuccess &seg_voltages_success);
    void status(const Status &status, const SegmentSuccess &status_success);
    void owc(const Owc &owc_ok, const CellSuccess &owc_ok_success);
    void info();
} // namespace broadcast

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
std::expected<Cells<std::expected<float, ErrorCode>>, ErrorCode>                      runVoltageConversion();
std::expected<Cells<std::expected<float, ErrorCode>>, ErrorCode>                      runFilteredVoltageConversion();
std::expected<Therms<std::expected<float, ErrorCode>>, ErrorCode>                     runTempConversion();
std::expected<Segments<std::expected<float, ErrorCode>>, ErrorCode>                   runSegVoltageConversion();
std::expected<Segments<std::expected<io::adbms::StatusGroups, ErrorCode>>, ErrorCode> runStatusConversion();
std::expected<Cells<std::expected<bool, ErrorCode>>, ErrorCode>                       runCellOpenWireCheck();
} // namespace app::segments
