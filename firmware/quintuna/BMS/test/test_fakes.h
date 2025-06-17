#pragma once

#include <cstdint>
#include <array>

extern "C"
{
#include "io_irs.h"
#include "io_faultLatch.h"
#include "io_ltc6813.h"
}

namespace fakes
{
namespace segments
{
    void setCellVoltages(const std::array<std::array<float, CELLS_PER_SEGMENT>, NUM_SEGMENTS> &voltages);
    void setPackVoltageEvenly(float pack_voltage);
    void setCellTemperatures(const std::array<std::array<float, AUX_REGS_PER_SEGMENT>, NUM_SEGMENTS> &temperatures);
} // namespace segments
} // namespace fakes
