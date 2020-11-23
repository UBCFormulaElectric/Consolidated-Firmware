#include "Io_AccumulatorVoltages.h"
#include "Io_SharedSpi.h"
#include "Io_LTC6813.h"
#include "configs/App_AccumulatorConfigs.h"
#include "configs/Io_LTC6813Configs.h"

// This conversion factor is used to convert raw voltages (100µV) to voltages
// (V).
#define V_PER_100UV 1E-4f

#define NUM_OF_CELLS_READ_PER_CHIP 16U
#define NUM_OF_CELLS_PER_LTC6813_REGISTER_GROUP 3U

enum CellVoltageRegisterGroup
{
    CELL_VOLTAGE_REGISTER_GROUP_F = 5,
    NUM_OF_CELL_VOLTAGE_REGISTER_GROUPS
};

static const uint16_t cell_voltage_register_group_commands
    [NUM_OF_CELL_VOLTAGE_REGISTER_GROUPS] = {
        0x0400, // RDCVA
        0x0600, // RDCVB
        0x0800, // RDCVC
        0x0A00, // RDCVD
        0x0900, // RDCVE
        0x0B00  // RDCVF
    };

static uint16_t cell_voltages[NUM_OF_CELL_MONITOR_CHIPS]
                             [NUM_OF_CELLS_READ_PER_CHIP];

/**
 * A function used to compute the sum of array elements.
 * @param array A pointer to the given array.
 * @param length The length of the given array.
 * @return The sum of elements for the given array.
 */
static uint32_t App_SumOfArrayElements(uint16_t array[], size_t length);

/**
 * Parse raw cell voltages received from the cell monitoring chip and perform
 * PEC15 checks.
 * @param current_chip The current cell monitoring chip to parse cell voltages
 * for.
 * @param current_register_group The current register group on the given chip to
 * parse cell voltages for.
 * @param rx_cell_voltages The buffer containing the cell voltages read from the
 * cell monitoring chip.
 * @return EXIT_CODE_OK if the PEC15 check was successful. Else,
 * EXIT_CODE_ERROR.
 */
static ExitCode Io_CellVoltages_ParseRawVoltagesAndDoPec15Check(
    size_t  current_chip,
    size_t  current_register_group,
    uint8_t rx_cell_voltages[]);

static uint32_t App_SumOfArrayElements(uint16_t array[], size_t length)
{
    uint32_t array_sum = 0U;
    for (size_t i = 0U; i < length; i++)
    {
        array_sum += array[i];
    }

    return array_sum;
}

static ExitCode Io_CellVoltages_ParseRawVoltagesAndDoPec15Check(
    size_t  current_chip,
    size_t  current_register_group,
    uint8_t rx_cell_voltages[])
{
    size_t cell_voltage_index = current_chip * NUM_OF_RX_BYTES;

    for (size_t current_cell = 0U;
         current_cell < NUM_OF_CELLS_PER_LTC6813_REGISTER_GROUP; current_cell++)
    {
        const uint16_t cell_voltage = (uint16_t)(
            (rx_cell_voltages[cell_voltage_index]) |
            (rx_cell_voltages[cell_voltage_index + 1] << 8));

        cell_voltages[current_chip]
                     [current_cell +
                      current_register_group *
                          NUM_OF_CELLS_PER_LTC6813_REGISTER_GROUP] =
                         cell_voltage;

        if (current_register_group == CELL_VOLTAGE_REGISTER_GROUP_F)
        {
            // Since 16 cells are monitored for each accumulator segment and
            // there are 3 cell voltages per register group, ignore the last 2
            // cell voltages read back from CELL_VOLTAGE_REGISTER_F. The cell
            // voltage index is incremented by 6 to retrieve the PEC15 bytes for
            // the register group.
            cell_voltage_index += 6U;
            break;
        }
        else
        {
            // Each cell voltage is represented by 2 bytes. Therefore,
            // the cell voltage index is incremented by 2 to retrieve the next
            // cell voltage.
            cell_voltage_index += 2U;
        }
    }

    const uint16_t received_pec15 = (uint16_t)(
        (rx_cell_voltages[cell_voltage_index] << 8) |
        (rx_cell_voltages[cell_voltage_index + 1]));

    // Calculate the PEC15 using the first 6 bytes of data received from the
    // chip.
    const uint16_t calculated_pec15 = Io_LTC6813_CalculatePec15(
        &rx_cell_voltages[current_chip * NUM_OF_RX_BYTES], 6U);

    if (received_pec15 != calculated_pec15)
    {
        return EXIT_CODE_ERROR;
    }

    return EXIT_CODE_OK;
}

