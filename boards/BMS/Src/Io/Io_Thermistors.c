#include "Io_Thermistors.h"
#include "Io_LTC6813.h"
#include "Io_SharedSpi.h"
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

// The set of commands used to read values stored inside auxiliary register
// groups.
static const uint16_t
    aux_register_group_commands[NUM_OF_AUX_REGISTER_GROUPS] = {
        0x000C, // RDAUXA
        0x000E, // RDAUXB
        0x000D, // RDAUXC
        0x000F, // RDAUXD
    };

static float thermistor_resistances[NUM_OF_CELL_MONITOR_ICS]
                                   [NUM_OF_THERMISTORS_PER_IC];

/**
 *
 * @param current_ic
 * @param current_register_group
 * @param rx_aux_measurement
 * @return
 */
static ExitCode Io_Thermistors_ParseAuxMeasurementsAndPerformPec15Check(
    size_t                      current_ic,
    enum AuxiliaryRegisterGroup current_register_group,
    uint8_t *                   rx_aux_measurement);

static ExitCode Io_Thermistors_ParseAuxMeasurementsAndPerformPec15Check(
    size_t                      current_ic,
    enum AuxiliaryRegisterGroup current_register_group,
    uint8_t *                   rx_aux_measurement)
{
    size_t      aux_measurement_index = current_ic * NUM_OF_RX_BYTES;
    const float bias_resistor_ohms    = 10000.0f;
    const float reference_voltage     = 3.0f;

    for (size_t current_cell = 0U;
         current_cell < NUM_OF_AUX_MEASUREMENTS_PER_REGISTER_GROUP;
         current_cell++)
    {
        // The thermistor resistance (Ohms) can be calculated by the following
        // equation:
        //
        //                      MEASURED_V * BIAS_RESISTOR_OHMS
        // THERMISTOR_OHMS = -------------------------------------
        //                        (REFERENCE_V - MEASURED_V)
        //
        // BIAS_RESISTOR_OHMS = 10kΩ
        //
        // REFERENCE_V = 3.0V
        //

        const uint32_t measured_aux_voltage =
            (uint32_t)(rx_aux_measurement[aux_measurement_index]) |
            (uint32_t)((rx_aux_measurement[aux_measurement_index + 1] << 8));

        // The measured aux voltage is divided by 10000 in the following
        // calculation since it is read back in 100µV.
        const float thermistor_resistance =
            (float)measured_aux_voltage * bias_resistor_ohms /
            (reference_voltage - (float)measured_aux_voltage / 10000.0f);

        thermistor_resistances[current_ic]
                              [current_cell +
                               current_register_group *
                                   NUM_OF_AUX_MEASUREMENTS_PER_REGISTER_GROUP] =
                                  (float)thermistor_resistance;

        if (current_register_group == AUX_REGISTER_GROUP_D &&
            aux_measurement_index == 2U)
        {
            // Since 8 thermistor resistances are monitored for each accumulator
            // segment and there are 3 auxiliary measurements per auxiliary
            // register group, ignore the last thermistor reading read back from
            // AUX_REGISTER_GROUP_D. Increment the cell_voltage_index by 4 to
            // retrieve the PEC15 bytes for the current register group.
            aux_measurement_index += 4U;
            break;
        }
        else
        {
            // Each aux measurement is represented by 2 bytes. Therefore,
            // the cell voltage index is incremented by 2 to retrieve the next
            // cell voltage.
            aux_measurement_index += 2U;
        }
    }

    uint32_t received_pec15 =
        (uint32_t)(rx_aux_measurement[aux_measurement_index] << 8) |
        (uint32_t)(rx_aux_measurement[aux_measurement_index + 1]);

    // Calculate the PEC15 using the first 6 bytes of data received from the
    // chip.
    uint32_t calculated_pec15 = Io_LTC6813_CalculatePec15(
        &rx_aux_measurement[current_ic * NUM_OF_RX_BYTES], 6U);

    if (received_pec15 != calculated_pec15)
    {
        return EXIT_CODE_ERROR;
    }

    return EXIT_CODE_OK;
}

ExitCode Io_Thermistors_ReadThermistorResistancesOhms(void)
{
    uint16_t aux_register_group_cmd;
    uint8_t  tx_cmd[NUM_OF_CMD_BYTES];
    uint8_t
        rx_thermistor_resistances[NUM_OF_RX_BYTES * NUM_OF_CELL_MONITOR_ICS] = {
            0
        };

    RETURN_IF_EXIT_NOT_OK(Io_LTC6813_EnterReadyState())
    RETURN_IF_EXIT_NOT_OK(Io_LTC6813_StartAuxiliaryGPIOMeasurements())
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
            if (Io_Thermistors_ParseAuxMeasurementsAndPerformPec15Check(
                    current_ic, current_register_group,
                    rx_thermistor_resistances) != EXIT_CODE_OK)
            {
                return EXIT_CODE_ERROR;
            }
        }
    }

    return EXIT_CODE_OK;
}

float *Io_Thermistors_GetThermistorResistancesOhms(void)
{
    return &thermistor_resistances[0][0];
}
