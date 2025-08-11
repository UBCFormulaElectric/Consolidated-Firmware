#pragma once

#include <cstdint>
#include <array>

extern "C"
{
#include "io_ltc6813.h"
}

namespace fakes
{
namespace segments
{
    void SetCellVoltages(const std::array<std::array<float, CELLS_PER_SEGMENT>, NUM_SEGMENTS> &voltages);
    void SetCellVoltage(uint8_t segment, uint8_t cell, float voltage);
    void SetPackVoltageEvenly(float pack_voltage);

    // TODO: Better testing interface for temps.
    void SetAuxRegs(float voltage);
    void SetAuxReg(uint8_t segment, uint8_t cell, float voltage);
} // namespace segments
} // namespace fakes
