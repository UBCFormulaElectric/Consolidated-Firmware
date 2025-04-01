#include "io_ltc6813.h"

#include "app_utils.h"
#include "hw_spis.h"

#include <assert.h>
#include <io_log.h>
#include <string.h>

static uint16_t calculate_pec15(const uint8_t *data, const uint8_t len)
{
    // TODO ensure this works with little endian data (i'm fairly certain it does not)
    static const uint16_t pec15_lut[256] = {
        0x0,    0xC599, 0xCEAB, 0xB32,  0xD8CF, 0x1D56, 0x1664, 0xD3FD, 0xF407, 0x319E, 0x3AAC, 0xFF35, 0x2CC8, 0xE951,
        0xE263, 0x27FA, 0xAD97, 0x680E, 0x633C, 0xA6A5, 0x7558, 0xB0C1, 0xBBF3, 0x7E6A, 0x5990, 0x9C09, 0x973B, 0x52A2,
        0x815F, 0x44C6, 0x4FF4, 0x8A6D, 0x5B2E, 0x9EB7, 0x9585, 0x501C, 0x83E1, 0x4678, 0x4D4A, 0x88D3, 0xAF29, 0x6AB0,
        0x6182, 0xA41B, 0x77E6, 0xB27F, 0xB94D, 0x7CD4, 0xF6B9, 0x3320, 0x3812, 0xFD8B, 0x2E76, 0xEBEF, 0xE0DD, 0x2544,
        0x2BE,  0xC727, 0xCC15, 0x98C,  0xDA71, 0x1FE8, 0x14DA, 0xD143, 0xF3C5, 0x365C, 0x3D6E, 0xF8F7, 0x2B0A, 0xEE93,
        0xE5A1, 0x2038, 0x7C2,  0xC25B, 0xC969, 0xCF0,  0xDF0D, 0x1A94, 0x11A6, 0xD43F, 0x5E52, 0x9BCB, 0x90F9, 0x5560,
        0x869D, 0x4304, 0x4836, 0x8DAF, 0xAA55, 0x6FCC, 0x64FE, 0xA167, 0x729A, 0xB703, 0xBC31, 0x79A8, 0xA8EB, 0x6D72,
        0x6640, 0xA3D9, 0x7024, 0xB5BD, 0xBE8F, 0x7B16, 0x5CEC, 0x9975, 0x9247, 0x57DE, 0x8423, 0x41BA, 0x4A88, 0x8F11,
        0x57C,  0xC0E5, 0xCBD7, 0xE4E,  0xDDB3, 0x182A, 0x1318, 0xD681, 0xF17B, 0x34E2, 0x3FD0, 0xFA49, 0x29B4, 0xEC2D,
        0xE71F, 0x2286, 0xA213, 0x678A, 0x6CB8, 0xA921, 0x7ADC, 0xBF45, 0xB477, 0x71EE, 0x5614, 0x938D, 0x98BF, 0x5D26,
        0x8EDB, 0x4B42, 0x4070, 0x85E9, 0xF84,  0xCA1D, 0xC12F, 0x4B6,  0xD74B, 0x12D2, 0x19E0, 0xDC79, 0xFB83, 0x3E1A,
        0x3528, 0xF0B1, 0x234C, 0xE6D5, 0xEDE7, 0x287E, 0xF93D, 0x3CA4, 0x3796, 0xF20F, 0x21F2, 0xE46B, 0xEF59, 0x2AC0,
        0xD3A,  0xC8A3, 0xC391, 0x608,  0xD5F5, 0x106C, 0x1B5E, 0xDEC7, 0x54AA, 0x9133, 0x9A01, 0x5F98, 0x8C65, 0x49FC,
        0x42CE, 0x8757, 0xA0AD, 0x6534, 0x6E06, 0xAB9F, 0x7862, 0xBDFB, 0xB6C9, 0x7350, 0x51D6, 0x944F, 0x9F7D, 0x5AE4,
        0x8919, 0x4C80, 0x47B2, 0x822B, 0xA5D1, 0x6048, 0x6B7A, 0xAEE3, 0x7D1E, 0xB887, 0xB3B5, 0x762C, 0xFC41, 0x39D8,
        0x32EA, 0xF773, 0x248E, 0xE117, 0xEA25, 0x2FBC, 0x846,  0xCDDF, 0xC6ED, 0x374,  0xD089, 0x1510, 0x1E22, 0xDBBB,
        0xAF8,  0xCF61, 0xC453, 0x1CA,  0xD237, 0x17AE, 0x1C9C, 0xD905, 0xFEFF, 0x3B66, 0x3054, 0xF5CD, 0x2630, 0xE3A9,
        0xE89B, 0x2D02, 0xA76F, 0x62F6, 0x69C4, 0xAC5D, 0x7fA0, 0xBA39, 0xB10B, 0x7492, 0x5368, 0x96F1, 0x9DC3, 0x585A,
        0x8BA7, 0x4E3E, 0x450C, 0x8095
    };

    // Initialize the value of the PEC15 remainder to 16
    uint16_t remainder = 16U;
    uint8_t  index     = 0U;

    // Refer to PEC15 calculation in the 'PEC Calculation' of the LTC6813
    // datasheet
    for (size_t i = 0U; i < len; i++)
    {
        index     = ((uint8_t)(remainder >> 7U) ^ data[i]) & 0xFF;
        remainder = (uint16_t)(remainder << 8U ^ pec15_lut[index]);
    }

    // Set the LSB of the PEC15 remainder to 0.
    return (uint16_t)(remainder << 1); // TODO make sure the shifting to load into the registers is correct
}

