#include <string.h>
#include "Io_SharedSpi.h"
#include "Io_LTC6813/Io_LTC6813Shared.h"
#include "Io_LTC6813/Io_LTC6813CellVoltages.h"
#include "App_SharedMacros.h"

// clang-format off
#define NUM_OF_CELLS_PER_SEGMENT   (16U)
#define TOTAL_NUM_OF_CELLS         (NUM_OF_CELLS_PER_SEGMENT * NUM_OF_ACCUMULATOR_SEGMENTS)
#define NUM_OF_CELLS_PER_REG_GROUP (3U)

// Command used to start ADC conversions
#define MD   (1U)
#define DCP  (0U)
#define CH   (0U)
#define ADCV (((0x0060U + (MD << 7) + (DCP << 4) + CH) << 8) | 0x0002U)

// Commands used to read back cell voltages
#define RDCVA (0x0400U)
#define RDCVB (0x0600U)
#define RDCVC (0x0800U)
#define RDCVD (0x0A00U)
#define RDCVE (0x0900U)
#define RDCVF (0x0B00U)

#define CONVERT_100UV_TO_VOLTAGE(v_100uv) ((float)v_100uv * V_PER_100UV)
// clang-format on

enum CellVoltageRegGroup
{
    CELL_V_REG_GROUP_A = 0U,
    CELL_V_REG_GROUP_B,
    CELL_V_REG_GROUP_C,
    CELL_V_REG_GROUP_D,
    CELL_V_REG_GROUP_E,
    CELL_V_REG_GROUP_F,
    NUM_OF_CELL_V_REG_GROUPS
};

struct Voltages
{
    struct
    {
        uint8_t  segment;
        uint8_t  cell;
        uint16_t voltage;
    } min, max;

    uint32_t segment[NUM_OF_ACCUMULATOR_SEGMENTS];
    uint32_t pack;
};

extern struct SharedSpi *ltc6813_spi;

static const uint16_t cv_read_cmds[NUM_OF_CELL_V_REG_GROUPS] = {
    [CELL_V_REG_GROUP_A] = RDCVA, [CELL_V_REG_GROUP_B] = RDCVB,
    [CELL_V_REG_GROUP_C] = RDCVC, [CELL_V_REG_GROUP_D] = RDCVD,
    [CELL_V_REG_GROUP_E] = RDCVE, [CELL_V_REG_GROUP_F] = RDCVF,
};

static struct Voltages voltages = { 0U };
static uint16_t        cell_voltages[NUM_OF_ACCUMULATOR_SEGMENTS]
                             [NUM_OF_CELL_V_REG_GROUPS]
                             [NUM_OF_CELLS_PER_REG_GROUP] = { 0U };

/**
 * A function that can be called to update min/max cell voltages, segment
 * voltages, and pack voltage
 */
static void Io_UpdateCellVoltages(void);

/**
 * Parse cell voltage from all segments to be stored in cell_voltages array
 * @param curr_reg_group The current register group to read cell voltages from
 * @param rx_buffer The buffer containing data read from the LTC6813 chips
 * @return True if the data is read back successfully. Else, false
 */
static bool Io_ParseCellVoltageFromAllSegments(
    uint8_t  curr_reg_group,
    uint16_t rx_buffer[TOTAL_NUM_OF_REG_GROUP_WORDS]);

static void Io_UpdateCellVoltages(void)
{
    struct Voltages temp_voltages = {
        .min     = { .segment = 0U, .cell = 0U, .voltage = UINT16_MAX },
        .max     = { .segment = 0U, .cell = 0U, .voltage = 0x0000U },
        .pack    = 0U,
        .segment = { 0U },
    };

    for (uint8_t curr_segment = 0U; curr_segment < NUM_OF_ACCUMULATOR_SEGMENTS;
         curr_segment++)
    {
        for (uint8_t curr_reg_group = 0U;
             curr_reg_group < NUM_OF_CELL_V_REG_GROUPS; curr_reg_group++)
        {
            for (uint8_t curr_cell = 0U; curr_cell < NUM_OF_CELLS_PER_REG_GROUP;
                 curr_cell++)
            {
                const uint16_t curr_cell_voltage =
                    cell_voltages[curr_segment][curr_reg_group][curr_cell];
                const uint8_t curr_cell_index = (uint8_t)(
                    curr_reg_group * NUM_OF_CELLS_PER_REG_GROUP + curr_cell);

                // Get the minimum cell voltage
                if (curr_cell_voltage < temp_voltages.min.voltage)
                {
                    temp_voltages.min.voltage = curr_cell_voltage;
                    temp_voltages.min.segment = curr_segment;
                    temp_voltages.min.cell    = curr_cell_index;
                }

                // Get the maximum cell voltage
                if (curr_cell_voltage > temp_voltages.max.voltage)
                {
                    temp_voltages.max.voltage = curr_cell_voltage;
                    temp_voltages.max.segment = curr_segment;
                    temp_voltages.max.cell    = curr_cell_index;
                }

                temp_voltages.segment[curr_segment] += curr_cell_voltage;

                // Only reading back cell 16 from register group F
                if (curr_reg_group == CELL_V_REG_GROUP_F)
                {
                    break;
                }
            }
        }

        temp_voltages.pack += temp_voltages.segment[curr_segment];
    }

    // Update voltages
    voltages = temp_voltages;
}

