#include <string.h>
#include "hw_spi.h"
#include "ltc6813/io_ltc6813Shared.h"
#include "app_accumulator.h"

// clang-format off

#define NOMINAL_TEMPERATURE            (200U)
#define NUM_OF_THERMISTORS_PER_SEGMENT (8U)
#define TOTAL_NUM_OF_THERMISTORS       (NUM_OF_THERMISTORS_PER_SEGMENT * ACCUMULATOR_NUM_SEGMENTS)

#define IS_CELL_TEMP_READING(curr_reg_group, curr_reading) (((curr_reg_group) != AUX_REGISTER_GROUP_B) || ((curr_reading) != REG_GROUP_READING_2))

// Commands used to read from auxiliary register groups A-C
#define RDAUXA (0x0C00U)
#define RDAUXB (0x0E00U)
#define RDAUXC (0x0D00U)

// Command used to trigger ADC conversions for LTC6813 GPIO inputs
#define ADAX ((uint16_t)((((MD << 7U) + 0x0060U + CHG) << 8U) | 0x0004U))

#define SIZE_OF_TEMPERATURE_LUT (201U)
#define BIAS_RESISTOR_OHM       (10000.0f)
#define REFERENCE_VOLTAGE       (3.0f)
#define THERM_INDEX_TO_DEGC     (5U)

// Conversion factor to convert deci-degrees celcius to degrees celcius
#define DECI_DEGC_TO_DEGC       (0.1f)

// clang-format on

typedef enum
{
    AUX_REGISTER_GROUP_A = 0U,
    AUX_REGISTER_GROUP_B,
    AUX_REGISTER_GROUP_C,
    NUM_OF_AUX_REGISTER_GROUPS
} AuxiliaryRegisterGroup;

extern const SpiInterface *ltc6813_spi;

// A 0-100°C temperature reverse lookup table with 0.5°C resolution for a Vishay
// NTCALUG03A103G thermistor. The 0th index represents 0°C. Incrementing the
// index represents a 0.5°C increase in temperature.
static const float temp_resistance_lut[SIZE_OF_TEMPERATURE_LUT] = {
    32624.2f, 31804.3f, 31007.3f, 30232.8f, 29479.9f, 28747.9f, 28036.3f, 27344.5f, 26671.8f, 26017.6f, 25381.4f,
    24762.6f, 24160.7f, 23575.3f, 23005.7f, 22451.6f, 21912.4f, 21387.8f, 20877.3f, 20380.5f, 19896.9f, 19426.2f,
    18968.0f, 18522.0f, 18087.8f, 17664.9f, 17253.2f, 16852.3f, 16461.9f, 16081.6f, 15711.3f, 15350.5f, 14999.0f,
    14656.6f, 14323.0f, 13998.0f, 13681.2f, 13372.6f, 13071.7f, 12778.5f, 12492.8f, 12214.2f, 11942.6f, 11677.8f,
    11419.7f, 11168.0f, 10922.5f, 10683.2f, 10449.8f, 10222.0f, 10000.0f, 9783.4f,  9572.1f,  9365.9f,  9164.7f,
    8968.5f,  8777.0f,  8590.1f,  8407.7f,  8229.7f,  8056.0f,  7886.4f,  7720.8f,  7559.2f,  7401.4f,  7247.4f,
    7097.0f,  6950.1f,  6806.6f,  6666.6f,  6529.7f,  6396.1f,  6265.6f,  6138.1f,  6013.5f,  5891.8f,  5772.9f,
    5656.7f,  5543.2f,  5432.3f,  5323.9f,  5217.9f,  5114.4f,  5013.2f,  4914.2f,  4817.5f,  4722.9f,  4630.5f,
    4540.1f,  4451.7f,  4365.3f,  4280.8f,  4198.1f,  4117.3f,  4038.2f,  3960.9f,  3885.2f,  3811.2f,  3738.8f,
    3668.0f,  3598.7f,  3530.9f,  3464.6f,  3399.7f,  3336.1f,  3273.9f,  3213.1f,  3153.5f,  3095.2f,  3038.1f,
    2982.3f,  2927.6f,  2874.0f,  2821.6f,  2770.3f,  2720.0f,  2670.8f,  2622.6f,  2575.3f,  2529.1f,  2483.8f,
    2439.5f,  2396.0f,  2353.4f,  2311.7f,  2270.9f,  2230.9f,  2191.6f,  2153.2f,  2115.6f,  2078.7f,  2042.5f,
    2007.1f,  1972.3f,  1938.3f,  1904.9f,  1872.2f,  1840.1f,  1808.7f,  1777.9f,  1747.7f,  1718.0f,  1689.0f,
    1660.5f,  1632.6f,  1605.2f,  1578.3f,  1552.0f,  1526.1f,  1500.8f,  1475.9f,  1451.5f,  1427.6f,  1404.2f,
    1381.1f,  1358.5f,  1336.4f,  1314.6f,  1293.3f,  1272.4f,  1251.8f
};