// we wrote it this way to make hide the little endian processor storage of the struct
// this way, the mental model is that the data is stored in the order it is written (big endian)
// I know about scalar_storage_order("big-endian") but you cannot get the address a struct with that attribute
// that is problematic for pec15 finding
// see table 30
typedef struct __attribute__((__packed__))
{
    uint8_t pec_0;
    uint8_t pec_1;
} raw_pec;

static raw_pec build_tx_pec(const uint8_t *data, const uint8_t len)
{
    const uint16_t pec = calculate_pec15(data, len);
    raw_pec        out = { 0 };
    out.pec_0          = (uint8_t)(pec >> 8);
    out.pec_1          = (uint8_t)pec;
    return out;
}

static uint16_t read_rx_pec(const raw_pec *pec)
{
    return (uint16_t)(pec->pec_0 << 8 | pec->pec_1);
}

// see above about the struct nonsense
// see table 35
typedef struct __attribute__((__packed__))
{
    uint8_t cmd_0;
    uint8_t cmd_1;
    raw_pec pec;
} raw_cmd;

static raw_cmd build_tx_cmd(const uint16_t command)
{
    raw_cmd out = { 0 };
    out.cmd_0   = (uint8_t)(command >> 8);
    out.cmd_1   = (uint8_t)command;
    out.pec     = build_tx_pec((uint8_t *)&out, sizeof(out.cmd_0) + sizeof(out.cmd_1));
    return out;
}

// as per table 38
typedef struct __attribute__((__packed__))
{
    // byte 1
    uint8_t adcopt : 1;
    uint8_t dten : 1;
    uint8_t refon : 1;
    uint8_t gpio_1_5 : 5;
    // byte 2
    uint8_t vuv_0_7;
    // byte 3
    uint8_t vuv_8_11 : 4;
    uint8_t vov_0_3 : 4;
    // byte 4
    uint8_t vov_4_11;
    // byte 5
    uint8_t dcc_1_8;
    // byte 6
    uint8_t dcc_9_12 : 4;
    uint8_t dcto : 4;
    // byte 7/8
    raw_pec pec;
} CFGAR;

// as per table 39
typedef struct __attribute__((__packed__))
{
    // byte 1
    uint8_t gpio_6_9 : 4;
    uint8_t dcc_13_16 : 4;
    // byte 2
    uint8_t dcc_17 : 1;
    uint8_t dcc_18 : 1;
    uint8_t dcc_0 : 1;
    uint8_t dtmen : 1;
    uint8_t ps : 2;
    uint8_t fdrf : 1;
    uint8_t mute : 1;
    // byte 3-6
    uint32_t reserved;
    // byte 7/8
    raw_pec pec;
} CFGBR;

