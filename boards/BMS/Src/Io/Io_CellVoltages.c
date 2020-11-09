#include "Io_CellVoltages.h"
#include "Io_SharedSpi.h"
#include "Io_LTC6813.h"
#include "configs/App_AccumulatorConfigs.h"
#include "configs/Io_LTC6813Configs.h"

#define NUM_OF_CELLS_PER_LTC6813_REGISTER_GROUP 3U

enum CellVoltageRegisterGroup
{
    CELL_VOLTAGE_REGISTER_GROUP_A,
    CELL_VOLTAGE_REGISTER_GROUP_B,
    CELL_VOLTAGE_REGISTER_GROUP_C,
    CELL_VOLTAGE_REGISTER_GROUP_D,
    CELL_VOLTAGE_REGISTER_GROUP_E,
    CELL_VOLTAGE_REGISTER_GROUP_F,
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

static uint16_t cell_voltages[NUM_OF_CELL_MONITOR_ICS]
                             [NUM_OF_CELLS_READ_PER_IC];

/**
 * Parse raw cell voltages received from the cell monitoring IC and perform
 * PEC15 checks.
 * @param current_ic The current cell monitoring IC to parse cell voltages for.
 * @param current_register_group The current register group on the given IC to
 * parse cell voltages for.
 * @param rx_cell_voltages The buffer containing the cell voltages read from the
 * cell monitoring IC.
 * @return EXIT_CODE_OK if the PEC15 check was successful. Else,
 * EXIT_CODE_ERROR.
 */
static ExitCode Io_CellVoltages_ParseRawVoltagesAndDoPec15Check(
    size_t                        current_ic,
    enum CellVoltageRegisterGroup current_register_group,
    uint8_t *                     rx_cell_voltages);

static ExitCode Io_CellVoltages_ParseRawVoltagesAndDoPec15Check(
    size_t                        current_ic,
    enum CellVoltageRegisterGroup current_register_group,
    uint8_t *                     rx_cell_voltages)
{
    size_t cell_voltage_index = current_ic * NUM_OF_RX_BYTES;

    for (size_t current_cell = 0U;
         current_cell < NUM_OF_CELLS_PER_LTC6813_REGISTER_GROUP; current_cell++)
    {
        const uint32_t cell_voltage =
            (uint32_t)(rx_cell_voltages[cell_voltage_index]) |
            (uint32_t)((rx_cell_voltages[cell_voltage_index + 1] << 8));

        cell_voltages[current_ic]
                     [current_cell +
                      current_register_group *
                          NUM_OF_CELLS_PER_LTC6813_REGISTER_GROUP] =
                         (uint16_t)cell_voltage;

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

    const uint32_t received_pec15 =
        (uint32_t)(rx_cell_voltages[cell_voltage_index] << 8) |
        (uint32_t)(rx_cell_voltages[cell_voltage_index + 1]);

    // Calculate the PEC15 using the first 6 bytes of data received from the
    // chip.
    const uint32_t calculated_pec15 = Io_LTC6813_CalculatePec15(
        &rx_cell_voltages[current_ic * NUM_OF_RX_BYTES], 6U);

    if (received_pec15 != calculated_pec15)
    {
        return EXIT_CODE_ERROR;
    }

    return EXIT_CODE_OK;
}

ExitCode Io_CellVoltages_ReadRawCellVoltages(void)
{
    uint16_t cell_register_group_cmd;
    uint8_t  tx_cmd[NUM_OF_CMD_BYTES];
    uint8_t rx_cell_voltages[NUM_OF_RX_BYTES * NUM_OF_CELL_MONITOR_ICS] = { 0 };

    RETURN_IF_EXIT_NOT_OK(Io_LTC6813_EnterReadyState())
    RETURN_IF_EXIT_NOT_OK(Io_LTC6813_StartCellVoltageConversions())
    RETURN_IF_EXIT_NOT_OK(Io_LTC6813_PollConversions())

    for (enum CellVoltageRegisterGroup current_register_group =
             CELL_VOLTAGE_REGISTER_GROUP_A;
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
                NUM_OF_RX_BYTES * NUM_OF_CELL_MONITOR_ICS) != HAL_OK)
        {
            return EXIT_CODE_ERROR;
        }

        for (enum CellMonitorICs current_ic = CELL_MONITOR_IC_0;
             current_ic < NUM_OF_CELL_MONITOR_ICS; current_ic++)
        {
            if (Io_CellVoltages_ParseRawVoltagesAndDoPec15Check(
                    current_ic, current_register_group, rx_cell_voltages) !=
                EXIT_CODE_OK)
            {
                return EXIT_CODE_ERROR;
            }
        }
    }

    return EXIT_CODE_OK;
}

uint16_t *Io_CellVoltages_GetRawCellVoltages(void)
{
    return &cell_voltages[0][0];
}
