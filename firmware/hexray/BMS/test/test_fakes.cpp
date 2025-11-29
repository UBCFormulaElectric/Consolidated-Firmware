#include "test_fakes.hpp"

#include <algorithm>
#include <cstring>
#include <gtest/gtest.h>

#include "io_time.hpp"

struct FaultLatchParams
{
    io::faultLatch::FaultLatch     *arg0;
    io::faultLatch::FaultLatchState arg1;

    bool operator==(const FaultLatchParams &other) const { return arg0 == other.arg0 && arg1 == other.arg1; }
};
template <> struct std::hash<FaultLatchParams>
{
    std::size_t operator()(const FaultLatchParams &params) const noexcept
    {
        return hash<const io::faultLatch::FaultLatch *>()(params.arg0) ^
               hash<io::faultLatch::FaultLatchState>()(params.arg1);
    }
};
static std::unordered_map<FaultLatchParams, uint32_t> setCurrentStatus_call_count;

extern "C"
{
    // #include "io_adbms.h"
    //     static std::array<SegmentConfig, NUM_SEGMENTS> segment_config{};

    //     void io_adbms_readConfigurationRegisters(SegmentConfig configs[NUM_SEGMENTS], ExitCode success[NUM_SEGMENTS])
    //     {
    //         for (size_t i = 0; i < NUM_SEGMENTS; i++)
    //         {
    //             configs[i] = segment_config[i];
    //             success[i] = EXIT_CODE_OK;
    //         }
    //     }
    //     ExitCode io_adbms_writeConfigurationRegisters(const SegmentConfig config[NUM_SEGMENTS])
    //     {
    //         std::ranges::copy_n(config, NUM_SEGMENTS, segment_config.data());
    //         return EXIT_CODE_OK;
    //     }

    static std::array<std::array<uint16_t, CELLS_PER_SEGMENT>, NUM_SEGMENTS> voltage_regs{};

    static bool     started_adc_conversion     = false;
    static bool     started_overlap_test       = false;
    static bool     started_self_test_voltages = false;
    static uint16_t expected_self_test_value   = 0x0;

    //     ExitCode io_adbms_startCellsAdcConversion(void)
    //     {
    //         started_adc_conversion = true;
    //         return EXIT_CODE_OK;
    //     }
    //     ExitCode io_adbms_overlapADCTest(void)
    //     {
    //         started_overlap_test = true;
    //         return EXIT_CODE_OK;
    //     }
    //     ExitCode io_adbms_sendSelfTestVoltages(void)
    //     {
    //         started_self_test_voltages = true;
    //         return EXIT_CODE_OK;
    //     }
    //     void io_adbms_readVoltageRegisters(
    //         uint16_t cell_voltage_regs[NUM_SEGMENTS][CELLS_PER_SEGMENT],
    //         ExitCode comm_success[NUM_SEGMENTS][CELLS_PER_SEGMENT])
    //     {
    //         if (started_adc_conversion || started_overlap_test)
    //         {
    //             memcpy(cell_voltage_regs, voltage_regs.data(), sizeof(uint16_t) * NUM_SEGMENTS * CELLS_PER_SEGMENT);
    //         }
    //         else if (started_self_test_voltages)
    //         {
    //             // Fill with self-test values
    //             for (int i = 0; i < NUM_SEGMENTS; i++)
    //             {
    //                 for (int j = 0; j < CELLS_PER_SEGMENT; j++)
    //                 {
    //                     cell_voltage_regs[i][j] = expected_self_test_value; // Example self-test value
    //                 }
    //             }
    //         }
    //         else
    //         {
    //             FAIL() << "Did not start ADC conversion, overlap test or self-test voltages";
    //         }
    //         for (int i = 0; i < NUM_SEGMENTS; i++)
    //         {
    //             for (int j = 0; j < CELLS_PER_SEGMENT; j++)
    //             {
    //                 comm_success[i][j] = EXIT_CODE_OK;
    //             }
    //         }
    //         started_adc_conversion = false;
    //     }

    static std::array<std::array<uint16_t, AUX_REGS_PER_SEGMENT>, NUM_SEGMENTS> aux_regs_storage{};

    //     bool started_therm_adc_conversion = false;
    //     bool started_self_test_aux        = false;

    //     void io_ltc6813_readAuxRegisters(
    //         uint16_t aux_regs[NUM_SEGMENTS][AUX_REGS_PER_SEGMENT],
    //         ExitCode comm_success[NUM_SEGMENTS][AUX_REGS_PER_SEGMENT])
    //     {
    //         if (started_therm_adc_conversion || started_self_test_aux)
    //         {
    //             memcpy(aux_regs, aux_regs_storage.data(), sizeof(uint16_t) * NUM_SEGMENTS * AUX_REGS_PER_SEGMENT);
    //             for (int i = 0; i < NUM_SEGMENTS; i++)
    //             {
    //                 for (int j = 0; j < AUX_REGS_PER_SEGMENT; j++)
    //                 {
    //                     // aux_regs[i][j]     = 0;
    //                     comm_success[i][j] = EXIT_CODE_OK;
    //                 }
    //             }
    //         }
    //         else
    //         {
    //             FAIL() << "Did not start thermistor ADC conversion";
    //         }
    //         started_therm_adc_conversion = false;
    //     }

    //     ExitCode io_adbms_startThermistorsAdcConversion(void)
    //     {
    //         started_therm_adc_conversion = true;
    //         return EXIT_CODE_OK;
    //     }
    //     void     io_adbms_wakeup(void) {}
    //     ExitCode io_adbms_pollAdcConversions(void)
    //     {
    //         return EXIT_CODE_OK;
    //     }
    //     ExitCode io_adbms_sendBalanceCommand(void)
    //     {
    //         return EXIT_CODE_OK;
    //     }
    //     ExitCode io_adbms_sendStopBalanceCommand(void)
    //     {
    //         return EXIT_CODE_OK;
    //     }
    //     ExitCode io_adbms_owcPull(const PullDirection pull_direction)
    //     {
    //         UNUSED(pull_direction);
    //         return EXIT_CODE_OK;
    //     }
    //     ExitCode io_adbms_sendSelfTestAux(void)
    //     {
    //         started_self_test_aux = true;
    //         return EXIT_CODE_OK;
    //     }
    //     ExitCode io_adbms_sendSelfTestStat(void)
    //     {
    //         return EXIT_CODE_OK;
    //     }
    //     ExitCode io_adbms_diagnoseMUX(void)
    //     {
    //         return EXIT_CODE_OK;
    //     }
    //     ExitCode io_adbms_startInternalADCConversions(void)
    //     {
    //         return EXIT_CODE_OK;
    //     }
    //     void io_adbms_getStatus(StatusRegGroups status[NUM_SEGMENTS], ExitCode success[NUM_SEGMENTS])
    //     {
    //         UNUSED(status);
    //         for (int i = 0; i < NUM_SEGMENTS; i++)
    //         {
    //             success[i] = EXIT_CODE_OK;
    //         }
    //     }
    //     ExitCode io_adbms_clearCellRegisters(void)
    //     {
    //         return EXIT_CODE_OK;
    //     }
    //     ExitCode io_adbms_clearAuxRegisters(void)
    //     {
    //         return EXIT_CODE_OK;
    //     }
    //     ExitCode io_adbms_clearStatRegisters(void)
    //     {
    //         return EXIT_CODE_OK;
    //     }

#include "io_canTx.h"
    void io_canTx_init(
        void (*transmit_can1_msg_func)(const JsonCanMsg *),
        void (*transmit_charger_msg_func)(const JsonCanMsg *))
    {
        UNUSED(transmit_can1_msg_func);
        UNUSED(transmit_charger_msg_func);
    }
}