/**
 * @param balance_config Configuration for the balancing
 * @attention For more information on how to configure the LTC, see table 56
 * @return Success if both succeeded. Fail if at least one failed.
 */
bool io_ltc6813_writeConfigurationRegisters(bool balance_config[NUM_SEGMENTS][CELLS_PER_SEGMENT])
{
    // as per table 33
    struct __attribute__((__packed__))
    {
        raw_cmd cmd;
        CFGAR   segment_configs[NUM_SEGMENTS]; // note these must be shifted in backwards (shift register style)
    } tx_msg_a = { 0 };
    static_assert(sizeof(tx_msg_a) == 4 + 8 * NUM_SEGMENTS);

    // as per table 33`
    struct __attribute__((__packed__))
    {
        raw_cmd cmd;
        CFGBR   segment_configs[NUM_SEGMENTS];
    } tx_msg_b = { 0 };
    static_assert(sizeof(tx_msg_b) == 4 + 8 * NUM_SEGMENTS);

    // just in case
    memset(&tx_msg_a, 0, sizeof(tx_msg_a));
    memset(&tx_msg_b, 0, sizeof(tx_msg_b));

#define WRCFGA (0x0001)
#define WRCFGB (0x0024)
    tx_msg_a.cmd = build_tx_cmd(WRCFGA);
    tx_msg_b.cmd = build_tx_cmd(WRCFGB);

    for (uint8_t curr_segment = 0U; curr_segment < NUM_SEGMENTS; curr_segment++)
    {
        // Data used to configure the last segment on the daisy chain needs to be sent first
        const uint8_t tx_cfg_idx = NUM_SEGMENTS - curr_segment - 1;
        CFGAR *const  seg_a      = &tx_msg_a.segment_configs[tx_cfg_idx];
        CFGBR *const  seg_b      = &tx_msg_b.segment_configs[tx_cfg_idx];

        seg_a->gpio_1_5 = 0x1F;
        seg_b->gpio_6_9 = 0xF;
        seg_a->refon    = 0;
        seg_a->dten     = 0;
        seg_a->adcopt   = 1;

#define VUV (0x4E1U) // Under-voltage comparison voltage, (VUV + 1) * 16 * 100uV
        seg_a->vuv_0_7  = VUV & 0xFF;
        seg_a->vuv_8_11 = VUV >> 8 & 0xF;
#define VOV (0x8CAU) // Over-voltage comparison voltage, VOV * 16 * 100uV
        seg_a->vov_0_3  = VOV & 0xF;
        seg_a->vov_4_11 = VOV >> 4 & 0xFF;

        // Write to configuration registers DCC bits
        if (balance_config != NULL)
        {
            uint32_t dcc_bits = 0U;
            // Get dcc bits to write for the current segment (which cells to balance)
            for (uint8_t cell = 0; cell < CELLS_PER_SEGMENT; cell++)
            {
                dcc_bits |= (uint32_t)(balance_config[curr_segment][cell] << cell);
            }
            seg_b->dcc_0     = 0;
            seg_a->dcc_1_8   = dcc_bits & 0xFF;
            seg_a->dcc_9_12  = dcc_bits >> 8 & 0xF;
            seg_b->dcc_13_16 = dcc_bits >> 12 & 0xF;
            seg_b->dcc_17    = 0;
            seg_b->dcc_18    = 0;
        }
        else
        {
            seg_b->dcc_0     = 0;
            seg_a->dcc_1_8   = 0xff;
            seg_a->dcc_9_12  = 0;
            seg_b->dcc_13_16 = 0;
            seg_b->dcc_17    = 0;
            seg_b->dcc_18    = 0;
        }

        // Calculate and pack the PEC15 bytes into data to write to the configuration register
        seg_a->pec = build_tx_pec((uint8_t *)seg_a, sizeof(CFGAR) - sizeof(raw_pec));
        seg_b->pec = build_tx_pec((uint8_t *)seg_b, sizeof(CFGBR) - sizeof(raw_pec));
    }
    // Write to configuration registers
    if (!hw_spi_transmit(&ltc6813_spi, (uint8_t *)&tx_msg_a, sizeof(tx_msg_a)))
        return false;
    if (!hw_spi_transmit(&ltc6813_spi, (uint8_t *)&tx_msg_b, sizeof(tx_msg_b)))
        return false;
    return true;
}

