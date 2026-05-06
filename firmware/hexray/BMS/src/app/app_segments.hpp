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
void balancingEnable();
void balancingDisable();

// Broadcast and fault evaluation.
// Each broadcast function accepts the freshly-converted data so the caller
// can commit it to the shared globals atomically under adbms_app_lock.
using CellVoltageSuccess         = array<array<expected<void, ErrorCode>, io::CELLS_PER_SEGMENT>, io::NUM_SEGMENTS>;
using CellOwcOk                  = array<array<bool, io::CELLS_PER_SEGMENT>, io::NUM_SEGMENTS>;

void broadcastCellVoltages(
    const array<array<float, io::CELLS_PER_SEGMENT>, io::NUM_SEGMENTS>& voltages,
    const CellVoltageSuccess&                                            success);
void broadcastFilteredCellVoltages(
    const array<array<float, io::CELLS_PER_SEGMENT>, io::NUM_SEGMENTS>& voltages,
    const CellVoltageSuccess&                                            success);
void broadcastCellTemps();
void broadcastStatus();
void broadcastCellOpenWireCheck(const CellOwcOk& owc_ok);
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
// Output params receive the results so the caller can hold them on the stack
// until they are committed to the shared globals under adbms_app_lock.
expected<void, ErrorCode> runVoltageConversion(
    array<array<float, io::CELLS_PER_SEGMENT>, io::NUM_SEGMENTS>& out_voltages,
    CellVoltageSuccess&                                            out_success);
expected<void, ErrorCode> runFilteredVoltageConversion(
    array<array<float, io::CELLS_PER_SEGMENT>, io::NUM_SEGMENTS>& out_voltages,
    CellVoltageSuccess&                                            out_success);
expected<void, ErrorCode> runAuxConversion();
expected<void, ErrorCode> runStatusConversion();
expected<void, ErrorCode> runCellOpenWireCheck(CellOwcOk& out_owc_ok);

} // namespace app::segments
