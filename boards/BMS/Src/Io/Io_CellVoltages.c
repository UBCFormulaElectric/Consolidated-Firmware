#include "Io_SharedSpi.h"
#include "Io_LTC6813.h"
#include "Io_CellVoltages.h"
#include "App_Accumulator.h"

#define NUM_OF_CELLS_PER_SEGMENT (16U)
#define NUM_OF_CELLS_PER_REG_GROUP (3U)
#define TOTAL_NUM_OF_CELLS \
    (NUM_OF_CELLS_PER_SEGMENT * NUM_OF_ACCUMULATOR_SEGMENTS)

// Command used to start ADC conversions
#define MD (1U)
#define DCP (0U)
#define CH (0U)
#define ADCV ((0x260 + (MD << 7) + (DCP << 4) + CH))

// Used to store read commands for each cell register group
#define CELL_V_REG_GROUP_READ_CMD                                     \
    {                                                                 \
        [CELL_V_REG_GROUP_A] = 0x0004, [CELL_V_REG_GROUP_B] = 0x0006, \
        [CELL_V_REG_GROUP_C] = 0x0008, [CELL_V_REG_GROUP_D] = 0x000A, \
        [CELL_V_REG_GROUP_E] = 0x0009, [CELL_V_REG_GROUP_F] = 0x000B  \
    }

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
    uint16_t cell[NUM_OF_ACCUMULATOR_SEGMENTS][NUM_OF_CELLS_PER_SEGMENT];
    uint32_t segment[NUM_OF_ACCUMULATOR_SEGMENTS];
    uint32_t pack;

    struct
    {
        uint8_t  segment;
        uint8_t  cell;
        uint16_t voltage;
    } min, max;
};

extern struct SharedSpi *ltc6813_spi;
static struct Voltages   voltages = { 0U };

/**
 * Parse voltages stored in the given register group for the given segment
 * @param curr_segment The current segment to read voltages from
 * @param curr_reg_group The register group to read voltages from
 * @param rx_cell_v Ptr to an array to store raw cell voltages
 * @return True if raw cell voltages can be read back uncorrupted. Else, false.
 */
INLINE_FORCE static bool Io_ParseRegGroupFromSingleSegment(
    uint8_t  curr_segment,
    uint8_t  curr_reg_group,
    uint8_t *rx_cell_v)
{
    bool    status       = false;
    uint8_t cell_v_index = (uint8_t)(curr_segment * NUM_REG_GROUP_BYTES);

    // Check if received PEC15 is equal to calculated PEC15
    const uint16_t recv_pec15 = BYTES_TO_WORD(
        rx_cell_v[cell_v_index + REG_GROUP_PEC0],
        rx_cell_v[cell_v_index + REG_GROUP_PEC1]);
    const uint16_t calc_pec15 =
        Io_LTC6813_CalculateRegGroupPec15(&rx_cell_v[cell_v_index]);

    if (recv_pec15 == calc_pec15)
    {
        for (uint8_t curr_cell = 0U; curr_cell < NUM_OF_CELLS_PER_REG_GROUP;
             curr_cell++)
        {
            // Store cell voltages
            const uint16_t cell_voltage = BYTES_TO_WORD(
                rx_cell_v[cell_v_index + 1], rx_cell_v[cell_v_index]);
            voltages
                .cell[curr_segment]
                     [curr_cell + curr_reg_group * NUM_OF_CELLS_PER_REG_GROUP] =
                cell_voltage;

            if (curr_reg_group == CELL_V_REG_GROUP_F)
            {
                // Ignore the last two cell voltages in register group F since
                // we are reading back the first 16 cell voltages
                break;
            }
            else
            {
                // Each cell voltage is represented by 2 bytes. Increment by 2
                // bytes to get the next cell voltage in the register group
                cell_v_index =
                    (uint8_t)(cell_v_index + NUM_BYTES_REG_GROUP_DATA);
            }
        }

        status = true;
    }

    return status;
}

/**
 * Parse cell voltages from the given register group from every segment
 * @param curr_reg_group The register group to read back from for every segment
 * @param rx_cell_v Ptr to an array used to store raw cell voltages
 * @return True if cell voltages read back for each segment successfully. Else,
 * false
 */
INLINE_FORCE static bool Io_ParseRegGroupDataFromAllSegments(
    uint8_t  curr_reg_group,
    uint8_t *rx_cell_v)
{
    for (uint8_t curr_segment = 0U; curr_segment < NUM_OF_ACCUMULATOR_SEGMENTS;
         curr_segment++)
    {
        if (!Io_ParseRegGroupFromSingleSegment(
                curr_segment, curr_reg_group, rx_cell_v))
        {
            return false;
        }
    }

    return true;
}

/**
 * Reset min, max voltage and location as well as pack and segment voltages
 */
