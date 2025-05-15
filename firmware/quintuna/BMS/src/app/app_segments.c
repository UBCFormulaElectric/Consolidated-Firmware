#include "app_segments.h"
#include "app_canTx.h"
#include "app_thermistors.h"

#include "io_ltc6813.h"
#include "io_time.h"

#include <hw_utils.h>
#include <string.h>

static const ADCSpeed s = ADCSpeed_7kHz;

static uint16_t segment_vref[NUM_SEGMENTS];

#define V_PER_100UV (1E-4f)
#define CONVERT_100UV_TO_VOLTAGE(v_100uv) ((float)v_100uv * V_PER_100UV)
#define CONVERT_VOLTAGE_TO_100UV(v) ((uint16_t)(v * 1E4f))
// This buffer is only used for storing valid cell voltage data
// dump whatever you want in here fr
static uint16_t voltage_regs[NUM_SEGMENTS][CELLS_PER_SEGMENT];
static ExitCode volt_success_buf[NUM_SEGMENTS][VOLTAGE_REGISTER_GROUPS];

// dump whatever you want in here fr
static uint16_t aux_regs[NUM_SEGMENTS][AUX_REGS_PER_SEGMENT];
// TODO find a way to merge these two
static ExitCode temp_success_buf[NUM_SEGMENTS][AUX_REG_GROUPS * 2];
static ExitCode aux_reg_success_buf[NUM_SEGMENTS][AUX_REG_GROUPS];

static StatusRegGroups statuses[NUM_SEGMENTS];
static ExitCode        status_success_buf[NUM_SEGMENTS];

// test setters
void app_segments_broadcastVoltages()
{
    static void (*const cellVoltageSetters[NUM_SEGMENTS][CELLS_PER_SEGMENT])(
        float) = { { app_canTx_BMS_Seg0_Cell0_Voltage_set, app_canTx_BMS_Seg0_Cell1_Voltage_set,
                     app_canTx_BMS_Seg0_Cell2_Voltage_set, app_canTx_BMS_Seg0_Cell3_Voltage_set,
                     app_canTx_BMS_Seg0_Cell4_Voltage_set, app_canTx_BMS_Seg0_Cell5_Voltage_set,
                     app_canTx_BMS_Seg0_Cell6_Voltage_set, app_canTx_BMS_Seg0_Cell7_Voltage_set,
                     app_canTx_BMS_Seg0_Cell8_Voltage_set, app_canTx_BMS_Seg0_Cell9_Voltage_set,
                     app_canTx_BMS_Seg0_Cell10_Voltage_set, app_canTx_BMS_Seg0_Cell11_Voltage_set,
                     app_canTx_BMS_Seg0_Cell12_Voltage_set, app_canTx_BMS_Seg0_Cell13_Voltage_set } };

    // information gathering
    ASSERT_EXIT_OK(io_ltc6813_startCellsAdcConversion(s));
    io_time_delay(10); // TODO tweak timings
    io_ltc6813_readVoltageRegisters(voltage_regs, volt_success_buf);

    // processing
    for (uint8_t segment = 0; segment < NUM_SEGMENTS; segment++)
    {
        for (uint8_t cell = 0; cell < CELLS_PER_SEGMENT; cell++)
        {
            if (IS_EXIT_ERR(volt_success_buf[segment][cell / 3]))
                continue;
            // see page 68, 0xffff is invalid (either not populated or faulted)
            if (voltage_regs[segment][cell] == 0xffff)
            {
                cellVoltageSetters[segment][cell](0xffff);
                volt_success_buf[segment][cell / 3] = EXIT_CODE_ERROR;
                continue;
            }
            cellVoltageSetters[segment][cell](CONVERT_100UV_TO_VOLTAGE(voltage_regs[segment][cell]));
        }
    }
}

