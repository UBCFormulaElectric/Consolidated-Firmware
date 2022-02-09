#include "Io_SharedSpi.h"
#include "Io_LTC6813.h"
#include "App_Accumulator.h"

#define NUM_OF_THERMISTORS_PER_IC (8U)
#define NUM_OF_THERMISTORS_PER_REG_GROUP (3U)
#define SIZE_OF_TEMPERATURE_LUT (201U)

#define BIAS_RESISTOR_OHM (10000.0f)
#define REFERENCE_VOLTAGE (3.0f)
#define THERM_INDEX_TO_DEGC (5U)

#define MAX_THERMISTOR_RESISTANCE (temperature_lut[0])
#define MIN_THERMISTOR_RESISTANCE (temperature_lut[SIZE_OF_TEMPERATURE_LUT - 1])

enum AuxiliaryRegisterGroup
{
    AUX_REGISTER_GROUP_C = 2,
    NUM_OF_AUX_REGISTER_GROUPS
};

extern struct SharedSpi *ltc6813_spi;
extern uint32_t          cell_temperatures[NUM_OF_ACCUMULATOR_SEGMENTS]
                                 [NUM_OF_THERMISTORS_PER_IC];
uint32_t cell_temperatures[NUM_OF_ACCUMULATOR_SEGMENTS]
                          [NUM_OF_THERMISTORS_PER_IC];

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

static inline uint16_t
    Io_CellTemp_CalculateCellTemperatureDegC(uint16_t raw_thermistor_voltage)
{
    uint16_t cell_temp = 0xFFFFU;

    const float gpio_voltage = (float)raw_thermistor_voltage * V_PER_100UV;
    const float thermistor_resistance =
        (gpio_voltage * BIAS_RESISTOR_OHM) / (REFERENCE_VOLTAGE - gpio_voltage);

    // Check that the thermistor resistance is in range
    if ((thermistor_resistance <= MAX_THERMISTOR_RESISTANCE) &&
        (thermistor_resistance >= MIN_THERMISTOR_RESISTANCE))
    {
        // Find the index corresponding to the calculated thermistor
        // resistance
        uint8_t therm_lut_index;
        for (therm_lut_index = 0U;
             thermistor_resistance < temperature_lut[therm_lut_index];
             therm_lut_index++)
            ;

        // Divide the index of the thermistor lookup table by 2 as the
        // temperature lookup table's key has a resolution of 0.5°C.
        // Multiply the result by 10 as we are storing the temperature
        // as 0.1°C.
        //
        //                            THERMISTOR LUT INDEX * 10
        // CELL_TEMPERATURES_DEGC = ----------------------------
        //                                        2
        //
        cell_temp = (uint16_t)(therm_lut_index * THERM_INDEX_TO_DEGC);
    }

    return cell_temp;
}

/**
 * Parse the raw thermistor voltages measured from the cell monitoring chip and
 * perform PEC15 check
 * @param curr_segment The current cell monitoring chip to parse thermistor
 * voltages for
 * @param curr_reg_group The current register group on the given chip to
 * parse thermistor voltages for
 * @param rx_raw_thermistor_voltage The buffer containing the raw thermistor
 * voltages read from the thermistors
 * @return EXIT_CODE_OK if the PEC15 check was successful. Else,
 * EXIT_CODE_ERROR
 */
static bool Io_CellTemperatures_ParseThermistorVoltages(
    uint8_t curr_segment,
    uint8_t curr_reg_group,
    uint8_t rx_raw_therm_v[TOTAL_NUM_OF_REG_BYTES]);

static bool Io_CellTemperatures_ParseThermistorVoltages(
    uint8_t curr_segment,
    uint8_t curr_reg_group,
    uint8_t rx_raw_therm_v[TOTAL_NUM_OF_REG_BYTES])
{
    // Set the start index of the current thermistor reading to store
    uint32_t rx_therm_v_index = (uint32_t)(curr_segment * NUM_REG_GROUP_BYTES);

    for (uint8_t curr_thermistor = 0U;
         curr_thermistor < NUM_OF_THERMISTORS_PER_REG_GROUP; curr_thermistor++)
    {
        // Calculate the current thermistor voltage index
        uint32_t curr_therm_v_index =
            curr_thermistor + curr_reg_group * NUM_OF_THERMISTORS_PER_REG_GROUP;
        if (curr_reg_group == AUX_REGISTER_GROUP_C)
        {
            // Subtract curr_therm_v_index to ignore the reference voltage
            // read back from aux register group B.
            curr_therm_v_index--;
        }

        // Calculate and store temperature from thermistor voltage
        const uint16_t raw_thermistor_voltage = (uint16_t)(
            (rx_raw_therm_v[rx_therm_v_index]) |
            ((rx_raw_therm_v[rx_therm_v_index + 1] << 8)));
        cell_temperatures[curr_segment][curr_therm_v_index] =
            Io_CellTemp_CalculateCellTemperatureDegC(raw_thermistor_voltage);

        // Data stored within a register group is 2 bytes wide. Increment by 2
        // bytes to retrieve the next thermistor voltage
        rx_therm_v_index += REG_GROUP_DATA_SIZE;
    }

    // Perform PEC15 check
    const uint16_t received_pec15 = (uint16_t)(
        (rx_raw_therm_v[rx_therm_v_index] << 8) |
        (rx_raw_therm_v[rx_therm_v_index + 1]));
    const uint16_t calculated_pec15 = Io_LTC6813_CalculatePec15(
        &rx_raw_therm_v[rx_therm_v_index], NUM_OF_REGS_IN_GROUP);

    return received_pec15 == calculated_pec15;
}

bool Io_CellTemperatures_GetCellTemperatureDegC(void)
{
    uint8_t tx_cmd[NUM_TX_CMD_BYTES] = { 0U };
    uint8_t recv_therm_resistances[TOTAL_NUM_OF_REG_BYTES];

    // Read thermistor voltages stored in the AUX register groups
    for (uint8_t curr_reg_group = 0U;
         curr_reg_group < NUM_OF_AUX_REGISTER_GROUPS; curr_reg_group++)
    {
        Io_LTC6813_PackCmd(tx_cmd, aux_register_group_commands[curr_reg_group]);
        Io_LTC6813_PackPec15(tx_cmd, NUM_OF_CMD_BYTES);

        if (Io_SharedSpi_TransmitAndReceive(
                ltc6813_spi, tx_cmd, NUM_TX_CMD_BYTES, recv_therm_resistances,
                TOTAL_NUM_OF_REG_BYTES))
        {
            for (uint8_t curr_segment = 0U;
                 curr_segment < NUM_OF_ACCUMULATOR_SEGMENTS; curr_segment++)
            {
                // Parse received thermistor voltages, and store data in
                // raw_therm_voltage
                if (!Io_CellTemperatures_ParseThermistorVoltages(
                        curr_segment, curr_reg_group, recv_therm_resistances))
                {
                    return false;
                }
            }
        }
    }

    return true;
}