static void Io_ResetVoltages(void)
{
    // Reset each segment voltage
    for (uint8_t curr_segment = 0U; curr_segment < NUM_OF_ACCUMULATOR_SEGMENTS;
         curr_segment++)
    {
        voltages.segment[curr_segment] = 0U;
    }

    // Reset the pack voltage
    voltages.pack = 0U;

    // Reset max voltage and and max voltage location
    voltages.min.voltage = 0xFFFFU;
    voltages.min.cell    = 0U;
    voltages.min.segment = 0U;

    // Reset max voltage and and max voltage location
    voltages.max.voltage = 0U;
    voltages.max.cell    = 0U;
    voltages.max.segment = 0U;
}

/**
 * Update min, max, segment and pack voltages
 */
static void Io_UpdateVoltages(void)
{
    // Reset min, max, segment, pack voltages
    Io_ResetVoltages();

    // Loop through each segment and cell to get: min and max voltages, segment
    // voltage, pack voltage
    for (uint8_t curr_segment = 0U; curr_segment < NUM_OF_ACCUMULATOR_SEGMENTS;
         curr_segment++)
    {
        for (uint8_t curr_cell = 0U; curr_cell < NUM_OF_CELLS_PER_SEGMENT;
             curr_cell++)
        {
            const uint16_t curr_cell_voltage =
                voltages.cell[curr_segment][curr_cell];

            // Get the current min voltage, segment, and cell
            if (curr_cell_voltage < voltages.min.voltage)
            {
                voltages.min.voltage = curr_cell_voltage;
                voltages.min.segment = curr_segment;
                voltages.min.cell    = curr_cell;
            }

            // Get the current max voltage, segment, and cell
            if (curr_cell_voltage > voltages.max.voltage)
            {
                voltages.max.voltage = curr_cell_voltage;
                voltages.max.segment = curr_segment;
                voltages.max.cell    = curr_cell;
            }

            // Get the voltage of a given segment
            voltages.segment[curr_segment] += curr_cell_voltage;
        }

        // Calculate the pack voltage
        voltages.pack += voltages.segment[curr_segment];
    }
}

bool Io_CellVoltages_ReadVoltages(void)
{
    if (!Io_LTC6813_PollAdcConversions())
    {
        return false;
    }

    const uint16_t cv_read_cmds[NUM_OF_CELL_V_REG_GROUPS] =
        CELL_V_REG_GROUP_READ_CMD;
    uint8_t rx_buffer[TOTAL_NUM_OF_REG_BYTES] = { 0U };

    for (uint8_t curr_reg_group = 0U; curr_reg_group < NUM_OF_CELL_V_REG_GROUPS;
         curr_reg_group++)
    {
        uint16_t tx_cmd[NUM_OF_CMD_WORDS] = {
            [CMD_WORD] = cv_read_cmds[curr_reg_group], [CMD_PEC15] = 0U
        };

        // Prepare the command used to read data back from a register group
        Io_LTC6813_PrepareCmd(tx_cmd);

        // Transmit the command and receive data stored in register group
        if (Io_SharedSpi_TransmitAndReceive(
                ltc6813_spi, (uint8_t *)tx_cmd, TOTAL_NUM_CMD_BYTES, rx_buffer,
                TOTAL_NUM_OF_REG_BYTES))
        {
            // Get the voltage from each register group for a given segment
            if (!Io_ParseRegGroupDataFromAllSegments(curr_reg_group, rx_buffer))
            {
                return false;
            }
        }
    }

    // Calculate the min, max, segment and pack voltage
    Io_UpdateVoltages();

    return true;
}

bool Io_CellVoltages_StartAdcConversion(void)
{
    return Io_LTC6813_SendCommand(ADCV);
}

void Io_CellVoltages_GetMinCellLocation(uint8_t *segment, uint8_t *cell)
{
    *segment = voltages.min.segment;
    *cell    = voltages.min.cell;
}

void Io_CellVoltages_GetMaxCellLocation(uint8_t *segment, uint8_t *cell)
{
    *segment = voltages.max.segment;
    *cell    = voltages.max.cell;
}

float Io_CellVoltages_GetMinCellVoltage(void)
{
    return (float)(voltages.min.voltage) * V_PER_100UV;
}

float Io_CellVoltages_GetMaxCellVoltage(void)
{
    return (float)(voltages.max.voltage) * V_PER_100UV;
}

float Io_CellVoltages_GetPackVoltage(void)
{
    return (float)(voltages.pack) * V_PER_100UV;
}

float Io_CellVoltages_GetSegmentVoltage(AccumulatorSegments_E segment)
{
    return (float)(voltages.segment[segment]) * V_PER_100UV;
}

float Io_CellVoltages_GetAvgCellVoltage(void)
{
    return ((float)voltages.pack * V_PER_100UV) / TOTAL_NUM_OF_CELLS;
}