void app_segments_broadcastTempsVRef()
{
    static void (*const thermistorSetters[NUM_SEGMENTS][THERMISTORS_PER_SEGMENT])(float) = { { 0 } }; // TODO populate
    // TODO convert at last moment, most always keep as uint16_t
    static float cell_temps[NUM_SEGMENTS][THERMISTORS_PER_SEGMENT];
    memset(segment_vref, 0, NUM_SEGMENTS * sizeof(uint16_t));
    memset(cell_temps, 0, NUM_SEGMENTS * THERMISTORS_PER_SEGMENT * sizeof(float));

    for (uint8_t mux = 0; mux < 2; mux++)
    {
        // information gathering
        // TODO flip a mux based on i == 0 and i == 1
        ASSERT_EXIT_OK(io_ltc6813_startThermistorsAdcConversion(s));
        io_time_delay(10); // TODO tweak timings
        io_ltc6813_readAuxRegisters(aux_regs, aux_reg_success_buf);

        // processing
        for (uint8_t segment = 0U; segment < NUM_SEGMENTS; segment++)
        {
            for (uint8_t aux_gpio = 0U; aux_gpio < AUX_REGS_PER_SEGMENT; aux_gpio++)
            {
                temp_success_buf[segment][aux_gpio / 3 + AUX_REG_GROUPS * mux] =
                    aux_reg_success_buf[segment][aux_gpio / 3];
                if (IS_EXIT_ERR(aux_reg_success_buf[segment][aux_gpio / 3]))
                {
                    continue;
                }
                if (aux_gpio == 5)
                {
                    segment_vref[segment] = aux_regs[segment][aux_gpio];
                    continue;
                }
                const uint8_t adj = aux_gpio >= 6 ? 1 : 0;
                cell_temps[segment][aux_gpio - adj + THERMISTORS_PER_SEGMENT / 2 * mux] =
                    app_thermistor_resistanceToTemp(aux_regs[segment][aux_gpio], &ltc_thermistor_lut);
                // thermistorSetters[segment][therm](cell_temps[segment][therm]);
            }
        }
    }
}

void app_segments_broadcastStatus()
{
    static void (*const muxTestSetters[NUM_SEGMENTS])(bool) = { app_canTx_BMS_Seg0_MUX_OK_set };
    static void (*const vregSetters[NUM_SEGMENTS])(bool)    = { app_canTx_BMS_Seg0_VREG_OK_set };
    static void (*const vregdSetters[NUM_SEGMENTS])(bool)   = { app_canTx_BMS_Seg0_VREGD_OK_set };

    static void (*const revCodeSetters[NUM_SEGMENTS])(uint32_t) = { app_canTx_BMS_Seg0_REV_CODE_set };
    // static void (*const tempSetters[NUM_SEGMENTS])(float)           = { app_canTx_BMS_Seg0_VREGD_OK_set };
    static void (*const thermalOKsetters[NUM_SEGMENTS])(bool) = { app_canTx_BMS_Seg0_THERMAL_OK_set };

    ASSERT_EXIT_OK(io_ltc6813_startInternalADCConversions(s));
    io_time_delay(15);
    // ASSERT_EXIT_OK(io_ltc6813_diagnoseMUX());
    // io_time_delay(5);
    io_ltc6813_getStatus(statuses, status_success_buf);
    for (uint8_t segment = 0; segment < NUM_SEGMENTS; segment++)
    {
        if (IS_EXIT_ERR(status_success_buf[segment]))
            continue;
        revCodeSetters[segment](statuses[segment].REV);

        // tempSetters[segment]((float)statuses[segment].ITMP / 76.0f - 276.0f);

        vregSetters[segment](
            CONVERT_VOLTAGE_TO_100UV(4.5f) <= statuses[segment].VA &&
            statuses[segment].VA <= CONVERT_VOLTAGE_TO_100UV(5.5f));
        vregdSetters[segment](
            CONVERT_VOLTAGE_TO_100UV(2.7f) <= statuses[segment].VD &&
            statuses[segment].VD <= CONVERT_VOLTAGE_TO_100UV(3.6f));

        thermalOKsetters[segment](!statuses[segment].THSD);
        muxTestSetters[segment](!(bool)statuses[segment].MUXFAIL);
    }
}

