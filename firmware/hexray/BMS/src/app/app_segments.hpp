#pragma once

#include "io_adbms.hpp"

#include <array>
#include <expected>

using namespace std;

namespace app::segments
{
// Thermistor bank selected during AUX conversions.
enum class ThermistorMux
{
    THERMISTOR_MUX_0_7,
    THERMISTOR_MUX_8_13,
    THERMISTOR_MUX_COUNT,
};

// Identifies one cell and its latest measured values.
struct CellParam
{
    uint8_t segment;
    uint8_t cell;
    float   voltage;
    float   temp;
};

// Configuration helpers.
void setDefaultConfig();
void setBalanceConfig(
    array<array<bool, io::CELLS_PER_SEGMENT>, io::NUM_SEGMENTS> &balance_config,
    bool                                                         balancing_enabled);
void                      setPwmConfig(array<array<uint8_t, io::CELLS_PER_SEGMENT>, io::NUM_SEGMENTS> &pwm_duty);
void                      setThermistorConfig(ThermistorMux mux);
expected<void, ErrorCode> configSync();
expected<void, ErrorCode> writeConfig();

// Balancing state machine.
void balancingInit();
void balancingTick(bool enable);

// Broadcast and fault evaluation.
void broadcastCellVoltages();
void broadcastFilteredCellVoltages();
void broadcastCellTemps();
void broadcastStatus();
void broadcastCellOpenWireCheck();
void initFaults();
bool checkWarnings();
bool checkFaults();

// Aggregated pack measurements.
float     getPackVoltage();
CellParam getMaxCellVoltage();
CellParam getMinCellVoltage();
CellParam getMaxCellTemp();
CellParam getMinCellTemp();

// Conversion and diagnostics.
expected<void, ErrorCode> runVoltageConversion();
expected<void, ErrorCode> runFilteredVoltageConversion();
expected<void, ErrorCode> runAuxConversion();
expected<void, ErrorCode> runStatusConversion();
expected<void, ErrorCode> runCellOpenWireCheck();

} // namespace app::segments
