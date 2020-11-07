#include "Io_SharedSpi.h"
#include "Io_LTC6813.h"
#include "Io_Thermistors.h"
#include "configs/App_CellConfigs.h"
#include "configs/Io_LTC6813Configs.h"

#define NUM_OF_THERMISTORS_PER_IC NUM_OF_CELL_TEMPERATURES_READ_PER_IC
#define NUM_OF_AUX_MEASUREMENTS_PER_REGISTER_GROUP 3U

enum AuxiliaryRegisterGroup
{
    AUX_REGISTER_GROUP_A,
    AUX_REGISTER_GROUP_B,
    AUX_REGISTER_GROUP_C,
    AUX_REGISTER_GROUP_D,
    NUM_OF_AUX_REGISTER_GROUPS
};

// The commands below to read values stored inside auxiliary register groups.
static const uint16_t
    aux_register_group_commands[NUM_OF_AUX_REGISTER_GROUPS] = {
        0x000C, // RDAUXA
        0x000E, // RDAUXB
        0x000D, // RDAUXC
        0x000F, // RDAUXD
    };

static uint16_t raw_thermistor_voltages[NUM_OF_CELL_MONITOR_ICS]
                                       [NUM_OF_THERMISTORS_PER_IC];

/**
 * Parse the raw thermistor voltages measured from the cell monitoring chip and
 * perform PEC15 check.
 * @param current_chip The current cell monitoring chip to parse thermistor
 * voltages for.
 * @param current_register_group The current register group on the given chip to
 * parse thermistor voltages for.
 * @param rx_raw_thermistor_voltage The buffer containing the raw thermistor
 * voltages read from the cell monitoring chip.
 * @return EXIT_CODE_OK if the PEC15 check was successful. Else,
 * EXIT_CODE_ERROR.
 */
static ExitCode Io_Thermistors_ParseThermistorVoltagesAndPerformPec15Check(
    size_t                      current_chip,
    enum AuxiliaryRegisterGroup current_register_group,
    uint8_t *                   rx_raw_thermistor_voltages);

static ExitCode Io_Thermistors_ParseThermistorVoltagesAndPerformPec15Check(
    size_t                      current_chip,
    enum AuxiliaryRegisterGroup current_register_group,
    uint8_t *                   rx_raw_thermistor_voltages)
{
    size_t raw_thermistor_voltages_index = current_chip * NUM_OF_RX_BYTES;

    for (size_t current_cell = 0U;
         current_cell < NUM_OF_AUX_MEASUREMENTS_PER_REGISTER_GROUP;
         current_cell++)
    {
        const uint32_t raw_thermistor_voltage =
            (uint32_t)(
                rx_raw_thermistor_voltages[raw_thermistor_voltages_index]) |
            (uint32_t)(
                (rx_raw_thermistor_voltages[raw_thermistor_voltages_index + 1]
                 << 8));

        // Exclude the register group and index for the reference voltage.
        if (current_register_group != AUX_REGISTER_GROUP_B ||
            raw_thermistor_voltages_index != 4U)
        {
            raw_thermistor_voltages
                [current_chip]
                [current_cell +
                 current_register_group *
                     NUM_OF_AUX_MEASUREMENTS_PER_REGISTER_GROUP] =
                    (uint16_t)raw_thermistor_voltage;

            if (current_register_group == AUX_REGISTER_GROUP_D &&
                raw_thermistor_voltages_index == 2U)
            {
                // Since 8 thermistors temperatures are monitored for each
                // accumulator segment and there are 3 auxiliary measurements
                // per auxiliary register group, ignore the last thermistor
                // reading read back from AUX_REGISTER_GROUP_D. Increment the
                // cell_voltage_index by 4 to retrieve the PEC15 bytes for the
                // current register group.
                raw_thermistor_voltages_index += 4U;
                break;
            }
        }

        // Each aux measurement is represented by 2 bytes. Therefore,
        // the cell voltage index is incremented by 2 to retrieve the next
        // cell voltage.
        raw_thermistor_voltages_index += 2U;
    }

    uint32_t received_pec15 =
        (uint32_t)(
            rx_raw_thermistor_voltages[raw_thermistor_voltages_index] << 8) |
        (uint32_t)(
            rx_raw_thermistor_voltages[raw_thermistor_voltages_index + 1]);

    // Calculate the PEC15 using the first 6 bytes of data received from the
    // chip.
    uint32_t calculated_pec15 = Io_LTC6813_CalculatePec15(
        &rx_raw_thermistor_voltages[current_chip * NUM_OF_RX_BYTES], 6U);

    if (received_pec15 != calculated_pec15)
    {
        return EXIT_CODE_ERROR;
    }

    return EXIT_CODE_OK;
}

ExitCode Io_Thermistors_ReadRawVoltages(void)
{
    uint16_t aux_register_group_cmd;
    uint8_t  tx_cmd[NUM_OF_CMD_BYTES];
    uint8_t
        rx_thermistor_resistances[NUM_OF_RX_BYTES * NUM_OF_CELL_MONITOR_ICS] = {
            0
        };

    RETURN_IF_EXIT_NOT_OK(Io_LTC6813_EnterReadyState())
    RETURN_IF_EXIT_NOT_OK(Io_LTC6813_StartAuxiliaryMeasurements())
    RETURN_IF_EXIT_NOT_OK(Io_LTC6813_PollConversions())

    for (enum AuxiliaryRegisterGroup current_register_group =
             AUX_REGISTER_GROUP_A;
         current_register_group < NUM_OF_AUX_REGISTER_GROUPS;
         current_register_group++)
    {
        aux_register_group_cmd =
            aux_register_group_commands[current_register_group];

        tx_cmd[0] = (uint8_t)aux_register_group_cmd;
        tx_cmd[1] = (uint8_t)(aux_register_group_cmd >> 8);

        uint16_t tx_cmd_pec15 =
            Io_LTC6813_CalculatePec15(tx_cmd, NUM_OF_PEC15_BYTES_PER_CMD);
        tx_cmd[2] = (uint8_t)(tx_cmd_pec15 >> 8);
        tx_cmd[3] = (uint8_t)(tx_cmd_pec15);

        if (Io_SharedSpi_TransmitAndReceive(
                Io_LTC6813_GetSpiInterface(), tx_cmd, NUM_OF_CMD_BYTES,
                rx_thermistor_resistances,
                NUM_OF_RX_BYTES * NUM_OF_CELL_MONITOR_ICS) != HAL_OK)
        {
            return EXIT_CODE_ERROR;
        }

        for (enum CellMonitorICs current_ic = CELL_MONITOR_IC_0;
             current_ic < NUM_OF_CELL_MONITOR_ICS; current_ic++)
        {
            if (Io_Thermistors_ParseThermistorVoltagesAndPerformPec15Check(
                    current_ic, current_register_group,
                    rx_thermistor_resistances) != EXIT_CODE_OK)
            {
                return EXIT_CODE_ERROR;
            }
        }
    }

    return EXIT_CODE_OK;
}

uint16_t *Io_Thermistors_GetRawVoltages(void)
{
    return &raw_thermistor_voltages[0][0];
}