// C++ namespace wrappers so production C++ code (io::*) can call the
// same fake state used by the C-style io_* functions above. Tests may
// continue to use `fakes::*` helpers to mutate this state.
namespace io
{
#include "io_irs.hpp"
namespace irs
{
    static ContactorState positive_state  = CONTACTOR_STATE_OPEN;
    static ContactorState precharge_state = CONTACTOR_STATE_OPEN;
    static ContactorState negative_state  = CONTACTOR_STATE_OPEN;

    ContactorState negativeState()
    {
        return negative_state;
    }
    void setPositive(const ContactorState state)
    {
        positive_state = state;
    }
    ContactorState positiveState()
    {
        return positive_state;
    }
    void setPrecharge(const ContactorState state)
    {
        precharge_state = state;
    }
    ContactorState prechargeState()
    {
        return precharge_state;
    }
} // namespace irs

#include "io_tractiveSystem.hpp"
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

#include "io_imd.hpp"
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

#include "io_faultLatch.hpp"
namespace faultLatch
{
    FaultLatch bms_ok_latch{ FAULT_LATCH_OK, FAULT_LATCH_OK, true, false };
    FaultLatch imd_ok_latch{ FAULT_LATCH_OK, FAULT_LATCH_OK, false, true };
    FaultLatch bspd_ok_latch{ FAULT_LATCH_OK, FAULT_LATCH_OK, false, true };