bool io_ltc6813_readConfigurationRegisters()
{
#define RDCFGA (0x0002)
#define RDCFGB (0x0026)
    raw_cmd tx_msg_a = build_tx_cmd(RDCFGA);
    CFGAR   rx_buf_a[NUM_SEGMENTS];
    if (!hw_spi_transmitThenReceive(
            &ltc6813_spi, (uint8_t *)&tx_msg_a, sizeof(tx_msg_a), (uint8_t *)&rx_buf_a, sizeof(rx_buf_a)))
    {
        return false;
    }

    for (uint8_t curr_segment = 0U; curr_segment < NUM_SEGMENTS; curr_segment++)
    {
        const uint16_t calc_pec15 = calculate_pec15((uint8_t *)&rx_buf_a[curr_segment], 6);
        const uint16_t recv_pec15 = read_rx_pec(&rx_buf_a[curr_segment].pec);
        assert(calc_pec15 == recv_pec15);
    }

    raw_cmd tx_msg_b = build_tx_cmd(RDCFGA);
    CFGBR   rx_buf_b[NUM_SEGMENTS];
    if (!hw_spi_transmitThenReceive(
            &ltc6813_spi, (uint8_t *)&tx_msg_b, sizeof(tx_msg_b), (uint8_t *)&rx_buf_b, sizeof(rx_buf_b)))
    {
        return false;
    }

    for (uint8_t curr_segment = 0U; curr_segment < NUM_SEGMENTS; curr_segment++)
    {
        const uint16_t calc_pec15 = calculate_pec15((uint8_t *)&rx_buf_b[curr_segment], 6);
        const uint16_t recv_pec15 = read_rx_pec(&rx_buf_b[curr_segment].pec);
        assert(calc_pec15 == recv_pec15);
    }

    return true;
}

bool io_ltc6813_sendCommand(const uint16_t command)
{
    const raw_cmd tx_cmd = build_tx_cmd(command);
    return hw_spi_transmit(&ltc6813_spi, (uint8_t *)&tx_cmd, sizeof(tx_cmd));
}

// VOLTAGE PARSING SEGMENT

// as per table 40-45
typedef struct __attribute__((__packed__))
{
    uint16_t a;
    uint16_t b;
    uint16_t c;
    raw_pec  pec;
} VoltageRegisterGroup;
static_assert(sizeof(VoltageRegisterGroup) == 8);

/**
 * This functions works by iterating through all register groups, and for each register group asking each segment
 * what is the value of the register group in that segment
 */