// Commands used to read data stored inside auxiliary register groups
static const uint16_t aux_reg_group_cmds[NUM_OF_AUX_REGISTER_GROUPS] = {
    [AUX_REGISTER_GROUP_A] = RDAUXA,
    [AUX_REGISTER_GROUP_B] = RDAUXB,
    [AUX_REGISTER_GROUP_C] = RDAUXC,
};

struct LTC6813TempStatistics
{
    struct
    {
        uint8_t  segment;
        uint8_t  thermistor;
        uint16_t temp;
    } min, max;

    float avg;
};

typedef struct
{
    struct LTC6813TempStatistics stats;
    uint16_t cell[ACCUMULATOR_NUM_SEGMENTS][NUM_OF_AUX_REGISTER_GROUPS][NUM_OF_READINGS_PER_REG_GROUP];
} LTC6813Temperatures;
static LTC6813Temperatures ltc6813_temp = { 0U };

/**
 * A function that can be called to update min/max cell voltages, segment
 * voltages, and pack voltage
 */
static void updateCellTemperatureStatistics(void)
{
    struct LTC6813TempStatistics temp_stats = {
        .min = { .segment = 0U, .thermistor = 0U, .temp = UINT16_MAX, },
        .max = { .segment = 0U, .thermistor = 0U, .temp = 0x0000U, },
        .avg = 0.0f,
    };

    uint32_t sum_temp = 0U;

    for (uint8_t curr_segment = 0U; curr_segment < ACCUMULATOR_NUM_SEGMENTS; curr_segment++)
    {
        for (uint8_t curr_reg_group = 0U; curr_reg_group < NUM_OF_AUX_REGISTER_GROUPS; curr_reg_group++)
        {
            for (uint8_t curr_thermistor = 0U; curr_thermistor < NUM_OF_READINGS_PER_REG_GROUP; curr_thermistor++)
            {
                if (IS_CELL_TEMP_READING(curr_reg_group, curr_thermistor))
                {
                    const uint16_t curr_cell_temp = ltc6813_temp.cell[curr_segment][curr_reg_group][curr_thermistor];
                    const uint8_t  curr_cell_index =
                        (uint8_t)(curr_reg_group * NUM_OF_READINGS_PER_REG_GROUP + curr_thermistor);

                    // Get the minimum cell voltage
                    if (curr_cell_temp < temp_stats.min.temp)
                    {
                        temp_stats.min.temp       = curr_cell_temp;
                        temp_stats.min.segment    = curr_segment;
                        temp_stats.min.thermistor = curr_cell_index;
                    }

                    // Get the maximum cell voltage
                    if (curr_cell_temp > temp_stats.max.temp)
                    {
                        temp_stats.max.temp       = curr_cell_temp;
                        temp_stats.max.segment    = curr_segment;
                        temp_stats.max.thermistor = curr_cell_index;
                    }

                    sum_temp += curr_cell_temp;
                }
            }
        }
    }

    // Calculate the average temperature
    temp_stats.avg = (float)sum_temp / TOTAL_NUM_OF_THERMISTORS;

    // Update min, max, average temperatures
    ltc6813_temp.stats = temp_stats;
}

/**
 * Calculate thermistor temperature
 * @param raw_thermistor_voltage The voltage measured across the thermistor
 * @return The thermistor temp in deci degC
 */
static uint16_t calculateThermistorTempDeciDegC(uint16_t raw_thermistor_voltage)
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
    //      ┌─── VREF
    //      │
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
    //      │
    //      └─── V-
    //
    //

    uint16_t    cell_temp             = UINT16_MAX;
    const float gpio_voltage          = (float)raw_thermistor_voltage * V_PER_100UV;
    const float thermistor_resistance = (gpio_voltage * BIAS_RESISTOR_OHM) / (REFERENCE_VOLTAGE - gpio_voltage);

    // Check that the thermistor resistance is in range
    if ((thermistor_resistance <= temp_resistance_lut[0]) &&
        (thermistor_resistance >= temp_resistance_lut[SIZE_OF_TEMPERATURE_LUT - 1U]))
    {
        // Find the index corresponding to the calculated thermistor
        // resistance
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
        cell_temp = (uint16_t)(therm_lut_index * THERM_INDEX_TO_DEGC);
    }

    return cell_temp;
}