ExitCode Io_AccumulatorVoltages_ReadRawCellVoltages(void)
{
    uint16_t cell_register_group_cmd;
    uint8_t  tx_cmd[NUM_OF_CMD_BYTES];
    uint8_t  rx_cell_voltages[NUM_OF_RX_BYTES * NUM_OF_CELL_MONITOR_CHIPS] = {
        0
    };

    // The command used to start ADC conversions for battery cell voltages.
    const uint16_t ADCV = (0x260 + (MD << 7) + (DCP << 4) + CH);

    RETURN_IF_EXIT_NOT_OK(Io_LTC6813_EnterReadyState())
    RETURN_IF_EXIT_NOT_OK(Io_LTC6813_SendCommand(ADCV))
    RETURN_IF_EXIT_NOT_OK(Io_LTC6813_PollConversions())

    for (size_t current_register_group = 0U;
         current_register_group < NUM_OF_CELL_VOLTAGE_REGISTER_GROUPS;
         current_register_group++)
    {
        cell_register_group_cmd =
            cell_voltage_register_group_commands[current_register_group];

        tx_cmd[0] = (uint8_t)cell_register_group_cmd;
        tx_cmd[1] = (uint8_t)(cell_register_group_cmd >> 8);

        uint16_t tx_cmd_pec15 =
            Io_LTC6813_CalculatePec15(tx_cmd, NUM_OF_PEC15_BYTES_PER_CMD);
        tx_cmd[2] = (uint8_t)(tx_cmd_pec15 >> 8);
        tx_cmd[3] = (uint8_t)(tx_cmd_pec15);

        if (Io_SharedSpi_TransmitAndReceive(
                Io_LTC6813_GetSpiInterface(), tx_cmd, NUM_OF_CMD_BYTES,
                rx_cell_voltages,
                NUM_OF_RX_BYTES * NUM_OF_CELL_MONITOR_CHIPS) != HAL_OK)
        {
            return EXIT_CODE_ERROR;
        }

        for (size_t current_chip = 0U; current_chip < NUM_OF_CELL_MONITOR_CHIPS;
             current_chip++)
        {
            if (Io_CellVoltages_ParseRawVoltagesAndDoPec15Check(
                    current_chip, current_register_group, rx_cell_voltages) !=
                EXIT_CODE_OK)
            {
                return EXIT_CODE_ERROR;
            }
        }
    }

    return EXIT_CODE_OK;
}

float Io_AccumulatorVoltages_GetMinCellVoltage(void)
{
    uint16_t min_cell_voltage = cell_voltages[0][0];
    for (size_t current_chip = 0U; current_chip < NUM_OF_CELL_MONITOR_CHIPS;
         current_chip++)
    {
        for (size_t current_cell = 0U;
             current_cell < NUM_OF_CELLS_READ_PER_CHIP; current_cell++)
        {
            uint16_t current_cell_temp =
                cell_voltages[current_chip][current_cell];
            if (min_cell_voltage > current_cell_temp)
            {
                min_cell_voltage = current_cell_temp;
            }
        }
    }

    return (float)min_cell_voltage * V_PER_100UV;
}

float Io_AccumulatorVoltages_GetMaxCellVoltage(void)
{
    uint16_t max_cell_voltage = cell_voltages[0][0];
    for (size_t current_chip = 0U; current_chip < NUM_OF_CELL_MONITOR_CHIPS;
         current_chip++)
    {
        for (size_t current_cell = 0U;
             current_cell < NUM_OF_CELLS_READ_PER_CHIP; current_cell++)
        {
            uint16_t current_cell_temp =
                cell_voltages[current_chip][current_cell];
            if (max_cell_voltage < current_cell_temp)
            {
                max_cell_voltage = current_cell_temp;
            }
        }
    }

    return (float)max_cell_voltage * V_PER_100UV;
}

float Io_AccumulatorVoltages_GetAverageCellVoltage(void)
{
    return Io_AccumulatorVoltages_GetPackVoltage() /
           (float)(NUM_OF_CELL_MONITOR_CHIPS * NUM_OF_CELLS_READ_PER_CHIP);
}

float Io_AccumulatorVoltages_GetPackVoltage(void)
{
    uint32_t sum_of_cell_voltages = 0U;

    for (size_t current_chip = 0U; current_chip < NUM_OF_CELL_MONITOR_CHIPS;
         current_chip++)
    {
        for (size_t current_cell = 0U;
             current_cell < NUM_OF_CELLS_READ_PER_CHIP; current_cell++)
        {
            sum_of_cell_voltages += cell_voltages[current_chip][current_cell];
        }
    }

    return (float)sum_of_cell_voltages * V_PER_100UV;
}

float Io_AccumulatorVoltages_GetSegment0Voltage(void)
{
    return (float)App_SumOfArrayElements(
               &cell_voltages[0][0], NUM_OF_CELLS_READ_PER_CHIP) *
           V_PER_100UV;
}

float Io_AccumulatorVoltages_GetSegment1Voltage(void)
{
    return (float)App_SumOfArrayElements(
               &cell_voltages[1][0], NUM_OF_CELLS_READ_PER_CHIP) *
           V_PER_100UV;
}

float Io_AccumulatorVoltages_GetSegment2Voltage(void)
{
    return 0.0f;
}

float Io_AccumulatorVoltages_GetSegment3Voltage(void)
{
    return 0.0f;
}

float Io_AccumulatorVoltages_GetSegment4Voltage(void)
{
    return 0.0f;
}

float Io_AccumulatorVoltages_GetSegment5Voltage(void)
{
    return 0.0f;
}
