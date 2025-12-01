#pragma once

#include <cstdint>
#include <array>
#include "io_irs.hpp"
#include "io_faultLatch.hpp"
// #include "io_adbms.hpp"

namespace fakes
{
namespace irs
{
    void setNegativeState(ContactorState state);
}
namespace ts
{
    void setVoltage(float voltage);
    void setCurrentHighResolution(float current);
    void setCurrentLowResolution(float current);
    void setVoltageDiagState(bool state);
    void setCurrentDiagState(bool state);
} // namespace ts

namespace faultLatches
{
    void     resetFaultLatch(const FaultLatch *latch);
    void     updateFaultLatch(FaultLatch *latch, FaultLatchState status);
    void     setCurrentStatus_resetCallCounts();
    uint32_t setCurrentStatus_getCallsWithArgs(const FaultLatch *latch, FaultLatchState status);
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
    void setCellVoltage(size_t segment, size_t cell, float voltage);
    void setPackVoltageEvenly(float pack_voltage);
    void setCellTemperatures(const std::array<std::array<float, AUX_REGS_PER_SEGMENT>, NUM_SEGMENTS> &temperatures);

    // TODO: Better testing interface for temps.
    void SetAuxRegs(float voltage);
    void SetAuxReg(uint8_t segment, uint8_t cell, float voltage);
} // namespace segments
namespace charger
{
    void setConnectionStatus(ChargerConnectedType status);
    void setCPDutyCycle(float duty_cycle);
} // namespace charger
} // namespace fakes
