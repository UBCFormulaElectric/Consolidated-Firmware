#include "test_fakes.hpp"

#include <algorithm>
#include <cstring>
#include <gtest/gtest.h>
#include <cmath>

#include "io_bmsShdn.hpp"
#include "io_bspdTest.hpp"
#include "io_canTx.hpp"
#include "io_charger.hpp"
#include "io_fans.hpp"
#include "io_imd.hpp"
#include "io_shdnLoopNode.hpp"
#include "io_tractiveSystem.hpp"
#include "io_time.hpp"

#include "util_errorCodes.hpp"
#include "util_utils.hpp"

struct FaultLatchParams
{
    const io::FaultLatch           *arg0;
    io::FaultLatch::FaultLatchState arg1;

    bool operator==(const FaultLatchParams &other) const { return arg0 == other.arg0 && arg1 == other.arg1; }
};
template <> struct std::hash<FaultLatchParams>
{
    std::size_t operator()(const FaultLatchParams &params) const noexcept
    {
        return hash<const io::FaultLatch *>()(params.arg0) ^ hash<io::FaultLatch::FaultLatchState>()(params.arg1);
    }
};
static std::unordered_map<FaultLatchParams, uint32_t> setCurrentStatus_call_count;

namespace io
{
namespace irs
{
    static auto positive_state  = app::can_utils::ContactorState::CONTACTOR_STATE_OPEN;
    static auto precharge_state = app::can_utils::ContactorState::CONTACTOR_STATE_OPEN;
    static auto negative_state  = app::can_utils::ContactorState::CONTACTOR_STATE_OPEN;

    app::can_utils::ContactorState negativeState()
    {
        return negative_state;
    }
    void setPositive(const app::can_utils::ContactorState state)
    {
        positive_state = state;
    }
    app::can_utils::ContactorState positiveState()
    {
        return positive_state;
    }
    void setPrecharge(const app::can_utils::ContactorState state)
    {
        precharge_state = state;
    }
    app::can_utils::ContactorState prechargeState()
    {
        return precharge_state;
    }
} // namespace irs

namespace ts
{
    static float voltage               = 0.0f;
    static float currentHighResolution = 0.0f;
    static float currentLowResolution  = 0.0f;
    static bool  voltageDiagState      = true;
    static bool  currentDiagState      = true;

    float getVoltage()
    {
        return voltage;
    }
    float getCurrentHighResolution()
    {
        return currentHighResolution;
    }
    float getCurrentLowResolution()
    {
        return currentLowResolution;
    }
    bool getVoltageSnsDiagState()
    {
        return voltageDiagState;
    }
    bool getCurrentSnsDiagState()
    {
        return currentDiagState;
    }
} // namespace ts

namespace imd
{
    static float   frequency   = 0.0f;
    static float   duty_cycle  = 0.0f;
    static uint8_t pwm_counter = 0;
    float          getFrequency()
    {
        return frequency;
    }
    float getDutyCycle()
    {
        return duty_cycle;
    }
    uint8_t pwmCounterTick()
    {
        return pwm_counter;
    }
    uint32_t getTimeSincePowerOn()
    {
        return time::getCurrentMs();
    }
} // namespace imd
namespace charger
{
    static app::can_utils::ChargerConnectedType connectionStatus =
        app::can_utils::ChargerConnectedType::CHARGER_DISCONNECTED;
    static float evse_dutyCycle = 0.0f;

    app::can_utils::ChargerConnectedType getConnectionStatus()
    {
        return connectionStatus;
    }
    float getCPDutyCycle()
    {
        return evse_dutyCycle;
    }
} // namespace charger

namespace shdn
{
    node hv_p_ok_node(app::can_tx::BMS_HVPShdnOKStatus_set);
    node hv_n_ok_node(app::can_tx::BMS_HVNShdnOKStatus_set);
    node loop_ok_node(app::can_tx::BMS_ShdnTermOKStatus_set);
} // namespace shdn

namespace fans
{
    void tick(const bool enable)
    {
        UNUSED(enable);
    }
} // namespace fans

namespace bspdtest
{
    void enable(const bool enable)
    {
        UNUSED(enable);
    }
    bool isCurrentThresholdExceeded(void)
    {
        return false;
    }
    bool isBrakePressureThresholdExceeded(void)
    {
        return false;
    }
    bool isAccelBrakeOk(void)
    {
        return true;
    }
} // namespace bspdtest

// ADBMS reads/writes are mocked in bmsMocks.cpp.

} // namespace io