/**
 * Parse cell temperature from all segments on the daisy chain
 * @param curr_reg_group The current register group to read back temperatures
 * for
 * @param rx_buffer The buffer containing cell temperature data
 * @return True if cell temperatures are read back correctly, otherwise false.
 */
static bool parseCellTempFromAllSegments(uint8_t curr_reg_group, uint16_t rx_buffer[NUM_REG_GROUP_RX_WORDS])
{
    bool status = true;

    for (uint8_t curr_segment = 0U; curr_segment < ACCUMULATOR_NUM_SEGMENTS; curr_segment++)
    {
        // Set the starting index to read cell voltages for the current segment
        // from rx_buffer
        uint8_t start_index = (uint8_t)(curr_segment * TOTAL_NUM_REG_GROUP_WORDS);

        // Calculate PEC15 from the data received on rx_buffer
        const uint16_t calc_pec15 = io_ltc6813Shared_calculateRegGroupPec15((uint8_t *)&rx_buffer[start_index]);

        // Read PEC15 from the rx_buffer
        const uint16_t recv_pec15 = rx_buffer[start_index + REG_GROUP_WORD_PEC_INDEX];

        if (recv_pec15 == calc_pec15)
        {
            for (uint8_t curr_thermistor = 0U; curr_thermistor < NUM_OF_READINGS_PER_REG_GROUP; curr_thermistor++)
            {
                // Skip the reference voltage of AUX register group B
                if (IS_CELL_TEMP_READING(curr_reg_group, curr_thermistor))
                {
                    ltc6813_temp.cell[curr_segment][curr_reg_group][curr_thermistor] =
                        calculateThermistorTempDeciDegC(rx_buffer[start_index + curr_thermistor]);
                }
            }
        }
        else
        {
            status = false;
        }
    }

    return status;
}

bool io_ltc6813CellTemps_startAdcConversion(void)
{
    return io_ltc6813Shared_sendCommand(ADAX);
}

bool io_ltc6813CellTemps_readTemperatures(void)
{
    bool status = false;

    if (io_ltc6813Shared_pollAdcConversions())
    {
        uint16_t rx_buffer[NUM_REG_GROUP_RX_WORDS] = { 0U };

        // Read thermistor voltages stored in the AUX register groups
        for (uint8_t curr_reg_group = 0U; curr_reg_group < NUM_OF_AUX_REGISTER_GROUPS; curr_reg_group++)
        {
            uint16_t tx_cmd[NUM_OF_CMD_WORDS] = {
                [CMD_WORD]  = aux_reg_group_cmds[curr_reg_group],
                [CMD_PEC15] = 0U,
            };
            io_ltc6813Shared_packCmdPec15(tx_cmd);

            if (hw_spi_transmitAndReceive(
                    ltc6813_spi, (uint8_t *)tx_cmd, TOTAL_NUM_CMD_BYTES, (uint8_t *)rx_buffer, NUM_REG_GROUP_RX_BYTES))
            {
                if (!parseCellTempFromAllSegments(curr_reg_group, rx_buffer))
                {
                    status = false;
                }
            }
        }

        // Update min/max cell segment, index and voltages and update pack
        // voltage and segment voltages
        updateCellTemperatureStatistics();
    }

    return status;
}

float io_ltc6813CellTemps_getMinTempDegC(uint8_t *segment, uint8_t *therm)
{
    *segment = ltc6813_temp.stats.min.segment;
    *therm   = ltc6813_temp.stats.min.thermistor;
    return (float)ltc6813_temp.stats.min.temp * DECI_DEGC_TO_DEGC;
}

float io_ltc6813CellTemps_getMaxTempDegC(uint8_t *segment, uint8_t *therm)
{
    *segment = ltc6813_temp.stats.max.segment;
    *therm   = ltc6813_temp.stats.max.thermistor;
    return (float)ltc6813_temp.stats.max.temp * DECI_DEGC_TO_DEGC;
}

float io_ltc6813CellTemps_getAverageTempDegC(void)
{
    return ltc6813_temp.stats.avg;
}

float io_ltc6813CellTemperatures_getSpecificCellTempDegC(uint8_t segment, uint8_t reg_group, uint8_t thermistor)
{
    return ltc6813_temp.cell[segment][reg_group][thermistor] * DECI_DEGC_TO_DEGC;
}
