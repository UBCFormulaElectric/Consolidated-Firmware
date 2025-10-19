#include "utils.h"
#include "io_ltc6813.h"

#include "io_ltc6813_internal.h"
#include "hw_spis.h"

#include <assert.h>
#include <io_log.h>
#include <string.h>

#define V_PER_100UV (1E-4f)

// TODO assert that for each speed that the ADCOPT is correct
ExitCode io_ltc6813_startThermistorsAdcConversion(const ADCSpeed speed)
{
#define CLRAUX (0x0712)
    RETURN_IF_ERR(io_ltc6813_sendCommand(CLRAUX));

    const uint16_t adc_speed_factor = (speed & 0x3) << 7;
// GPIO Selection for ADC conversion
#define CHG (0x000U)
#define ADAX (0x460 | adc_speed_factor | CHG)
    return io_ltc6813_sendCommand(ADAX);
}

/**
 * Calculate thermistor temperature
 * @param raw_thermistor_voltage The voltage measured across the thermistor
 * @return The thermistor temp in deci degC
 */
static float calculateThermistorTempDeciDegC(const uint16_t raw_thermistor_voltage)
{
    // The following configuration is now the thermistor temperature is
    // calculated
    //
    // - VREF is a ~3.0V provided by the LTC6813
    // - The 10kOhm resistor on top is the bias resistor used to help compute
    // the resistance across the thermistor
    //
    // (1) The thermistor resistance is first determined from the voltage drop
    // (VTemp - V-) across the thermistor (2) Using the thermistor resistance we
    // can use the look up table provided to get the temperature of the
    // thermistor
    //
    //
    //       ┌─── VREF
    //       │
    //      ┌┴┐
    //      │ │
    //      │ │ 10kOhm
    //      │ │
    //      └┬┘
    //       │----- VTemp
    //      ┌┴┐
    //      │ │
    //      │ │ NTCALUG03A103G thermistor
    //      │ │
    //      └┬┘
    //       │
    //       └─── V-
    //
    //

    // A 0-100°C temperature reverse lookup table with 0.5°C resolution for a Vishay
    // NTCS0603E3103JMT thermistor. The 0th index represents 0°C. Incrementing the
    // index represents a 0.5°C increase in temperature.
    static const float temp_resistance_lut[201] = {
        29239.8f, 28571.6f, 27920.7f, 27286.6f, 26668.8f, 26066.9f, 25480.3f, 24908.8f, 24351.8f, 23808.9f, 23279.8f,
        22764.0f, 22261.2f, 21771.0f, 21293.0f, 20827.0f, 20372.5f, 19929.4f, 19497.2f, 19075.6f, 18664.4f, 18263.3f,
        17872.0f, 17490.3f, 17117.8f, 16754.3f, 16399.7f, 16053.6f, 15715.8f, 15386.1f, 15064.3f, 14750.1f, 14443.4f,
        14144.0f, 13851.7f, 13566.3f, 13287.5f, 13015.3f, 12749.5f, 12489.9f, 12236.3f, 11988.6f, 11746.6f, 11510.2f,
        11279.2f, 11053.5f, 10833.0f, 10617.5f, 10407.0f, 10201.2f, 10000.0f, 9803.4f,  9611.2f,  9423.3f,  9239.7f,
        9060.1f,  8884.5f,  8712.9f,  8545.0f,  8380.8f,  8220.3f,  8063.3f,  7909.7f,  7759.4f,  7612.5f,  7468.7f,
        7328.1f,  7190.5f,  7055.8f,  6924.1f,  6795.2f,  6669.0f,  6545.5f,  6424.7f,  6306.4f,  6190.7f,  6077.3f,
        5966.4f,  5857.8f,  5751.4f,  5647.3f,  5545.4f,  5445.5f,  5347.8f,  5252.1f,  5158.3f,  5066.5f,  4976.5f,
        4888.4f,  4802.1f,  4717.5f,  4634.7f,  4553.5f,  4474.0f,  4396.1f,  4319.8f,  4244.9f,  4171.6f,  4099.8f,
        4029.3f,  3960.3f,  3892.6f,  3826.3f,  3761.3f,  3697.5f,  3635.0f,  3573.8f,  3513.7f,  3454.8f,  3397.0f,
        3340.4f,  3284.8f,  3230.3f,  3176.9f,  3124.5f,  3073.1f,  3022.7f,  2973.2f,  2924.7f,  2877.0f,  2830.3f,
        2784.5f,  2739.5f,  2695.4f,  2652.1f,  2609.6f,  2567.9f,  2526.9f,  2486.8f,  2447.4f,  2408.6f,  2370.6f,
        2333.4f,  2296.7f,  2260.8f,  2225.5f,  2190.9f,  2156.8f,  2123.4f,  2090.6f,  2058.4f,  2026.8f,  1995.7f,
        1965.2f,  1935.2f,  1905.8f,  1876.9f,  1848.5f,  1820.6f,  1793.2f,  1766.2f,  1739.8f,  1713.8f,  1688.2f,
        1663.1f,  1638.5f,  1614.2f,  1590.4f,  1567.0f,  1544.0f,  1521.4f,  1499.2f,  1477.3f,  1455.9f,  1434.8f,
        1414.0f,  1393.6f,  1373.6f,  1353.9f,  1334.5f,  1315.4f,  1296.7f,  1278.3f,  1260.2f,  1242.3f,  1224.8f,
        1207.6f,  1190.6f,  1174.0f,  1157.6f,  1141.4f,  1125.6f,  1110.0f,  1094.6f,  1079.5f,  1064.7f,  1050.1f,
        1035.7f,  1021.5f,  1007.6f,  993.9f,   980.4f,   967.2f,   954.1f,   941.3f,   928.6f,   916.2f,   903.9f,
        891.9f,   880.0f,   868.4f
    };
#define BIAS_RESISTOR_OHM (10000.0f)
#define REFERENCE_VOLTAGE (3.0f)
#define THERM_INDEX_TO_DEGC (0.5f)

    const float gpio_voltage          = (float)raw_thermistor_voltage * V_PER_100UV;
    const float thermistor_resistance = (gpio_voltage * BIAS_RESISTOR_OHM) / (REFERENCE_VOLTAGE - gpio_voltage);

    // Check that the thermistor resistance is in range
    if (!(thermistor_resistance <= temp_resistance_lut[0] && thermistor_resistance >= temp_resistance_lut[201 - 1U]))
    {
        return UINT16_MAX;
    }

    // Find the index corresponding to the calculated thermistor resistance
    // TODO binary search
    uint8_t therm_lut_index = 0U;
    for (therm_lut_index = 0U; thermistor_resistance < temp_resistance_lut[therm_lut_index]; therm_lut_index++)
        ;

    // Divide the index of the thermistor lookup table by 2 as the
    // temperature lookup table's key has a resolution of 0.5°C.
    // Multiply the result by 5 as we are storing the temperature
    // as in deci °C.
    //
    //                                 THERMISTOR LUT INDEX * 10
    // CELL_TEMPERATURES_DECI_DEG_C = ----------------------------
    //                                            2
    //
    return (float)therm_lut_index * THERM_INDEX_TO_DEGC;
}

