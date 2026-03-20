#pragma once

#include "io_adbms.hpp"

#include <array>
#include <expected>

using namespace std;

namespace app::segments
{
enum class ThermistorMux
{
    THERMISTOR_MUX_0_7,
    THERMISTOR_MUX_8_13,
    THERMISTOR_MUX_COUNT,
};

struct CellParam
{
    uint8_t segment;
    uint8_t cell;
    float   voltage;
    float   temp;
};

void                      setDefaultConfig();
void                      setBalanceConfig(array<array<bool, io::CELLS_PER_SEGMENT>, io::NUM_SEGMENTS> &balance_config);
void                      setPwmConfig(array<array<uint8_t, io::CELLS_PER_SEGMENT>, io::NUM_SEGMENTS> &pwm_duty);
void                      setThermistorConfig(ThermistorMux mux);
expected<void, ErrorCode> configSync();
expected<void, ErrorCode> writeConfig();

void balancingInit();
void balancingTick(bool enable);
void broadcastCellVoltages();

float     getPackVoltage();
CellParam getMaxCellVoltage();
CellParam getMinCellVoltage();
CellParam getMaxCellTemp();
CellParam getMinCellTemp();

expected<void, ErrorCode> runVoltageConversion();

} // namespace app::segments