    void setCurrentStatus(const FaultLatch *latch, const FaultLatchState status)
    {
        assert(!latch->read_only);
        fakes::faultLatches::updateFaultLatch(const_cast<FaultLatch *>(latch), status);
    }
    FaultLatchState getCurrentStatus(const FaultLatch *latch)
    {
        return latch->status;
    }
    FaultLatchState getLatchedStatus(const FaultLatch *latch)
    {
        return latch->latched_state;
    }
} // namespace faultLatch

#include "io_charger.hpp"
namespace charger
{
    static ChargerConnectedType connectionStatus = CHARGER_DISCONNECTED;
    static float                evse_dutyCycle   = 0.0f;

    ChargerConnectedType getConnectionStatus()
    {
        return connectionStatus;
    }
    float getCPDutyCycle()
    {
        return evse_dutyCycle;
    }
} // namespace charger

#include "io_bmsShdn.hpp"
namespace shdn
{
    static bool msd_shdn_sns    = false;
    static bool hv_p_intlck_sns = false;
    static bool hv_n_intlck_sns = false;

    bool msd_shdn_sns_pin_get()
    {
        return msd_shdn_sns;
    }
    bool hv_p_intlck_sns_pin_get()
    {
        return hv_p_intlck_sns;
    }
    bool hv_n_intlck_sns_pin_get()
    {
        return hv_n_intlck_sns;
    }
} // namespace shdn

#include "io_fans.hpp"
namespace fans
{
    void io_fans_tick(const bool enable)
    {
        (void)enable;
    }
} // namespace fans

#include "io_bspdTest.hpp"
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
    bool started_therm_adc_conversion = false;
    bool started_cell_adc_conversion  = false;

    ExitCode sendCommand(const uint16_t command)
    {
        UNUSED(command);
        return EXIT_CODE_OK;
    }

    ExitCode poll(uint16_t cmd, uint8_t *poll_buf, uint16_t poll_buf_len)
    {
        UNUSED(cmd);
        std::memset(poll_buf, 0, poll_buf_len);
        return EXIT_CODE_OK;
    }

    void readRegGroup(
        uint16_t cmd,
        uint16_t regs[NUM_SEGMENTS][adbms::REGS_PER_GROUP],
        ExitCode comm_success[NUM_SEGMENTS])
    {
        UNUSED(cmd);
        std::memset(regs, 0, NUM_SEGMENTS * adbms::REGS_PER_GROUP * sizeof(uint16_t));
        for (size_t i = 0; i < NUM_SEGMENTS; i++)
        {
            comm_success[i] = EXIT_CODE_OK;
        }
    }

    ExitCode writeRegGroup(uint16_t cmd, uint16_t regs[NUM_SEGMENTS][adbms::REGS_PER_GROUP])
    {
        UNUSED(cmd);
        UNUSED(regs);
        return EXIT_CODE_OK;
    }

    ExitCode sendBalanceCommand(void)
    {
        return EXIT_CODE_OK;
    }

    ExitCode sendStopBalanceCommand(void)
    {
        return EXIT_CODE_OK;
    }
} // namespace adbms

} // namespace io

