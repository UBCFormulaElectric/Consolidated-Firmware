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
    void setStartCellsAdcOk(bool ok);
    void setPollCellsAdcOk(bool ok);
    void setCellVoltage(size_t segment, size_t cell, float voltage);
    void setAllCellVoltages(float voltage);
    void setCellReadError(size_t segment, size_t cell, ErrorCode error);

    void setStartSecondaryCellsAdcOk(bool ok);
    void setPollSecondaryCellsAdcOk(bool ok);
    void setCellOwcVoltage(io::adbms::OpenWireSwitch channel, size_t segment, size_t cell, float voltage);
    void setAllCellOwcVoltages(io::adbms::OpenWireSwitch channel, float voltage);
    void setCellOwcReadError(size_t segment, size_t cell, ErrorCode error);

    void setStartAuxAdcOk(bool ok);
    void setPollAuxAdcOk(bool ok);
    void setCellTemperature(size_t segment, size_t gpio, float temperature_c);
    void setAllCellTemperatures(float temperature_c);
    void setThermReadError(size_t segment, size_t gpio, ErrorCode error);

    void resetAdbmsMocks();
} // namespace segments
namespace charger
{
    void setConnectionStatus(app::can_utils::ChargerConnectedType status);
    void setCPDutyCycle(float duty_cycle);
} // namespace charger
} // namespace fakes
