#include "Io_SharedSpi.h"
#include "Io_LTC6813.h"
#include "Io_CellTemperatures.h"
#include "configs/App_AccumulatorConfigs.h"
#include "configs/Io_LTC6813Configs.h"

#define NUM_OF_THERMISTORS_PER_IC 8U
#define NUM_OF_THERMISTORS_PER_REGISTER_GROUP 3U
#define SIZE_OF_TEMPERATURE_LUT 201

enum AuxiliaryRegisterGroup
{
    AUX_REGISTER_GROUP_C = 2,
    NUM_OF_AUX_REGISTER_GROUPS
};

// The commands below to read values stored inside auxiliary register groups.
static const uint16_t
    aux_register_group_commands[NUM_OF_AUX_REGISTER_GROUPS] = {
        0x000C, // RDAUXA
        0x000E, // RDAUXB
        0x000D, // RDAUXC
    };

// A 0-100°C temperature reverse lookup table with 0.5°C resolution for a Vishay
// NTCALUG03A103G thermistor. The 0th index represents 0°C. Incrementing the
// index represents a 0.5°C increase in temperature.
static const float temperature_lut[SIZE_OF_TEMPERATURE_LUT] = {
    32624.2f, 31804.3f, 31007.3f, 30232.8f, 29479.9f, 28747.9f, 28036.3f,
    27344.5f, 26671.8f, 26017.6f, 25381.4f, 24762.6f, 24160.7f, 23575.3f,
    23005.7f, 22451.6f, 21912.4f, 21387.8f, 20877.3f, 20380.5f, 19896.9f,
    19426.2f, 18968.0f, 18522.0f, 18087.8f, 17664.9f, 17253.2f, 16852.3f,
    16461.9f, 16081.6f, 15711.3f, 15350.5f, 14999.0f, 14656.6f, 14323.0f,
    13998.0f, 13681.2f, 13372.6f, 13071.7f, 12778.5f, 12492.8f, 12214.2f,
    11942.6f, 11677.8f, 11419.7f, 11168.0f, 10922.5f, 10683.2f, 10449.8f,
    10222.0f, 10000.0f, 9783.4f,  9572.1f,  9365.9f,  9164.7f,  8968.5f,
    8777.0f,  8590.1f,  8407.7f,  8229.7f,  8056.0f,  7886.4f,  7720.8f,
    7559.2f,  7401.4f,  7247.4f,  7097.0f,  6950.1f,  6806.6f,  6666.6f,
    6529.7f,  6396.1f,  6265.6f,  6138.1f,  6013.5f,  5891.8f,  5772.9f,
    5656.7f,  5543.2f,  5432.3f,  5323.9f,  5217.9f,  5114.4f,  5013.2f,
    4914.2f,  4817.5f,  4722.9f,  4630.5f,  4540.1f,  4451.7f,  4365.3f,
    4280.8f,  4198.1f,  4117.3f,  4038.2f,  3960.9f,  3885.2f,  3811.2f,
    3738.8f,  3668.0f,  3598.7f,  3530.9f,  3464.6f,  3399.7f,  3336.1f,
    3273.9f,  3213.1f,  3153.5f,  3095.2f,  3038.1f,  2982.3f,  2927.6f,
    2874.0f,  2821.6f,  2770.3f,  2720.0f,  2670.8f,  2622.6f,  2575.3f,
    2529.1f,  2483.8f,  2439.5f,  2396.0f,  2353.4f,  2311.7f,  2270.9f,
    2230.9f,  2191.6f,  2153.2f,  2115.6f,  2078.7f,  2042.5f,  2007.1f,
    1972.3f,  1938.3f,  1904.9f,  1872.2f,  1840.1f,  1808.7f,  1777.9f,
    1747.7f,  1718.0f,  1689.0f,  1660.5f,  1632.6f,  1605.2f,  1578.3f,
    1552.0f,  1526.1f,  1500.8f,  1475.9f,  1451.5f,  1427.6f,  1404.2f,
    1381.1f,  1358.5f,  1336.4f,  1314.6f,  1293.3f,  1272.4f,  1251.8f
};

static uint16_t raw_thermistor_voltages[NUM_OF_CELL_MONITOR_CHIPS]
                                       [NUM_OF_THERMISTORS_PER_IC];
static uint32_t cell_temperatures[NUM_OF_CELL_MONITOR_CHIPS]
                                 [NUM_OF_THERMISTORS_PER_IC];

/**
 * Parse the raw thermistor voltages measured from the cell monitoring chip and
 * perform PEC15 check
 * @param current_chip The current cell monitoring chip to parse thermistor
 * voltages for
 * @param current_register_group The current register group on the given chip to
 * parse thermistor voltages for
 * @param rx_raw_thermistor_voltage The buffer containing the raw thermistor
 * voltages read from the thermistors
 * @return EXIT_CODE_OK if the PEC15 check was successful. Else,
 * EXIT_CODE_ERROR
 */
