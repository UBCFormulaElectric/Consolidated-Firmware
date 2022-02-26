#include "Io_SharedSpi.h"
#include "Io_LTC6813.h"
#include "App_Accumulator.h"

#define NUM_OF_THERMISTORS_PER_IC (8U)
#define NUM_OF_THERMISTORS_PER_REG_GROUP (3U)
#define SIZE_OF_TEMPERATURE_LUT (201U)

#define BIAS_RESISTOR_OHM (10000.0f)
#define REFERENCE_VOLTAGE (3.0f)
#define THERM_INDEX_TO_DEGC (5U)
#define DECI_DEGC_TO_DEGC (0.1f)

#define MD (1U)
#define CHST (0U)
#define ADSTAT ((0x468 + (MD << 7) + CHST))

#define MAX_THERMISTOR_RESISTANCE (temperature_lut[0])
#define MIN_THERMISTOR_RESISTANCE (temperature_lut[SIZE_OF_TEMPERATURE_LUT - 1])

// A macro used to reset accumulator voltages to their default settings
#define RESET_ACCUMULATOR_TEMPERATURE()                                        \
    (struct AccumulatorTemperatures)                                           \
    {                                                                          \
        .avg_cell_degc = 0.0f,                                                 \
        .min_cell      = { .segment    = 0U,                                   \
                      .thermistor = 0U,                                   \
                      .temp_degc  = UINT16_MAX },                          \
        .max_cell      = { .segment = 0U, .thermistor = 0U, .temp_degc = 0U }, \
    }

enum AuxiliaryRegisterGroup
{
    AUX_REGISTER_GROUP_A = 0,
    AUX_REGISTER_GROUP_B,
    AUX_REGISTER_GROUP_C,
    NUM_OF_AUX_REGISTER_GROUPS
};

struct AccumulatorTemperatures
{
    float avg_cell_degc;
    struct CellTemperatures
    {
        uint8_t  segment;
        uint8_t  thermistor;
        uint16_t temp_degc;
    } min_cell, max_cell;
};

extern struct SharedSpi *ltc6813_spi;