void io_ltc6813_readVoltages(
    float cell_voltages[NUM_SEGMENTS][CELLS_PER_SEGMENT],
    bool  success[NUM_SEGMENTS][VOLTAGE_REGISTER_GROUPS])
{
    memset(success, false, NUM_SEGMENTS * VOLTAGE_REGISTER_GROUPS);
    memset(cell_voltages, 0, NUM_SEGMENTS * CELLS_PER_SEGMENT * sizeof(float));
    // Exit early if ADC conversion fails
    if (!io_ltc6813_pollAdcConversions())
    {
        return;
    }

    for (uint8_t curr_reg_group = 0U; curr_reg_group < VOLTAGE_REGISTER_GROUPS; curr_reg_group++)
    {
        // maps the register group number to the command to read that register group
#define RDCVA (0x04U)
#define RDCVB (0x06U)
#define RDCVC (0x08U)
#define RDCVD (0x0AU)
#define RDCVE (0x09U)
#define RDCVF (0x0BU)
        static const uint16_t cv_read_cmds[VOLTAGE_REGISTER_GROUPS] = { RDCVA, RDCVB, RDCVC, RDCVD, RDCVE, RDCVF };

        const raw_cmd tx_cmd = build_tx_cmd(cv_read_cmds[curr_reg_group]);

        // Transmit the command and receive data stored in register group.
        VoltageRegisterGroup rx_buffer[NUM_SEGMENTS] = { 0 };

        const bool voltage_read_success = hw_spi_transmitThenReceive(
            &ltc6813_spi, (uint8_t *)&tx_cmd, sizeof(tx_cmd), (uint8_t *)rx_buffer, sizeof(rx_buffer));
        if (!voltage_read_success)
        {
            continue;
        }

        for (uint8_t curr_segment = 0U; curr_segment < NUM_SEGMENTS; curr_segment++)
        {
            // Calculate PEC15 from the data received on rx_buffer
            const uint16_t calc_pec15 = calculate_pec15((uint8_t *)&rx_buffer[curr_segment], 6);
            const uint16_t recv_pec15 = read_rx_pec(&rx_buffer[curr_segment].pec);
            if (recv_pec15 != calc_pec15)
            {
                continue;
            }

            // fuck it we already here
            success[curr_segment][curr_reg_group] = true;

            // Conversion factor used to convert raw voltages (100µV) to voltages (V)
#define V_PER_100UV (1E-4f)
#define CONVERT_100UV_TO_VOLTAGE(v_100uv) ((float)v_100uv * V_PER_100UV)
            cell_voltages[curr_segment][curr_reg_group * 3 + 0] = CONVERT_100UV_TO_VOLTAGE(rx_buffer[curr_segment].a);
            // only read first cell for group F (cell 16)
            if (curr_reg_group == 5)
            {
                continue;
            }
            cell_voltages[curr_segment][curr_reg_group * 3 + 1] = CONVERT_100UV_TO_VOLTAGE(rx_buffer[curr_segment].b);
            cell_voltages[curr_segment][curr_reg_group * 3 + 2] = CONVERT_100UV_TO_VOLTAGE(rx_buffer[curr_segment].c);
        }
    }
}

// TEMPERATURE PARSING SEGMENT

typedef struct __attribute__((__packed__))
{
    uint16_t a;
    uint16_t b;
    uint16_t c;
    raw_pec  pec;
} AuxRegGroup;
static_assert(sizeof(AuxRegGroup) == 8);

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

void io_ltc6813_readTemperatures(
    float cell_temps[NUM_SEGMENTS][THERMISTORS_PER_SEGMENT],
    bool  success[NUM_SEGMENTS][THERMISTOR_REGISTER_GROUPS])
{
    memset(success, false, NUM_SEGMENTS * THERMISTOR_REGISTER_GROUPS);
    memset(cell_temps, 0, NUM_SEGMENTS * THERMISTORS_PER_SEGMENT * sizeof(float));
    if (!io_ltc6813_pollAdcConversions())
    {
        return;
    }

    // Read thermistor voltages stored in the AUX register groups
    for (uint8_t reg_group = 0U; reg_group < THERMISTOR_REGISTER_GROUPS; reg_group++)
    {
        // pack command
#define RDAUXA (0x000CU)
#define RDAUXB (0x000EU)
#define RDAUXC (0x000DU)
        static const uint16_t aux_reg_group_cmds[3] = { RDAUXA, RDAUXB, RDAUXC };
        const raw_cmd         tx_cmd                = build_tx_cmd(aux_reg_group_cmds[reg_group]);

        // as per table 46-48
        AuxRegGroup rx_buffer[NUM_SEGMENTS] = { 0 };
        static_assert(sizeof(rx_buffer[0]) == 8);

        // send command and receive data
        if (!hw_spi_transmitThenReceive(
                &ltc6813_spi, (uint8_t *)&tx_cmd, sizeof(tx_cmd), (uint8_t *)rx_buffer, sizeof(rx_buffer)))
        {
            // memset(success, false, NUM_SEGMENTS * THERMISTOR_REGISTER_GROUPS);
            for (int i = 0; i < NUM_SEGMENTS; i++)
            {
                success[i][reg_group] = false;
            }
            continue;
        }

        // process data
        for (uint8_t seg_idx = 0U; seg_idx < NUM_SEGMENTS; seg_idx++)
        {
            // look for data for the current segment from the back
            const AuxRegGroup *seg_reg_group = &rx_buffer[(NUM_SEGMENTS - 1) - seg_idx];

            const uint16_t calc_pec15 = calculate_pec15((uint8_t *)seg_reg_group, 6);
            const uint16_t recv_pec15 = read_rx_pec(&seg_reg_group->pec);
            if (recv_pec15 != calc_pec15)
            {
                success[seg_idx][reg_group] = false;
                continue;
            }

            // since we are ignoring REF variable, we need to offset all further readings by 1 backwards
            const int8_t adj                             = reg_group > 1 ? -1 : 0;
            success[seg_idx][reg_group]                  = true;
            cell_temps[seg_idx][reg_group * 3 + 0 + adj] = calculateThermistorTempDeciDegC(seg_reg_group->a);
            cell_temps[seg_idx][reg_group * 3 + 1 + adj] = calculateThermistorTempDeciDegC(seg_reg_group->b);
            cell_temps[seg_idx][reg_group * 3 + 2 + adj] = calculateThermistorTempDeciDegC(seg_reg_group->c);
        }
    }
}

