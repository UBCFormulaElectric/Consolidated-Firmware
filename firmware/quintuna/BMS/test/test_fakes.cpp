#include "test_fakes.h"

extern "C"
{
#include "io_ltc6813.h"
    void io_ltc6813_readConfigurationRegisters(SegmentConfig configs[NUM_SEGMENTS], ExitCode success[NUM_SEGMENTS])
    {
        for (int i = 0; i < NUM_SEGMENTS; i++)
        {
            success[i] = EXIT_CODE_OK;
        }
    }
    ExitCode io_ltc6813_writeConfigurationRegisters(const SegmentConfig config[NUM_SEGMENTS])
    {
        return EXIT_CODE_OK;
    }
    void io_ltc6813_readVoltageRegisters(
        uint16_t cell_voltage_regs[NUM_SEGMENTS][CELLS_PER_SEGMENT],
        ExitCode comm_success[NUM_SEGMENTS][CELLS_PER_SEGMENT])
    {
        for (int i = 0; i < NUM_SEGMENTS; i++)
        {
            for (int j = 0; j < CELLS_PER_SEGMENT; j++)
            {
                cell_voltage_regs[i][j] = 0;
                comm_success[i][j]      = EXIT_CODE_OK;
            }
        }
    }
    ExitCode io_ltc6813_startCellsAdcConversion(void)
    {
        return EXIT_CODE_OK;
    }
    void io_ltc6813_readAuxRegisters(
        uint16_t aux_regs[NUM_SEGMENTS][AUX_REGS_PER_SEGMENT],
        ExitCode comm_success[NUM_SEGMENTS][AUX_REGS_PER_SEGMENT])
    {
        for (int i = 0; i < NUM_SEGMENTS; i++)
        {
            for (int j = 0; j < AUX_REGS_PER_SEGMENT; j++)
            {
                aux_regs[i][j]     = 0;
                comm_success[i][j] = EXIT_CODE_OK;
            }
        }
    }
    ExitCode io_ltc6813_startThermistorsAdcConversion(void)
    {
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
    ExitCode io_ltc6813_owcPull(PullDirection pull_direction)
    {
        return EXIT_CODE_OK;
    }
    ExitCode io_ltc6813_sendSelfTestVoltages(void)
    {
        return EXIT_CODE_OK;
    }
    ExitCode io_ltc6813_sendSelfTestAux(void)
    {
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
    ExitCode io_ltc6813_overlapADCTest(void)
    {
        return EXIT_CODE_OK;
    }
    ExitCode io_ltc6813_startInternalADCConversions(void)
    {
        return EXIT_CODE_OK;
    }
    void io_ltc6813_getStatus(StatusRegGroups status[NUM_SEGMENTS], ExitCode success[NUM_SEGMENTS])
    {
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

#include "io_irs.h"
    static ContactorsState positive_state = CONTACTORS_OPEN;
    void                   io_irs_setPositive(const ContactorsState state)
    {
        positive_state = state;
    }
    ContactorsState io_irs_positiveState(void)
    {
        return positive_state;
    }

    static ContactorsState precharge_state = CONTACTORS_OPEN;
    void                   io_irs_setPrecharge(const ContactorsState state)
    {
        precharge_state = state;
    }
    ContactorsState io_irs_prechargeState(void)
    {
        return precharge_state;
    }

    static ContactorsState negative_state = CONTACTORS_OPEN;
    ContactorsState        io_irs_negativeState(void)
    {
        return negative_state;
    }

#include "io_tractiveSystem.h"
    void io_tractiveSystem_init(void) {}

    static float tractiveSystemVoltage = 0.0f;
    float        io_tractiveSystem_getVoltage(void)
    {
        return tractiveSystemVoltage;
    }

    static float currentHighResolution = 0.0f, currentLowResolution = 0.0f;
    float        io_tractiveSystem_getCurrentHighResolution(void)
    {
        return currentHighResolution;
    }
    float io_tractiveSystem_getCurrentLowResolution(void)
    {
        return currentLowResolution;
    }

#include "io_charger.h"
    static ConnectionStatus connectionStatus = DISCONNECTED;
    ConnectionStatus        io_charger_getConnectionStatus()
    {
        return connectionStatus;
    }

    static float evse_dutyCycle = 0.0f;
    float        io_charger_getDutyCycle()
    {
        return evse_dutyCycle;
    }

#include "io_imd.h"
    float io_imd_getFrequency(void)
    {
        return 0.0f;
    }
    float io_imd_getDutyCycle(void)
    {
        return 0.0f;
    }
    uint32_t io_imd_getTimeSincePowerOn(void)
    {
        return 0;
    }
    uint8_t io_imd_pwmCounterTick(void)
    {
        return 0;
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

#include "io_faultLatch.h"
    // latches to operate on
    FaultLatch bms_ok_latch{};
    FaultLatch imd_ok_latch{};
    FaultLatch bspd_ok_latch{};

    void io_faultLatch_setCurrentStatus(const FaultLatch *latch, const FaultLatchState status)
    {
        const_cast<FaultLatch *>(latch)->status = status;
        const_cast<FaultLatch *>(latch)->latched_state =
            latch->latched_state == FAULT_LATCH_OK ? status : FAULT_LATCH_FAULT;
    }
    FaultLatchState io_faultLatch_getCurrentStatus(const FaultLatch *latch)
    {
        return latch->status;
    }
    FaultLatchState io_faultLatch_getLatchedStatus(const FaultLatch *latch)
    {
        return latch->latched_state;
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
namespace irs
{
    void setNegativeState(const ContactorsState state)
    {
        negative_state = state;
    }
} // namespace irs

namespace tractiveSystem
{
    void setVoltage(const float voltage)
    {
        tractiveSystemVoltage = voltage;
    }
    void setCurrentHighResolution(const float current)
    {
        currentHighResolution = current;
    }
    void setCurrentLowResolution(const float current)
    {
        currentLowResolution = current;
    }
} // namespace tractiveSystem

namespace faultLatches
{
    void resetFaultLatch(const FaultLatch *latch)
    {
        const_cast<FaultLatch *>(latch)->status        = FAULT_LATCH_OK;
        const_cast<FaultLatch *>(latch)->latched_state = FAULT_LATCH_OK;
    }
} // namespace faultLatches
} // namespace fakes
