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
namespace segments
{
    void setCellVoltages(const std::array<std::array<float, CELLS_PER_SEGMENT>, NUM_SEGMENTS> &voltages);
    void setCellVoltage(size_t segment, size_t cell, float voltage);
    void setPackVoltageEvenly(float pack_voltage);
    void setCellTemperatures(const std::array<std::array<float, THERM_GPIOS_PER_SEGMENT>, NUM_SEGMENTS> &temperatures);

    // Sets every thermistor GPIO to the given temperature (Celsius).
    void SetAuxRegs(float temperature_c);
    void SetAuxReg(uint8_t segment, uint8_t gpio, float temperature_c);
} // namespace segments
namespace charger
{
    void setConnectionStatus(app::can_utils::ChargerConnectedType status);
    void setCPDutyCycle(float duty_cycle);
} // namespace charger
} // namespace fakes