// Commands used to read data stored inside auxiliary register groups
static const uint16_t aux_reg_group_cmds[NUM_OF_AUX_REGISTER_GROUPS] = {
    [AUX_REGISTER_GROUP_A] = 0x000C,
    [AUX_REGISTER_GROUP_B] = 0x000E,
    [AUX_REGISTER_GROUP_C] = 0x000D,
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

static struct AccumulatorTemperatures acc_temperatures = { 0U };
static uint16_t cell_temperatures[NUM_OF_ACCUMULATOR_SEGMENTS]
                                 [NUM_OF_THERMISTORS_PER_IC] = { { 0U } };

static void Io_UpdateCellTemperatures(void);
static uint16_t
            Io_CellTemp_CalculateCellTemperatureDegC(uint16_t raw_thermistor_voltage);
static bool Io_CellTemperatures_ParseThermistorVoltages(
    uint8_t curr_segment,
    uint8_t curr_reg_group,
    uint8_t rx_buffer[TOTAL_NUM_OF_REG_BYTES]);

static inline float Io_ConvertToDegC(float cell_temp)
{
    return cell_temp * DECI_DEGC_TO_DEGC;
}

static void Io_UpdateCellTemperatures(void)
{
    // Reset accumulator temperatures
    acc_temperatures       = RESET_ACCUMULATOR_TEMPERATURE();
    uint32_t sum_cell_temp = 0U;

    // Loop through each segment and thermistor to get min temperature, max
    // temperature, average temperature
    for (uint8_t curr_segment = 0U; curr_segment < NUM_OF_ACCUMULATOR_SEGMENTS;
         curr_segment++)
    {
        for (uint8_t curr_thermistor = 0U;
             curr_thermistor < NUM_OF_THERMISTORS_PER_IC; curr_thermistor++)
        {
            const uint16_t curr_cell_temp_degc =
                cell_temperatures[curr_segment][curr_thermistor];
            if (curr_cell_temp_degc < acc_temperatures.min_cell.temp_degc)
            {
                acc_temperatures.min_cell.temp_degc  = curr_cell_temp_degc;
                acc_temperatures.min_cell.segment    = curr_segment;
                acc_temperatures.min_cell.thermistor = curr_thermistor;
            }

            if (curr_cell_temp_degc > acc_temperatures.max_cell.temp_degc)
            {
                acc_temperatures.max_cell.temp_degc  = curr_cell_temp_degc;
                acc_temperatures.max_cell.segment    = curr_segment;
                acc_temperatures.max_cell.thermistor = curr_thermistor;
            }

            sum_cell_temp += curr_cell_temp_degc;
        }
    }

    // Calculate the average cell temperature
    acc_temperatures.avg_cell_degc =
        ((float)sum_cell_temp /
         (NUM_OF_THERMISTORS_PER_IC * NUM_OF_ACCUMULATOR_SEGMENTS));
}

static uint16_t
    Io_CellTemp_CalculateCellTemperatureDegC(uint16_t raw_thermistor_voltage)
{
    uint16_t    cell_temp    = UINT16_MAX;
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

static bool Io_CellTemperatures_ParseThermistorVoltages(
    uint8_t curr_segment,
    uint8_t curr_reg_group,
    uint8_t rx_buffer[TOTAL_NUM_OF_REG_BYTES])
{
    bool status = false;

    uint8_t        therm_index = (uint8_t)(curr_segment * NUM_REG_GROUP_BYTES);
    const uint16_t recv_pec15  = BYTES_TO_WORD(
        rx_buffer[therm_index + REG_GROUP_PEC0],
        rx_buffer[therm_index + REG_GROUP_PEC1]);
    const uint16_t calc_pec15 =
        Io_LTC6813_CalculateRegGroupPec15(&rx_buffer[therm_index]);

    if (recv_pec15 == calc_pec15)
    {
        for (uint8_t curr_thermistor = 0U;
             curr_thermistor < NUM_OF_THERMISTORS_PER_REG_GROUP;
             curr_thermistor++)
        {
            // Calculate the current thermistor voltage index
            uint32_t index = curr_thermistor +
                             curr_reg_group * NUM_OF_THERMISTORS_PER_REG_GROUP;

            if (curr_reg_group == AUX_REGISTER_GROUP_C)
            {
                // Subtract curr_therm_index to overwrite the reference voltage
                // read back from aux register group B
                index--;
            }

            // Calculate and store temperature from thermistor voltage
            cell_temperatures[curr_segment][index] =
                Io_CellTemp_CalculateCellTemperatureDegC(BYTES_TO_WORD(
                    rx_buffer[therm_index + 1], (rx_buffer[therm_index])));

            // Data stored within a register group is 2 bytes wide. Increment by
            // 2 bytes to retrieve the next thermistor voltage
            therm_index = (uint8_t)(therm_index + NUM_BYTES_REG_GROUP_DATA);
        }

        status = true;
    }

    return status;
}

bool Io_ParseAuxRegisterForAllSegments(
    uint8_t curr_reg_group,
    uint8_t rx_buffer[TOTAL_NUM_OF_REG_BYTES])
{
    for (uint8_t curr_segment = 0U; curr_segment < NUM_OF_ACCUMULATOR_SEGMENTS;
         curr_segment++)
    {
        // Parse received thermistor voltages, and store data in
        // raw_therm_voltage
        if (!Io_CellTemperatures_ParseThermistorVoltages(
                curr_segment, curr_reg_group, rx_buffer))
        {
            return false;
        }
    }

    return true;
}

bool Io_CellTemperatures_GetCellTemperatureDegC(void)
{
    if (!Io_LTC6813_PollAdcConversions())
    {
        return false;
    }

    uint8_t tx_cmd[TOTAL_NUM_CMD_BYTES] = { 0U };
    uint8_t rx_buffer[TOTAL_NUM_OF_REG_BYTES];

    // Read thermistor voltages stored in the AUX register groups
    for (uint8_t curr_reg_group = 0U;
         curr_reg_group < NUM_OF_AUX_REGISTER_GROUPS; curr_reg_group++)
    {
        Io_LTC6813_PrepareCmd(tx_cmd, aux_reg_group_cmds[curr_reg_group]);

        if (Io_SharedSpi_TransmitAndReceive(
                ltc6813_spi, tx_cmd, TOTAL_NUM_CMD_BYTES, rx_buffer,
                TOTAL_NUM_OF_REG_BYTES))
        {
            if (!Io_ParseAuxRegisterForAllSegments(curr_reg_group, rx_buffer))
            {
                return false;
            }
        }
    }

    // Get min, max, and average cell temperature
    Io_UpdateCellTemperatures();

    return true;
}

float Io_CellTemperatures_GetMinCellTempDegC(void)
{
    return Io_ConvertToDegC((float)(acc_temperatures.min_cell.temp_degc));
}

void Io_CellTemperatures_GetMinCellLocation(
    uint8_t *segment,
    uint8_t *thermistor)
{
    *segment    = acc_temperatures.min_cell.segment;
    *thermistor = acc_temperatures.min_cell.thermistor;
}

float Io_CellTemperatures_GetMaxCellTempDegC(void)
{
    return Io_ConvertToDegC((float)(acc_temperatures.max_cell.temp_degc));
}

void Io_CellTemperatures_GetMaxCellLocation(
    uint8_t *segment,
    uint8_t *thermistor)
{
    *segment    = acc_temperatures.max_cell.segment;
    *thermistor = acc_temperatures.max_cell.thermistor;
}

float Io_CellTemperatures_GetAverageCellTempDegC(void)
{
    return Io_ConvertToDegC(acc_temperatures.avg_cell_degc);
}

bool Io_CellVoltages_StartCellTemperatureConversion(void)
{
    return Io_LTC6813_SendCommand(ADSTAT);
}
