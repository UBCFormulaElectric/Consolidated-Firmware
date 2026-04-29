#pragma once

#include <cstdint>
#include <array>
#include "io_irs.hpp"
#include "io_faultLatch.hpp"
// #include "io_adbms.hpp"

// TODO: Remove definintions after io adbms and app segments are added
namespace io::adbms
{
#ifndef NUM_SEGMENTS
#define NUM_SEGMENTS 10U
#define CELLS_PER_SEGMENT 14U
// TODO: Have to change to match adbms regs
#define AUX_REG_GROUPS 3U
#define REGS_PER_GROUP 3U
#define AUX_REGS_PER_SEGMENT (AUX_REG_GROUPS * REGS_PER_GROUP)
#define MAX_CELL_VOLTAGE_FAULT_V 4.2f
#define MIN_CELL_VOLTAGE_FAULT_V 2.5f
#define MAX_CELL_TEMP_FAULT_C (60.0f)
#endif
} // namespace io::adbms
// Remove up to here

using namespace io::adbms;

namespace fakes
{
namespace irs
{
    void setNegativeState(app::can_utils::ContactorState state);
}
namespace ts
{
    void setVoltage(float voltage);
    void setCurrentHighResolution(float current);
    void setCurrentLowResolution(float current);
    void setVoltageDiagState(bool state);
    void setCurrentDiagState(bool state);
} // namespace ts

namespace faultLatch
{
    void     resetFaultLatch(const io::faultLatch::FaultLatch *latch);
    void     updateFaultLatch(io::faultLatch::FaultLatch *latch, io::faultLatch::FaultLatchState status);
    void     setCurrentStatus_resetCallCounts();
    uint32_t setCurrentStatus_getCallsWithArgs(
        const io::faultLatch::FaultLatch *latch,
        io::faultLatch::FaultLatchState   status);
} // namespace faultLatch

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
    void setConnectionStatus(app::can_utils::ChargerConnectedType status);
    void setCPDutyCycle(float duty_cycle);
} // namespace charger
} // namespace fakes
