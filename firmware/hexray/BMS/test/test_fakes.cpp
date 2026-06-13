#include "test_fakes.hpp"

#include <algorithm>
#include <cstring>
#include <gtest/gtest.h>
#include <cmath>

#include "io_bmsShdn.hpp"
#include "io_bspdTest.hpp"
#include "io_canQueues.hpp"
#include "io_canTx.hpp"
#include "io_charger.hpp"
#include "io_fans.hpp"
#include "io_imd.hpp"
#include "io_shdnLoopNode.hpp"
#include "io_tractiveSystem.hpp"
#include "io_time.hpp"
#include "segments/app_segments_internal.hpp"

#include "util_errorCodes.hpp"
#include "util_utils.hpp"
#include "io_canQueues.hpp"

io::queue<io::CanMsg, 250> can_tx_queue{ "" };
io::queue<io::CanMsg, 128> can_rx_queue{ "" };

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

namespace adbms
{
    Cells<result<int16_t>> cell_voltages{};
    Segments<SegmentConfig> config{};
    Segments<PWMConfig>     pwm{};

    namespace write
    {
        [[nodiscard]] result<void> pwmReg(const Segments<PWMConfig> &)
        {
            return result<void>{};
        }

        [[nodiscard]] result<void> configReg(const Segments<SegmentConfig> &)
        {
            return result<void>{};
        }
    } // namespace write

    namespace read
    {
        [[nodiscard]] Segments<result<SegmentConfig>> configReg()
        {
            Segments<result<SegmentConfig>> out;
            for (size_t seg = 0U; seg < NUM_SEGMENTS; ++seg)
            {
                out[seg] = config[seg];
            }
            return out;
        }

        [[nodiscard]] Segments<result<PWMConfig>> pwmReg()
        {
            Segments<result<PWMConfig>> out;
            for (size_t seg = 0U; seg < NUM_SEGMENTS; ++seg)
            {
                out[seg] = pwm[seg];
            }
            return out;
        }

        [[nodiscard]] Cells<result<int16_t>> cellVoltage()
        {
            return cell_voltages;
        }

        [[nodiscard]] Cells<result<int16_t>> secondaryCellVoltage()
        {
            return cell_voltages;
        }

        [[nodiscard]] Segments<result<int16_t>> segVoltage()
        {
            return Segments<result<int16_t>>{};
        }

        [[nodiscard]] ThermGpios<result<int16_t>> thermGpioVoltage()
        {
            return ThermGpios<result<int16_t>>{};
        }

        [[nodiscard]] Segments<StatusGroupsRes> status()
        {
            return Segments<StatusGroupsRes>{};
        }
    } // namespace read

    namespace command
    {
        [[nodiscard]] result<void> startCellsAdc()
        {
            return result<void>{};
        }

        [[nodiscard]] result<void> startAuxAdc()
        {
            return result<void>{};
        }

        [[nodiscard]] result<void> pollSecondaryCellsAdc()
        {
            return result<void>{};
        }

        [[nodiscard]] result<void> pollCellsAdc()
        {
            return result<void>{};
        }

        [[nodiscard]] result<void> pollAuxAdc()
        {
            return result<void>{};
        }

        [[nodiscard]] result<void> startBalance()
        {
            return result<void>{};
        }

        [[nodiscard]] result<void> stopBalance()
        {
            return result<void>{};
        }

        [[nodiscard]] result<void> owcCells(OpenWireSwitch owcSwitch)
        {
            return result<void>{};
        }
    } // namespace command

    namespace clear
    {
        [[nodiscard]] result<void> aux()
        {
            return result<void>{};
        }

        [[nodiscard]] result<void> flags()
        {
            return result<void>{};
        }

        [[nodiscard]] result<void> cell()
        {
            return result<void>{};
        }

        [[nodiscard]] result<void> secondaryCell()
        {
            return result<void>{};
        }

        [[nodiscard]] result<void> filteredCell()
        {
            return result<void>{};
        }

        [[nodiscard]] result<void> stat()
        {
            return result<void>{};
        }
    } // namespace clear
} // namespace adbms
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

namespace adbms
{
    static int16_t voltageToReg(const float voltage)
    {
        return static_cast<int16_t>((voltage - 1.5f) / 150e-6f);
    }

    void setCellVoltage(const float voltage, const int seg, const int cell)
    {
        io::adbms::cell_voltages[static_cast<size_t>(seg)][static_cast<size_t>(cell)] = voltageToReg(voltage);
    }

    void setPackVoltageEvenly(const float voltage)
    {
        const float cell_voltage = voltage / static_cast<float>(NUM_SEGMENTS * CELLS_PER_SEGMENT);
        for (size_t seg = 0; seg < NUM_SEGMENTS; seg++)
        {
            for (size_t cell = 0; cell < CELLS_PER_SEGMENT; cell++)
            {
                setCellVoltage(cell_voltage, static_cast<int>(seg), static_cast<int>(cell));
            }
        }
    }

    constexpr uint16_t VUV = 0x01A1; // 2.5V
    constexpr uint16_t VOV = 0x0465; // 4.2V

    io::adbms::SegmentConfig healthySegmentConfig()
    {
        io::adbms::SegmentConfig sc{};
        sc.reg_a.cth       = 0x01;
        sc.reg_a.ref_on    = 0x01;
        sc.reg_a.gpio_1_8  = 0xFF;
        sc.reg_a.gpio_9_10 = 0x03;
        sc.reg_a.fc        = 0x03;

        sc.reg_b.vuv_0_7  = static_cast<uint8_t>(VUV & 0xFF);
        sc.reg_b.vuv_8_11 = static_cast<uint8_t>((VUV >> 8) & 0x0F);
        sc.reg_b.vov_0_3  = static_cast<uint8_t>(VOV & 0x0F);
        sc.reg_b.vov_4_11 = static_cast<uint8_t>((VOV >> 4) & 0xFF);
        return sc;
    }

    io::adbms::PWMConfig healthyPwmConfig()
    {
        io::adbms::PWMConfig pc{};
        pc.reg_b.res = 0xFFFFFFFFu;
        return pc;
    }

    void setHealthyConfigs()
    {
        for (size_t seg = 0U; seg < NUM_SEGMENTS; ++seg)
        {
            io::adbms::config[seg] = healthySegmentConfig();
            io::adbms::pwm[seg]    = healthyPwmConfig();
        }
    }

    void setMismatchedConfigs()
    {
        for (size_t seg = 0U; seg < NUM_SEGMENTS; ++seg)
        {
            io::adbms::SegmentConfig sc = healthySegmentConfig();
            sc.reg_a.flag_d             = 0xFF;
            io::adbms::config[seg]      = sc;
            io::adbms::pwm[seg]         = healthyPwmConfig();
        }
    }

} // namespace adbms

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