static bool Io_ParseCellVoltageFromAllSegments(
    uint8_t  curr_reg_group,
    uint16_t rx_buffer[TOTAL_NUM_OF_REG_GROUP_WORDS])
{
    for (uint8_t curr_segment = 0U; curr_segment < NUM_OF_ACCUMULATOR_SEGMENTS;
         curr_segment++)
    {
        // Set the starting index to read cell voltages for the current segment
        // from rx_buffer
        uint8_t start_index =
            (uint8_t)(curr_segment * NUM_REG_GROUP_PACKET_WORDS);

        // Calculate PEC15 from the data received on rx_buffer
        const uint16_t calc_pec15 = Io_LTC6813Shared_CalculateRegGroupPec15(
            (uint8_t *)&rx_buffer[start_index]);

        // Read PEC15 from the rx_buffer
        const uint16_t recv_pec15 =
            rx_buffer[start_index + REG_GROUP_WORD_PEC_INDEX];

        if (recv_pec15 == calc_pec15)
        {
            // Ignore cells 17 and 18 if reading back from group F. Only read
            // single word. Otherwise read all bytes stored in rx_buffer
            const uint8_t num_bytes_to_copy =
                (curr_reg_group == CELL_V_REG_GROUP_F)
                    ? sizeof(*rx_buffer)
                    : NUM_REG_GROUP_PAYLOAD_BYTES;

            // Store register group data into the cell voltage array
            memcpy(
                (uint8_t *)&cell_voltages[curr_segment][curr_reg_group],
                (uint8_t *)&rx_buffer[start_index], num_bytes_to_copy);
        }
        else
        {
            return false;
        }
    }

    return true;
}

bool Io_LTC6813CellVoltages_ReadVoltages(void)
{
    bool status = true;

    if (Io_LTC6813Shared_PollAdcConversions())
    {
        uint16_t rx_buffer[TOTAL_NUM_OF_REG_GROUP_WORDS] = { 0U };

        for (uint8_t curr_reg_group = 0U;
             curr_reg_group < NUM_OF_CELL_V_REG_GROUPS; curr_reg_group++)
        {
            // Prepare the command used to read data back from a register group
            uint16_t tx_cmd[NUM_OF_CMD_WORDS] = {
                [CMD_WORD] = cv_read_cmds[curr_reg_group], [CMD_PEC15] = 0U
            };
            Io_LTC6813Shared_PackCmdPec15(tx_cmd);

            // Transmit the command and receive data stored in register group.
            // If Io_ParseCellVoltagesFromAllSegments returns false, continue to
            // update data for remaining cell register groups
            if (!Io_SharedSpi_TransmitAndReceive(
                    ltc6813_spi, (uint8_t *)tx_cmd, TOTAL_NUM_CMD_BYTES,
                    (uint8_t *)rx_buffer, TOTAL_NUM_OF_REG_BYTES) ||
                !Io_ParseCellVoltageFromAllSegments(curr_reg_group, rx_buffer))
            {
                status = false;
            }
        }
    }
    else
    {
        status = false;
    }

    // Update min/max cell segment, index and voltages and update pack voltage
    // and segment voltages
    Io_UpdateCellVoltages();

    return status;
}

bool Io_LTC6813CellVoltages_StartAdcConversion(void)
{
    return Io_LTC6813Shared_SendCommand(ADCV);
}

void Io_LTC6813CellVoltages_GetMinCellLocation(uint8_t *segment, uint8_t *cell)
{
    *segment = voltages.min.segment;
    *cell    = voltages.min.cell;
}

void Io_LTC6813CellVoltages_GetMaxCellLocation(uint8_t *segment, uint8_t *cell)
{
    *segment = voltages.max.segment;
    *cell    = voltages.max.cell;
}

float Io_LTC6813CellVoltages_GetMinCellVoltage(void)
{
    return CONVERT_100UV_TO_VOLTAGE(voltages.min.voltage);
}

float Io_LTC6813CellVoltages_GetMaxCellVoltage(void)
{
    return CONVERT_100UV_TO_VOLTAGE(voltages.max.voltage);
}

float Io_LTC6813CellVoltages_GetPackVoltage(void)
{
    return CONVERT_100UV_TO_VOLTAGE(voltages.pack);
}

float Io_LTC6813CellVoltages_GetSegmentVoltage(AccumulatorSegments_E segment)
{
    return CONVERT_100UV_TO_VOLTAGE(voltages.segment[segment]);
}

float Io_LTC6813CellVoltages_GetAverageCellVoltage(void)
{
    return CONVERT_100UV_TO_VOLTAGE(voltages.pack) / TOTAL_NUM_OF_CELLS;
}
