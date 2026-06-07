#pragma once

#include <cstdint>
#include <array>

#include "app_canUtils.hpp"
#include "app_canTx.hpp"
#include "io_faultLatch.hpp"
#include "io_adbms.hpp" // NUM_SEGMENTS, CELLS_PER_SEGMENT, THERMISTORS_PER_SEGMENT and io::adbms types

// Note: no `using namespace io::adbms;` here — this header is included widely (incl. before <gtest>),
// and io::adbms::read / io::adbms::write would clash with POSIX read/write inside gtest.

// ADBMS register-layout constants used only by the test fakes (not exposed by io_adbms.hpp).
constexpr uint8_t AUX_REG_GROUPS       = 3U;
constexpr uint8_t REGS_PER_GROUP       = 3U;
constexpr uint8_t AUX_REGS_PER_SEGMENT = AUX_REG_GROUPS * REGS_PER_GROUP;

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
    void     resetFaultLatch(const io::FaultLatch *latch);
    void     updateFaultLatch(io::FaultLatch *latch, io::FaultLatch::FaultLatchState status);
    void     setCurrentStatus_resetCallCounts();
    uint32_t setCurrentStatus_getCallsWithArgs(const io::FaultLatch *latch, io::FaultLatch::FaultLatchState status);
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

    // Directly drive the placeholder getMaxCellVoltage()/getMaxCellTemp()/getPackVoltage()
    // values consumed by chargeState and precharge. These exist because app::segments is not yet
    // implemented in tree.
    void setMaxCellVoltage(float v);
    void setMaxCellTemp(float t);
    void setPackVoltage(float v);
} // namespace segments
namespace charger
{
    void setConnectionStatus(app::can_utils::ChargerConnectedType status);
    void setCPDutyCycle(float duty_cycle);
} // namespace charger
} // namespace fakes
