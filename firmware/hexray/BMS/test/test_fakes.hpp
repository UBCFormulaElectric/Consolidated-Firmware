#pragma once

#include <cstdint>
#include <array>

#include "app_canUtils.hpp"
#include "app_canTx.hpp"
#include "io_faultLatch.hpp"
#include "io_adbms.hpp"

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
namespace adbms
{
    void setPackVoltageEvenly(float voltage);
    void setCellVoltage(int seg, int cell, float voltage);
    // Config register read-back echoes what was written -> config sync sees a healthy chip.
    void setHealthyConfigs();
    // Config register read-back differs from what was written -> config sync reports a mismatch.
    void setMismatchedConfigs();

} // namespace adbms
namespace charger
{
    void setConnectionStatus(app::can_utils::ChargerConnectedType status);
    void setCPDutyCycle(float duty_cycle);
} // namespace charger
} // namespace fakes