namespace fakes
{
namespace irs
{
    void setNegativeState(const ContactorState state)
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

namespace faultLatches
{
    void resetFaultLatch(const FaultLatch *latch)
    {
        const_cast<FaultLatch *>(latch)->status        = FAULT_LATCH_OK;
        const_cast<FaultLatch *>(latch)->latched_state = FAULT_LATCH_OK;
    }
    void updateFaultLatch(FaultLatch *latch, const FaultLatchState status)
    {
        if (latch->latched_state == FAULT_LATCH_OK && status == FAULT_LATCH_FAULT)
        {
            setCurrentStatus_call_count[FaultLatchParams{ const_cast<FaultLatch *>(latch), status }]++;
        }
        latch->status        = status;
        latch->latched_state = latch->latched_state == FAULT_LATCH_OK ? status : FAULT_LATCH_FAULT;
    }
    void setCurrentStatus_resetCallCounts()
    {
        setCurrentStatus_call_count = {};
    }
    uint32_t setCurrentStatus_getCallsWithArgs(const FaultLatch *latch, const FaultLatchState status)
    {
        return setCurrentStatus_call_count[FaultLatchParams{ const_cast<FaultLatch *>(latch), status }];
    }
} // namespace faultLatches

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

namespace segments
{
    void setCellVoltages(const std::array<std::array<float, CELLS_PER_SEGMENT>, NUM_SEGMENTS> &voltages)
    {
        for (size_t i = 0; i < NUM_SEGMENTS; i++)
        {
            for (size_t j = 0; j < CELLS_PER_SEGMENT; j++)
            {
                voltage_regs[i][j] = static_cast<uint16_t>(voltages[i][j] * 1e4f);
            }
        }
    }

    void setCellVoltage(const size_t segment, const size_t cell, const float voltage)
    {
        voltage_regs[segment][cell] = static_cast<uint16_t>(voltage * 1e4f);
    }

    void setCellTemperatures(const std::array<std::array<float, AUX_REGS_PER_SEGMENT>, NUM_SEGMENTS> &temperatures)
    {
        for (size_t i = 0; i < NUM_SEGMENTS; i++)
        {
            for (size_t j = 0; j < AUX_REGS_PER_SEGMENT; j++)
            {
                aux_regs_storage[i][j] =
                    static_cast<uint16_t>(temperatures[i][j] * 1000); // Not sure if conversion is correct
            }
        }
    }

    void setPackVoltageEvenly(const float pack_voltage)
    {
        const float cell_voltage = pack_voltage / (NUM_SEGMENTS * CELLS_PER_SEGMENT);
        std::array<std::array<float, CELLS_PER_SEGMENT>, NUM_SEGMENTS> v{};
        for (size_t i = 0; i < NUM_SEGMENTS; i++)
        {
            for (size_t j = 0; j < CELLS_PER_SEGMENT; j++)
            {
                v[i][j] = cell_voltage;
            }
        }
        setCellVoltages(v);
    }

    void setExpectedVoltageSelfTestValue(const uint16_t value)
    {
        expected_self_test_value = value;
    }

    void SetAuxRegs(const float voltage)
    {
        for (size_t i = 0; i < NUM_SEGMENTS; i++)
        {
            for (size_t j = 0; j < AUX_REGS_PER_SEGMENT; j++)
            {
                aux_regs_storage[i][j] = static_cast<uint16_t>(voltage * 1000); // Not sure if conversion is correct
            }
        }
    }

    void SetAuxReg(const uint8_t segment, const uint8_t cell, const float voltage)
    {
        aux_regs_storage[segment][cell] = static_cast<uint16_t>(voltage * 1000); // Not sure if conversion is correct
    }
} // namespace segments

namespace charger
{
    void setConnectionStatus(const ChargerConnectedType status)
    {
        io::charger::connectionStatus = status;
    }
    void setCPDutyCycle(const float duty_cycle)
    {
        io::charger::evse_dutyCycle = duty_cycle;
    }
} // namespace charger
} // namespace fakes