void app_segments_ADCAccuracyTest()
{
    static void (*const segmentVRefTrueSetters[NUM_SEGMENTS])(bool)         = { app_canTx_BMS_Seg0_VREF_OK_set };
    static void (*const segmentOverlapADC12TestSetters[NUM_SEGMENTS])(bool) = { app_canTx_BMS_Seg0_ADC_1_2_Equal_set };
    static void (*const segmentOverlapADC23TestSetters[NUM_SEGMENTS])(bool) = { app_canTx_BMS_Seg0_ADC_2_3_Equal_set };

    ASSERT_EXIT_OK(io_ltc6813_overlapADCTest(s));
    io_time_delay(10);                                               // TODO tweak timings
    io_ltc6813_readVoltageRegisters(voltage_regs, volt_success_buf); // TODO make this more efficient

    // for debug mode
    for (uint8_t segment = 0; segment < NUM_SEGMENTS; segment++)
    {
        // 6, 7 and 12, 13
        const uint16_t vref_diff    = (uint16_t)abs(segment_vref[segment] - CONVERT_VOLTAGE_TO_100UV(3.0f)),
                       adc_1_2_diff = (uint16_t)abs(voltage_regs[segment][6] - voltage_regs[segment][7]),
                       adc_3_4_diff = (uint16_t)abs(voltage_regs[segment][12] - voltage_regs[segment][13]);
        segmentVRefTrueSetters[segment](vref_diff < CONVERT_VOLTAGE_TO_100UV(0.014f));
        const bool adc_1_2_fail =
            IS_EXIT_ERR(volt_success_buf[segment][0]) || IS_EXIT_ERR(volt_success_buf[segment][1]);
        segmentOverlapADC12TestSetters[segment](!adc_1_2_fail && adc_1_2_diff < CONVERT_VOLTAGE_TO_100UV(0.001f));
        const bool adc_2_3_fail =
            IS_EXIT_ERR(volt_success_buf[segment][1]) || IS_EXIT_ERR(volt_success_buf[segment][2]);
        segmentOverlapADC23TestSetters[segment](!adc_2_3_fail && adc_3_4_diff < CONVERT_VOLTAGE_TO_100UV(0.001f));
    }
}

void app_segments_voltageSelftest()
{
    static void (*const segmentVoltageSelfTestSetters[NUM_SEGMENTS])(
        bool) = { app_canTx_BMS_Seg0_VOLT_REGISTER_OK_set };

    ASSERT_EXIT_OK(io_ltc6813_sendSelfTestVoltages(s));
    io_time_delay(10); // TODO tweak timings
    io_ltc6813_readVoltageRegisters(voltage_regs, volt_success_buf);
    for (uint8_t segment = 0; segment < NUM_SEGMENTS; segment++)
    {
        bool self_test_ok = true;
        for (uint8_t cell = 0; cell < CELLS_PER_SEGMENT; cell++)
        {
            const bool cell_ok = voltage_regs[segment][cell] == io_ltc6813_selfTestExpectedValue(s);
            self_test_ok &= cell_ok;
        }
        segmentVoltageSelfTestSetters[segment](self_test_ok);
    }
}

void app_segments_auxSelftest()
{
    static void (*const segmentAuxRegSelfTestSetters[NUM_SEGMENTS])(bool) = { app_canTx_BMS_Seg0_AUX_REGISTER_OK_set };

    ASSERT_EXIT_OK(io_ltc6813_sendSelfTestAux(s));
    io_time_delay(10); // TODO tweak timings
    io_ltc6813_readAuxRegisters(aux_regs, aux_reg_success_buf);
    for (uint8_t segment = 0; segment < NUM_SEGMENTS; segment++)
    {
        bool self_test_ok = true;
        for (uint8_t aux_reg = 0; self_test_ok && aux_reg < AUX_REGS_PER_SEGMENT; aux_reg++)
        {
            const bool aux_ok = aux_regs[segment][aux_reg] == io_ltc6813_selfTestExpectedValue(s);
            self_test_ok &= aux_ok;
        }
        segmentAuxRegSelfTestSetters[segment](self_test_ok);
    }
}

