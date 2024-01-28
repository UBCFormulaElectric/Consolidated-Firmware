#include <string.h>
#include "hw_spi.h"
#include "ltc6813/io_ltc6813Shared.h"
#include "ltc6813/io_ltc6813CellVoltages.h"
#include "app_utils.h"

// Command used to start ADC conversions
#define ADCV ((uint16_t)(((0x0060U + (MD << 7U) + (DCP << 4U) + CH) << 8U) | 0x0002U))

// Commands used to read back cell voltages
#define RDCVA (0x0400U)
#define RDCVB (0x0600U)
#define RDCVC (0x0800U)
#define RDCVD (0x0A00U)
#define RDCVE (0x0900U)
#define RDCVF (0x0B00U)

typedef enum
{
    CELL_V_REG_GROUP_A = 0U,
    CELL_V_REG_GROUP_B,
    CELL_V_REG_GROUP_C,
    CELL_V_REG_GROUP_D,
    CELL_V_REG_GROUP_E,
    CELL_V_REG_GROUP_F,
    NUM_OF_CELL_V_REG_GROUPS
} CellVoltageRegGroup;

extern const SpiInterface *ltc6813_spi;

static float cell_voltages[ACCUMULATOR_NUM_SEGMENTS][ACCUMULATOR_NUM_SERIES_CELLS_PER_SEGMENT];

static const uint16_t cv_read_cmds[NUM_OF_CELL_V_REG_GROUPS] = {
    [CELL_V_REG_GROUP_A] = RDCVA, [CELL_V_REG_GROUP_B] = RDCVB, [CELL_V_REG_GROUP_C] = RDCVC,
    [CELL_V_REG_GROUP_D] = RDCVD, [CELL_V_REG_GROUP_E] = RDCVE, [CELL_V_REG_GROUP_F] = RDCVF,
};

/**
 * Parse voltages from a single register group (consists of 3x cell voltage measurements) from all segments.
 * @param curr_reg_group The current register group to read cell voltages from
 * @param rx_buffer The buffer containing data read from the LTC6813 chips
 * @param cell_voltages Buffer to write out the read voltages to
 * @return True if the data is read back successfully. Else, false
 */
static bool parseCellVoltageFromAllSegments(uint8_t curr_reg_group, uint16_t rx_buffer[NUM_REG_GROUP_RX_WORDS])
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
            // Ignore cells 17 and 18 if reading back from group F. Only read
            // single word. Otherwise read all bytes stored in rx_buffer
            const uint8_t num_bytes_to_copy =
                (curr_reg_group == CELL_V_REG_GROUP_F) ? sizeof(*rx_buffer) : NUM_REG_GROUP_PAYLOAD_BYTES;

            // Store register group data into a temporary array reg_group_data
            uint16_t reg_group_data[NUM_OF_READINGS_PER_REG_GROUP] = { 0 };
            memcpy((uint8_t *)reg_group_data, (uint8_t *)&rx_buffer[start_index], num_bytes_to_copy);

            // Write out voltage readings
            const float reading_0_voltage = CONVERT_100UV_TO_VOLTAGE(reg_group_data[REG_GROUP_READING_0]);
            const float reading_1_voltage = CONVERT_100UV_TO_VOLTAGE(reg_group_data[REG_GROUP_READING_1]);
            const float reading_2_voltage = CONVERT_100UV_TO_VOLTAGE(reg_group_data[REG_GROUP_READING_2]);

            const int reading_0_cell_idx = curr_reg_group * NUM_OF_READINGS_PER_REG_GROUP + REG_GROUP_READING_0;
            const int reading_1_cell_idx = curr_reg_group * NUM_OF_READINGS_PER_REG_GROUP + REG_GROUP_READING_1;
            const int reading_2_cell_idx = curr_reg_group * NUM_OF_READINGS_PER_REG_GROUP + REG_GROUP_READING_2;

            if (reading_0_cell_idx < (int)ACCUMULATOR_NUM_SERIES_CELLS_PER_SEGMENT)
            {
                cell_voltages[curr_segment][reading_0_cell_idx] = reading_0_voltage;
            }

            if (reading_1_cell_idx < (int)ACCUMULATOR_NUM_SERIES_CELLS_PER_SEGMENT)
            {
                cell_voltages[curr_segment][reading_1_cell_idx] = reading_1_voltage;
            }

            if (reading_2_cell_idx < (int)ACCUMULATOR_NUM_SERIES_CELLS_PER_SEGMENT)
            {
                cell_voltages[curr_segment][reading_2_cell_idx] = reading_2_voltage;
            }
        }
        else
        {
            status = false;
        }
    }

    return status;
}

bool io_ltc6813CellVoltages_readVoltages(void)
{
    // Exit early if ADC conversion fails
    if (!io_ltc6813Shared_pollAdcConversions())
    {
        return false;
    }

    bool     status                            = true;
    uint16_t rx_buffer[NUM_REG_GROUP_RX_WORDS] = { 0U };

    for (uint8_t curr_reg_group = 0U; curr_reg_group < NUM_OF_CELL_V_REG_GROUPS; curr_reg_group++)
    {
        // Prepare the command used to read data back from a register group
        uint16_t tx_cmd[NUM_OF_CMD_WORDS] = {
            [CMD_WORD]  = cv_read_cmds[curr_reg_group],
            [CMD_PEC15] = 0U,
        };
        io_ltc6813Shared_packCmdPec15(tx_cmd);

        // Transmit the command and receive data stored in register group.
        bool voltage_read_success = hw_spi_transmitAndReceive(
            ltc6813_spi, (uint8_t *)tx_cmd, TOTAL_NUM_CMD_BYTES, (uint8_t *)rx_buffer, NUM_REG_GROUP_RX_BYTES);
        voltage_read_success &= parseCellVoltageFromAllSegments(curr_reg_group, rx_buffer);

        // If SPI communication or parsing fails, save result but continue to update data for remaining cell register
        // groups
        status &= voltage_read_success;
    }

    return status;
}

bool io_ltc6813CellVoltages_startAdcConversion(void)
{
    return io_ltc6813Shared_sendCommand(ADCV);
}

float io_ltc6813CellVoltages_getCellVoltage(uint8_t segement, uint8_t cell)
{
    return cell_voltages[segement][cell];
}