static ExitCode Io_CellTemperatures_ParseThermistorVoltagesAndPerformPec15Check(
    size_t   current_chip,
    size_t   current_register_group,
    uint8_t *rx_raw_thermistor_voltages);

/**
 * Read raw thermistor voltages from the cell monitoring chips
 * @return EXIT_CODE_OK if raw thermistor voltages are read successfully from
 * all cell monitoring chips. Else, EXIT_CODE_ERROR
 */
static ExitCode Io_CellTemperatures_ReadRawThermistorVoltages(void);

static ExitCode Io_CellTemperatures_ParseThermistorVoltagesAndPerformPec15Check(
    size_t   current_chip,
    size_t   current_register_group,
    uint8_t *rx_raw_thermistor_voltages)
{
    size_t raw_thermistor_voltages_index = current_chip * NUM_OF_RX_BYTES;

    for (size_t current_cell = 0U;
         current_cell < NUM_OF_THERMISTORS_PER_REGISTER_GROUP; current_cell++)
    {
        const uint16_t raw_thermistor_voltage = (uint16_t)(
            (rx_raw_thermistor_voltages[raw_thermistor_voltages_index]) |
            ((rx_raw_thermistor_voltages[raw_thermistor_voltages_index + 1]
              << 8)));

        size_t curr_column =
            current_cell +
            current_register_group * NUM_OF_THERMISTORS_PER_REGISTER_GROUP;
        if (current_register_group == AUX_REGISTER_GROUP_C)
        {
            // Subtract curr_column to ignore the reference voltage read back
            // from aux register group B.
            curr_column--;
        }
        raw_thermistor_voltages[current_chip][curr_column] =
            (uint16_t)raw_thermistor_voltage;

        // Each aux measurement is represented by 2 bytes. Therefore,
        // the raw thermistor voltage index is incremented by 2 to retrieve the
        // next thermistor voltage.
        raw_thermistor_voltages_index += 2U;
    }

    const uint16_t received_pec15 = (uint16_t)(
        (rx_raw_thermistor_voltages[raw_thermistor_voltages_index] << 8) |
        (rx_raw_thermistor_voltages[raw_thermistor_voltages_index + 1]));

    // Calculate the PEC15 using the first 6 bytes of data received from the
    // chip.
    const uint16_t calculated_pec15 = Io_LTC6813_CalculatePec15(
        &rx_raw_thermistor_voltages[current_chip * NUM_OF_RX_BYTES], 6U);

    return (received_pec15 == calculated_pec15) ? EXIT_CODE_OK
                                                : EXIT_CODE_ERROR;
}

static ExitCode Io_CellTemperatures_ReadRawThermistorVoltages(void)
{
    uint16_t aux_register_group_cmd;
    uint8_t  tx_cmd[NUM_OF_CMD_BYTES];
    uint8_t  rx_thermistor_resistances
        [NUM_OF_RX_BYTES * NUM_OF_CELL_MONITOR_CHIPS] = { 0 };

    // The command used to start auxiliary (GPIO) measurements.
    const uint16_t ADAX = 0x460 + (MD << 7) + CHST;

    RETURN_IF_EXIT_NOT_OK(Io_LTC6813_EnterReadyState())
    RETURN_IF_EXIT_NOT_OK(Io_LTC6813_SendCommand(ADAX))
    RETURN_IF_EXIT_NOT_OK(Io_LTC6813_PollConversions())

    for (size_t current_register_group = 0U;
         current_register_group < NUM_OF_AUX_REGISTER_GROUPS;
         current_register_group++)
    {
        aux_register_group_cmd =
            aux_register_group_commands[current_register_group];

        tx_cmd[0] = (uint8_t)(aux_register_group_cmd >> 8);
        tx_cmd[1] = (uint8_t)(aux_register_group_cmd);

        uint16_t tx_cmd_pec15 =
            Io_LTC6813_CalculatePec15(tx_cmd, NUM_OF_PEC15_BYTES_PER_CMD);
        tx_cmd[2] = (uint8_t)(tx_cmd_pec15 >> 8);
        tx_cmd[3] = (uint8_t)(tx_cmd_pec15);

        if (Io_SharedSpi_TransmitAndReceive(
                Io_LTC6813_GetSpiInterface(), tx_cmd, NUM_OF_CMD_BYTES,
                rx_thermistor_resistances,
                NUM_OF_RX_BYTES * NUM_OF_CELL_MONITOR_CHIPS) != HAL_OK)
        {
            return EXIT_CODE_ERROR;
        }

        for (size_t current_chip = 0U; current_chip < NUM_OF_CELL_MONITOR_CHIPS;
             current_chip++)
        {
            if (Io_CellTemperatures_ParseThermistorVoltagesAndPerformPec15Check(
                    current_chip, current_register_group,
                    rx_thermistor_resistances) != EXIT_CODE_OK)
            {
                return EXIT_CODE_ERROR;
            }
        }
    }

    return EXIT_CODE_OK;
}