void app_segments_statusSelftest()
{
    static void (*const segmentStatTestSelfTestSetters[NUM_SEGMENTS])(
        bool) = { app_canTx_BMS_Seg0_STAT_REGISTER_OK_set };

    ASSERT_EXIT_OK(io_ltc6813_sendSelfTestStat(s));
    io_time_delay(10); // TODO tweak timings
    io_ltc6813_getStatus(statuses, status_success_buf);

    for (uint8_t segment = 0; segment < NUM_SEGMENTS; segment++)
    {
        const uint16_t *statuses_buffer = (uint16_t *)&statuses[segment];
        bool            self_test_pass  = true;

        static_assert(sizeof(StatusRegGroups) == sizeof(uint16_t) * 6);
        for (uint8_t word = 0; word < 4; word++) // NOTE: only the first four words we need to check
        {
            self_test_pass &= statuses_buffer[word] == io_ltc6813_selfTestExpectedValue(s);
        }
        segmentStatTestSelfTestSetters[segment](self_test_pass);
    }
}

void app_segments_openWireCheck()
{
    static void (*const cellOWCSetters[NUM_SEGMENTS][CELLS_PER_SEGMENT])(bool) = {
        { app_canTx_BMS_Seg0_Cell0_OWC_OK_set, app_canTx_BMS_Seg0_Cell1_OWC_OK_set, app_canTx_BMS_Seg0_Cell2_OWC_OK_set,
          app_canTx_BMS_Seg0_Cell3_OWC_OK_set, app_canTx_BMS_Seg0_Cell4_OWC_OK_set, app_canTx_BMS_Seg0_Cell5_OWC_OK_set,
          app_canTx_BMS_Seg0_Cell6_OWC_OK_set, app_canTx_BMS_Seg0_Cell7_OWC_OK_set, app_canTx_BMS_Seg0_Cell8_OWC_OK_set,
          app_canTx_BMS_Seg0_Cell9_OWC_OK_set, app_canTx_BMS_Seg0_Cell10_OWC_OK_set,
          app_canTx_BMS_Seg0_Cell11_OWC_OK_set, app_canTx_BMS_Seg0_Cell12_OWC_OK_set,
          app_canTx_BMS_Seg0_Cell13_OWC_OK_set }
    };

    // data collection
    static uint16_t owc_pucv[NUM_SEGMENTS][CELLS_PER_SEGMENT];
    ASSERT_EXIT_OK(io_ltc6813CellVoltages_owcPull(PULL_UP, ADCSpeed_26Hz, false));
    // io_time_delay(201);
    ASSERT_EXIT_OK(io_ltc6813CellVoltages_owcPull(PULL_UP, ADCSpeed_26Hz, false));
    // io_time_delay(201);
    ASSERT_EXIT_OK(io_ltc6813_startCellsAdcConversion(ADCSpeed_26Hz));
    io_time_delay(201);
    io_ltc6813_readVoltageRegisters(owc_pucv, volt_success_buf);

    // reset back to normal
    static uint16_t owc_pdcv[NUM_SEGMENTS][CELLS_PER_SEGMENT];
    ASSERT_EXIT_OK(io_ltc6813CellVoltages_owcPull(PULL_DOWN, ADCSpeed_26Hz, false));
    // io_time_delay(201);
    ASSERT_EXIT_OK(io_ltc6813CellVoltages_owcPull(PULL_DOWN, ADCSpeed_26Hz, false));
    // io_time_delay(201);
    ASSERT_EXIT_OK(io_ltc6813_startCellsAdcConversion(ADCSpeed_26Hz));
    io_time_delay(201);
    io_ltc6813_readVoltageRegisters(owc_pdcv, volt_success_buf);

    // perform the check
    for (uint8_t segment = 0; segment < NUM_SEGMENTS; segment++)
    {
        cellOWCSetters[segment][0](owc_pucv[segment][0] != 0);
        for (uint8_t cell = 1; cell < CELLS_PER_SEGMENT; cell++)
        {
            const bool owc_passing = owc_pdcv[segment][cell] - owc_pucv[segment][cell] > CONVERT_VOLTAGE_TO_100UV(0.4f);
            cellOWCSetters[segment][cell](owc_passing);
        }
#if CELLS_PER_SEGMENT >= 18
        completeness cellOWCSetters[segment][17](owc_pdcv[segment][cell] == 0.0f);
#endif
    }
}