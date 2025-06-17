#include "test_fakes.h"

#include <algorithm>
#include <cstring>
#include <gtest/gtest.h>

extern "C"
{
#include "io_time.h"
}

extern "C"
{
#include "io_ltc6813.h"
    static std::array<SegmentConfig, NUM_SEGMENTS> segment_config{};

    void io_ltc6813_readConfigurationRegisters(SegmentConfig configs[NUM_SEGMENTS], ExitCode success[NUM_SEGMENTS])
    {
        for (int i = 0; i < NUM_SEGMENTS; i++)
        {
            configs[i] = segment_config[i];
            success[i] = EXIT_CODE_OK;
        }
    }
    ExitCode io_ltc6813_writeConfigurationRegisters(const SegmentConfig config[NUM_SEGMENTS])
    {
        std::ranges::copy_n(config, NUM_SEGMENTS, segment_config.data());
        return EXIT_CODE_OK;
    }

    static std::array<std::array<uint16_t, CELLS_PER_SEGMENT>, NUM_SEGMENTS> voltage_regs{};

    static bool     started_adc_conversion     = false;
    static bool     started_overlap_test       = false;
    static bool     started_self_test_voltages = false;
    static uint16_t expected_self_test_value   = 0x0;

    ExitCode io_ltc6813_startCellsAdcConversion(void)
    {
        started_adc_conversion = true;
        return EXIT_CODE_OK;
    }
    ExitCode io_ltc6813_overlapADCTest(void)
    {
        started_overlap_test = true;
        return EXIT_CODE_OK;
    }
    ExitCode io_ltc6813_sendSelfTestVoltages(void)
    {
        started_self_test_voltages = true;
        return EXIT_CODE_OK;
    }
    void io_ltc6813_readVoltageRegisters(
        uint16_t cell_voltage_regs[NUM_SEGMENTS][CELLS_PER_SEGMENT],
        ExitCode comm_success[NUM_SEGMENTS][CELLS_PER_SEGMENT])
    {
        if (started_adc_conversion || started_overlap_test)
        {
            memcpy(cell_voltage_regs, voltage_regs.data(), sizeof(uint16_t) * NUM_SEGMENTS * CELLS_PER_SEGMENT);
        }
        else if (started_self_test_voltages)
        {
            // Fill with self-test values
            for (int i = 0; i < NUM_SEGMENTS; i++)
            {
                for (int j = 0; j < CELLS_PER_SEGMENT; j++)
                {
                    cell_voltage_regs[i][j] = expected_self_test_value; // Example self-test value
                }
            }
        }
        else
        {
            FAIL() << "Did not start ADC conversion, overlap test or self-test voltages";
        }
        for (int i = 0; i < NUM_SEGMENTS; i++)
        {
            for (int j = 0; j < CELLS_PER_SEGMENT; j++)
            {
                comm_success[i][j] = EXIT_CODE_OK;
            }
        }
        started_adc_conversion = false;
    }

    static std::array<std::array<uint16_t, AUX_REGS_PER_SEGMENT>, NUM_SEGMENTS> aux_regs_storage{};

    bool started_therm_adc_conversion = false;
    bool started_self_test_aux        = false;

    void io_ltc6813_readAuxRegisters(
        uint16_t aux_regs[NUM_SEGMENTS][AUX_REGS_PER_SEGMENT],
        ExitCode comm_success[NUM_SEGMENTS][AUX_REGS_PER_SEGMENT])
    {
        // if (started_therm_adc_conversion || started_self_test_aux)
        // {
        //     memcpy(aux_regs, aux_regs_storage.data(), sizeof(uint16_t) * NUM_SEGMENTS * AUX_REGS_PER_SEGMENT);
        // }
        // else
        // {
        //     FAIL() << "Did not start thermistor ADC conversion";
        // }
        for (int i = 0; i < NUM_SEGMENTS; i++)
        {
            for (int j = 0; j < AUX_REGS_PER_SEGMENT; j++)
            {
                aux_regs[i][j]     = 0;
                comm_success[i][j] = EXIT_CODE_OK;
            }
        }
        started_therm_adc_conversion = false;
    }

    ExitCode io_ltc6813_startThermistorsAdcConversion(void)
    {
        started_therm_adc_conversion = true;
        return EXIT_CODE_OK;
    }
    void     io_ltc6813_wakeup(void) {}
    ExitCode io_ltc6813_pollAdcConversions(void)
    {
        return EXIT_CODE_OK;
    }
    ExitCode io_ltc6813_sendBalanceCommand(void)
    {
        return EXIT_CODE_OK;
    }
    ExitCode io_ltc6813_sendStopBalanceCommand(void)
    {
        return EXIT_CODE_OK;
    }
    ExitCode io_ltc6813_owcPull(const PullDirection pull_direction)
    {
        UNUSED(pull_direction);
        return EXIT_CODE_OK;
    }
    ExitCode io_ltc6813_sendSelfTestAux(void)
    {
        started_self_test_aux = true;
        return EXIT_CODE_OK;
    }
    ExitCode io_ltc6813_sendSelfTestStat(void)
    {
        return EXIT_CODE_OK;
    }
    ExitCode io_ltc6813_diagnoseMUX(void)
    {
        return EXIT_CODE_OK;
    }
    ExitCode io_ltc6813_startInternalADCConversions(void)
    {
        return EXIT_CODE_OK;
    }
    void io_ltc6813_getStatus(StatusRegGroups status[NUM_SEGMENTS], ExitCode success[NUM_SEGMENTS])
    {
        UNUSED(status);
        for (int i = 0; i < NUM_SEGMENTS; i++)
        {
            success[i] = EXIT_CODE_OK;
        }
    }
    ExitCode io_ltc6813_clearCellRegisters(void)
    {
        return EXIT_CODE_OK;
    }
    ExitCode io_ltc6813_clearAuxRegisters(void)
    {
        return EXIT_CODE_OK;
    }
    ExitCode io_ltc6813_clearStatRegisters(void)
    {
        return EXIT_CODE_OK;
    }

#include "io_charger.h"
    static ChargerConnectedType connectionStatus = CHARGER_DISCONNECTED;
    ChargerConnectedType        io_charger_getConnectionStatus()
    {
        return connectionStatus;
    }

    static float evse_dutyCycle = 0.0f;
    float        io_charger_getCPDutyCycle()
    {
        return evse_dutyCycle;
    }

#include "io_bmsShdn.h"
    bool io_bmsShdn_msd_shdn_sns_pin_get(void)
    {
        return false;
    }
    bool io_bmsShdn_hv_p_intlck_sns_pin_get(void)
    {
        return false;
    }
    bool io_bmsShdn_hv_n_intlck_sns_pin_get(void)
    {
        return false;
    }

#include "io_bspdTest.h"
    void io_bspdTest_enable(const bool enable)
    {
        UNUSED(enable);
    }
    bool io_bspdTest_isCurrentThresholdExceeded(void)
    {
        return false;
    }
    bool io_bspdTest_isBrakePressureThresholdExceeded(void)
    {
        return false;
    }
    bool io_bspdTest_isAccelBrakeOk(void)
    {
        return true;
    }

#include "io_canTx.h"
    void io_canTx_init(
        void (*transmit_can1_msg_func)(const JsonCanMsg *),
        void (*transmit_charger_msg_func)(const JsonCanMsg *))
    {
        UNUSED(transmit_can1_msg_func);
        UNUSED(transmit_charger_msg_func);
    }
}

namespace fakes
{

namespace segments
{
    void setCellVoltages(const std::array<std::array<float, CELLS_PER_SEGMENT>, NUM_SEGMENTS> &voltages)
    {
        for (int i = 0; i < NUM_SEGMENTS; i++)
        {
            for (int j = 0; j < CELLS_PER_SEGMENT; j++)
            {
                voltage_regs[i][j] = static_cast<uint16_t>(voltages[i][j] * 1e4);
            }
        }
    }

    void setCellTemperatures(const std::array<std::array<float, AUX_REGS_PER_SEGMENT>, NUM_SEGMENTS> &temperatures)
    {
        for (int i = 0; i < NUM_SEGMENTS; i++)
        {
            for (int j = 0; j < AUX_REGS_PER_SEGMENT; j++)
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
        for (int i = 0; i < NUM_SEGMENTS; i++)
        {
            for (int j = 0; j < CELLS_PER_SEGMENT; j++)
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
} // namespace segments
} // namespace fakes