// as per table 46-48
typedef struct __attribute__((__packed__))
{
    uint16_t a;
    uint16_t b;
    uint16_t c;
    PEC      pec;
} AuxRegGroup;
static_assert(sizeof(AuxRegGroup) == REGISTER_GROUP_SIZE + PEC_SIZE);

void io_ltc6813_readAuxRegisters(
    uint16_t aux_regs[NUM_SEGMENTS][AUX_REGS_PER_SEGMENT],
    bool     comm_success[NUM_SEGMENTS][AUX_REGISTER_GROUPS])
{
    memset(comm_success, false, NUM_SEGMENTS * AUX_REGISTER_GROUPS);
    memset(aux_regs, 0, NUM_SEGMENTS * AUX_REGS_PER_SEGMENT * sizeof(uint16_t));
    if (IS_EXIT_ERR(io_ltc6813_pollAdcConversions()))
    {
        return;
    }

    // Read thermistor voltages stored in the AUX register groups
    for (uint8_t reg_group = 0U; reg_group < AUX_REGISTER_GROUPS; reg_group++)
    {
        // pack command
#define RDAUXA (0x000CU)
#define RDAUXB (0x000EU)
#define RDAUXC (0x000DU)
        static const uint16_t aux_reg_group_cmds[3] = { RDAUXA, RDAUXB, RDAUXC };
        const ltc6813_tx      tx_cmd                = io_ltc6813_build_tx_cmd(aux_reg_group_cmds[reg_group]);

        AuxRegGroup rx_buffer[NUM_SEGMENTS];

        // send command and receive data
        if (IS_EXIT_ERR(hw_spi_transmitThenReceive(
                &ltc6813_spi, (uint8_t *)&tx_cmd, sizeof(tx_cmd), (uint8_t *)rx_buffer, sizeof(rx_buffer))))
        {
            continue;
        }

        // process data
        for (uint8_t seg_idx = 0U; seg_idx < NUM_SEGMENTS; seg_idx++)
        {
            // look for data for the current segment from the back
            const AuxRegGroup *seg_reg_group = &rx_buffer[(NUM_SEGMENTS - 1) - seg_idx];
            if (!io_ltc6813_check_pec((uint8_t *)seg_reg_group, 6, &seg_reg_group->pec))
            {
                continue;
            }

            // since we are ignoring REF variable, we need to offset all further readings by 1 backwards
            comm_success[seg_idx][reg_group]     = true;
            aux_regs[seg_idx][reg_group * 3 + 0] = seg_reg_group->a;
            aux_regs[seg_idx][reg_group * 3 + 1] = seg_reg_group->b;
            aux_regs[seg_idx][reg_group * 3 + 2] = seg_reg_group->c;
        }
    }
}

void io_ltc6813_readTemperatures(
    float  cell_temps[NUM_SEGMENTS][THERMISTORS_PER_SEGMENT],
    float *vref,
    bool   success[NUM_SEGMENTS][AUX_REGISTER_GROUPS])
{
    memset(cell_temps, 0, NUM_SEGMENTS * THERMISTORS_PER_SEGMENT * sizeof(float));
    *vref = 0;

    uint16_t aux_regs[NUM_SEGMENTS][AUX_REGS_PER_SEGMENT];
    io_ltc6813_readAuxRegisters(aux_regs, success);

    for (uint8_t segment = 0U; segment < NUM_SEGMENTS; segment++)
    {
        for (uint8_t aux_gpio = 0U; aux_gpio < THERMISTORS_PER_SEGMENT; aux_gpio++)
        {
            if (!success[segment][aux_gpio / 3])
            {
                continue;
            }
            if (aux_gpio == 5)
            {
                *vref = (float)aux_regs[segment][aux_gpio] * V_PER_100UV;
                continue;
            }
            const int8_t adj                    = aux_gpio >= 6 ? -1 : 0;
            cell_temps[segment][aux_gpio - adj] = calculateThermistorTempDeciDegC(aux_regs[segment][aux_gpio]);
        }
    }
}
