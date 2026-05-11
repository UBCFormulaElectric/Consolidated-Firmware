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

// Broadcast over CAN.
using Cell           = array<array<float, io::CELLS_PER_SEGMENT>, io::NUM_SEGMENTS>;
using Therm          = array<array<float, io::THERMISTORS_PER_SEGMENT>, io::NUM_SEGMENTS>;
using Segment        = array<float, io::NUM_SEGMENTS>;
using Status         = array<io::adbms::StatusGroups, io::NUM_SEGMENTS>;
using Owc            = array<array<bool, io::CELLS_PER_SEGMENT>, io::NUM_SEGMENTS>;
using CellSuccess    = array<array<expected<void, ErrorCode>, io::CELLS_PER_SEGMENT>, io::NUM_SEGMENTS>;
using ThermSuccess   = array<array<expected<void, ErrorCode>, io::THERMISTORS_PER_SEGMENT>, io::NUM_SEGMENTS>;
using SegmentSuccess = array<expected<void, ErrorCode>, io::NUM_SEGMENTS>;

void broadcastCellVoltages(const Cell &voltages, const CellSuccess &voltages_success);
void broadcastFilteredCellVoltages(const Cell &voltages, const CellSuccess &voltages_success);
void broadcastAux(const Therm &temps, const ThermSuccess &temps_success);
void broadcastTemps(const Therm &seg_voltages, const ThermSuccess &seg_voltages_success);
void broadcastSegVoltages(const Segment &seg_voltages, const SegmentSuccess &seg_voltages_success);
void broadcastStatus(const Status &status, const SegmentSuccess &status_success);
void broadcastCellOpenWireCheck(const Owc &owc_ok, const CellSuccess owc_ok_success);
void broadcastInfo();

// Fault evaluation.
void initFaults();
bool checkWarnings();
bool checkFaults();

// Conversion and diagnostics.
expected<void, ErrorCode> runVoltageConversion(Cell &out_voltages, CellSuccess &out_success);
expected<void, ErrorCode> runFilteredVoltageConversion(Cell &out_voltages, CellSuccess &out_success);
expected<void, ErrorCode> runTempConversion(Therm &out_temps, ThermSuccess &out_success);
expected<void, ErrorCode> runSegVoltageConversion(Segment &out_seg_voltages, SegmentSuccess &out_seg_success);
expected<void, ErrorCode> runStatusConversion(Status &out_status, SegmentSuccess &out_success);
expected<void, ErrorCode> runCellOpenWireCheck(Owc &out_owc, CellSuccess &out_success);

} // namespace app::segments