// Faultlatch

io::FaultLatch bms_ok_latch{ io::FaultLatch::FaultLatchState::OK, io::FaultLatch::FaultLatchState::OK, false };
io::FaultLatch imd_ok_latch{ io::FaultLatch::FaultLatchState::OK, io::FaultLatch::FaultLatchState::OK, true };
io::FaultLatch bspd_ok_latch{ io::FaultLatch::FaultLatchState::OK, io::FaultLatch::FaultLatchState::OK, true };

void io::FaultLatch::setCurrentStatus(FaultLatchState new_status) const
{
    assert(!this->read_only);
    fakes::faultLatch::updateFaultLatch(const_cast<io::FaultLatch *>(this), new_status);
}
io::FaultLatch::FaultLatchState io::FaultLatch::getCurrentStatus() const
{
    return this->status;
}
io::FaultLatch::FaultLatchState io::FaultLatch::getLatchedStatus() const
{
    return this->latched_state;
}

namespace fakes
{
namespace irs
{
    void setNegativeState(const app::can_utils::ContactorState state)
    {
        io::irs::negative_state = state;
    }
} // namespace irs

namespace ts
{
    void setVoltage(const float voltage)
    {
        io::ts::voltage = voltage;
    }
    void setCurrentHighResolution(const float current)
    {
        io::ts::currentHighResolution = current;
    }
    void setCurrentLowResolution(const float current)
    {
        io::ts::currentLowResolution = current;
    }
    void setVoltageDiagState(const bool state)
    {
        io::ts::voltageDiagState = state;
    }
    void setCurrentDiagState(const bool state)
    {
        io::ts::currentDiagState = state;
    }
} // namespace ts

namespace faultLatch
{
    using io::FaultLatch;
    using FaultLatchState = FaultLatch::FaultLatchState;

    void resetFaultLatch(const FaultLatch *latch)
    {
        const_cast<FaultLatch *>(latch)->status        = FaultLatchState::OK;
        const_cast<FaultLatch *>(latch)->latched_state = FaultLatchState::OK;
    }
    void updateFaultLatch(FaultLatch *latch, FaultLatchState status)
    {
        if (latch->latched_state == FaultLatchState::OK && status == FaultLatchState::FAULT)
        {
            setCurrentStatus_call_count[FaultLatchParams{ const_cast<FaultLatch *>(latch), status }]++;
        }
        latch->status        = status;
        latch->latched_state = latch->latched_state == FaultLatchState::OK ? status : FaultLatchState::FAULT;
    }
    void setCurrentStatus_resetCallCounts()
    {
        setCurrentStatus_call_count = {};
    }
    uint32_t setCurrentStatus_getCallsWithArgs(const FaultLatch *latch, FaultLatchState status)
    {
        return setCurrentStatus_call_count[FaultLatchParams{ const_cast<FaultLatch *>(latch), status }];
    }
} // namespace faultLatch

namespace imd
{
    void setFrequency(const float frequency)
    {
        io::imd::frequency = frequency;
    }
    void setDutyCycle(const float duty_cycle)
    {
        io::imd::duty_cycle = duty_cycle;
    }
    void setPwmCounter(const uint8_t counter)
    {
        io::imd::pwm_counter = counter;
    }
} // namespace imd

// fakes::segments::* setters are implemented in bmsMocks.cpp alongside the ADBMS read mocks.

namespace charger
{
    void setConnectionStatus(const app::can_utils::ChargerConnectedType status)
    {
        io::charger::connectionStatus = status;
    }
    void setCPDutyCycle(const float duty_cycle)
    {
        io::charger::evse_dutyCycle = duty_cycle;
    }
} // namespace charger
} // namespace fakes