ExitCode Io_CellTemperatures_ReadTemperatures(void)
{
    RETURN_IF_EXIT_NOT_OK(Io_CellTemperatures_ReadRawThermistorVoltages())

    for (size_t current_ic = 0U; current_ic < NUM_OF_CELL_MONITOR_CHIPS;
         current_ic++)
    {
        for (size_t cell_temp_index = 0U;
             cell_temp_index < NUM_OF_THERMISTORS_PER_IC; cell_temp_index++)
        {
            // Calculate the thermistor resistance by measuring the voltage
            // across the thermistor. The thermistor resistance (Ohms) can be
            // calculated by the following equation:
            //
            //                                GPIO_V * BIAS_RESISTOR_OHMS
            // THERMISTOR_RESISTANCE_OHMS = -------------------------------
            //                                  (REFERENCE_V - GPIO_V)
            //
            // BIAS_RESISTOR_OHMS = 10kΩ
            // REFERENCE_V = 3.0V
            //
            // The measured aux voltage is divided by 10000 in the calculation
            // below since it is read back in 100µV.

            const float BIAS_RESISTOR_OHM = 10000.0f;
            const float REFERENCE_VOLTAGE = 3.0f;
            const float gpio_voltage =
                (float)raw_thermistor_voltages[current_ic][cell_temp_index] /
                10000.0f;
            const float thermistor_resistance =
                (gpio_voltage * BIAS_RESISTOR_OHM) /
                (REFERENCE_VOLTAGE - gpio_voltage);

            // Check that the thermistor resistance calculated is within
            // [1251.8, 32624.2] ohms.
            if ((thermistor_resistance > temperature_lut[0]) ||
                (thermistor_resistance <
                 temperature_lut[SIZE_OF_TEMPERATURE_LUT - 1]))
            {
                return EXIT_CODE_OUT_OF_RANGE;
            }

            // Find the index corresponding to the calculated thermistor
            // resistance.
            size_t thermistor_lut_index;
            for (thermistor_lut_index = 0U;
                 thermistor_resistance < temperature_lut[thermistor_lut_index];
                 thermistor_lut_index++)
                ;

            // Divide the index of the thermistor lookup table by 2 as the
            // temperature lookup table's key has a resolution of 0.5°C.
            // Multiply the result by 10 as we are storing the temperature as
            // 0.1°C.
            //
            //                            THERMISTOR LUT INDEX * 10
            // CELL_TEMPERATURES_DEGC = ----------------------------
            //                                        2
            //

            cell_temperatures[current_ic][cell_temp_index] =
                thermistor_lut_index * 5U;
        }
    }

    return EXIT_CODE_OK;
}

uint32_t Io_CellTemperatures_GetMaxCellTemperature(void)
{
    uint32_t max_cell_temp = cell_temperatures[0][0];
    for (size_t current_chip = 0U; current_chip < NUM_OF_CELL_MONITOR_CHIPS;
         current_chip++)
    {
        for (size_t current_cell = 0U; current_cell < NUM_OF_THERMISTORS_PER_IC;
             current_cell++)
        {
            uint32_t current_cell_temp =
                cell_temperatures[current_chip][current_cell];
            if (max_cell_temp < current_cell_temp)
            {
                max_cell_temp = current_cell_temp;
            }
        }
    }

    return max_cell_temp;
}

uint32_t Io_CellTemperatures_GetMinCellTemperature(void)
{
    uint32_t min_cell_temp = cell_temperatures[0][0];
    for (size_t current_chip = 0U; current_chip < NUM_OF_CELL_MONITOR_CHIPS;
         current_chip++)
    {
        for (size_t current_cell = 0U; current_cell < NUM_OF_THERMISTORS_PER_IC;
             current_cell++)
        {
            uint32_t current_cell_temp =
                cell_temperatures[current_chip][current_cell];
            if (min_cell_temp > current_cell_temp)
            {
                min_cell_temp = current_cell_temp;
            }
        }
    }

    return min_cell_temp;
}

float Io_CellTemperatures_GetAverageCellTemperature(void)
{
    uint32_t sum_of_cell_temp = 0U;
    for (size_t current_chip = 0U; current_chip < NUM_OF_CELL_MONITOR_CHIPS;
         current_chip++)
    {
        for (size_t current_cell = 0U; current_cell < NUM_OF_THERMISTORS_PER_IC;
             current_cell++)
        {
            sum_of_cell_temp += cell_temperatures[current_chip][current_cell];
        }
    }

    return (float)sum_of_cell_temp /
           (float)(NUM_OF_THERMISTORS_PER_IC * NUM_OF_CELL_MONITOR_CHIPS);
}