bool io_ltc6813_startCellsAdcConversion(void)
{
// ADC mode selection
#define MD (01U)
// Cell selection for ADC conversion
#define CH (000U)
// Discharge permitted
#define DCP (0U)
#define ADCV (0x260 | CH | DCP << 4 | MD << 7)
    return io_ltc6813_sendCommand(ADCV);
}

bool io_ltc6813_startThermistorsAdcConversion(void)
{
// ADC mode selection
#define MD (01U)
// GPIO Selection for ADC conversion
#define CHG (000U)
#define ADAX (0x460 | MD << 7 | CHG)
    return io_ltc6813_sendCommand(ADAX);
}

#define MAX_NUM_ADC_COMPLETE_CHECKS 10U
bool io_ltc6813_pollAdcConversions(void)
{
    // Prepare command to get the status of ADC conversions
#define PLADC (0x0714U)
#define ADC_CONV_INCOMPLETE (0xFF)
    const raw_cmd tx_cmd = build_tx_cmd(PLADC);
    for (uint8_t num_attempts = 0U; num_attempts < MAX_NUM_ADC_COMPLETE_CHECKS; num_attempts++)
    {
        uint8_t rx_data;
        if (!hw_spi_transmitThenReceive(&ltc6813_spi, (uint8_t *)&tx_cmd, sizeof(tx_cmd), &rx_data, sizeof(rx_data)))
        {
            return false;
        }
        if (rx_data != ADC_CONV_INCOMPLETE)
        {
            return true;
        }
    }
    return false;
}

bool io_ltc6813_sendBalanceCommand(void)
{
#define UNMUTE (0x0029U)
    return io_ltc6813_sendCommand(UNMUTE);
}

bool io_ltc6813_sendStopBalanceCommand(void)
{
#define MUTE (0x0028U)
    return io_ltc6813_sendCommand(MUTE);
}

bool io_ltc6813CellVoltages_owc(const PullDirection pull_direction)
{
// ADOW mode selection
#define PUP_PU (1U) // Pull-up current
#define PUP_PD (0U) // Pull-down current

// Discharge permitted
#define DCP (0U)

// Cell selection for ADC conversion
#define CH_OWC (0U)

// TODO make backwards
#define ADOW_PU_FIL ((uint16_t)(((0x028U + (MD << 7U) + (PUP_PU << 6U) + (DCP << 4U) + CH_OWC) << 8U) | 0x0003U))
#define ADOW_PD_FIL ((uint16_t)(((0x028U + (MD << 7U) + (PUP_PD << 6U) + (DCP << 4U) + CH_OWC) << 8U) | 0x0003U))

    return pull_direction == PULL_UP ? io_ltc6813_sendCommand(ADOW_PU_FIL) : io_ltc6813_sendCommand(ADOW_PD_FIL);
}
