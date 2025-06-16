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
namespace irs
{
    void setNegativeState(bool closed);
}
namespace tractiveSystem
{
    void setVoltage(float voltage);
    void setCurrentHighResolution(float current);
    void setCurrentLowResolution(float current);
    void setVoltageDiagState(bool state);
    void setCurrentDiagState(bool state);
} // namespace tractiveSystem

namespace faultLatches
{
    void resetFaultLatch(const FaultLatch *latch);
    void updateFaultLatch(FaultLatch *latch, FaultLatchState status);
} // namespace faultLatches

namespace imd
{
    void setFrequency(float frequency);
    void setDutyCycle(float duty_cycle);
    void setPwmCounter(uint8_t counter);
} // namespace imd
namespace segments
{
    void setCellVoltages(const std::array<std::array<float, CELLS_PER_SEGMENT>, NUM_SEGMENTS> &voltages);
    void setPackVoltageEvenly(float pack_voltage);
    void setCellTemperatures(const std::array<std::array<float, AUX_REGS_PER_SEGMENT>, NUM_SEGMENTS> &temperatures);
} // namespace segments
} // namespace fakes
