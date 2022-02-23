#include "Io_SharedSpi.h"
#include "Io_LTC6813.h"
#include "Io_CellVoltages.h"
#include "App_Accumulator.h"

// clang-format off
#define NUM_OF_CELLS_PER_SEGMENT (16U)
#define NUM_OF_CELLS_PER_REG_GROUP (3U)
#define TOTAL_NUM_OF_CELLS (NUM_OF_CELLS_PER_SEGMENT * NUM_OF_ACCUMULATOR_SEGMENTS)

// A macro used to reset accumulator voltages to their default settings
#define RESET_ACCUMULATOR_VOLTAGE()                                 \
    (struct AccumulatorVoltages)                                    \
    {                                                               \
        .pack_v = 0U,                                               \
        .segment_v = { 0U },                                        \
        .min_v = { .voltage = 0xFFFFU, .cell = 0U, .segment = 0U }, \
        .max_v = { .voltage = 0U, .cell = 0U, .segment = 0U }       \
    }

// Command used to start ADC conversions
#define MD (1U)
#define DCP (0U)
#define CH (0U)
#define CHG (0U)
#define CHST (0U)
#define ADCV ((0x260 + (MD << 7) + (DCP << 4) + CH))

#define MAX_NUM_OF_TRIES (3U)

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

struct AccumulatorVoltages
{
    uint32_t segment_v[NUM_OF_ACCUMULATOR_SEGMENTS];
    uint32_t pack_v;
    struct CellVoltages
    {
        uint8_t  segment;
        uint8_t  cell;
        uint16_t voltage;
    } min_v, max_v;
};

extern struct SharedSpi *ltc6813_spi;
extern uint16_t cell_voltages[NUM_OF_ACCUMULATOR_SEGMENTS][NUM_OF_CELLS_PER_SEGMENT];
uint16_t cell_voltages[NUM_OF_ACCUMULATOR_SEGMENTS][NUM_OF_CELLS_PER_SEGMENT] = {{ 0U }};

static const uint16_t cv_read_cmds[NUM_OF_CELL_V_REG_GROUPS] =
{
    [CELL_V_REG_GROUP_A] = 0x0004, // RDCVA
    [CELL_V_REG_GROUP_B] = 0x0006, // RDCVB
    [CELL_V_REG_GROUP_C] = 0x0008, // RDCVC
    [CELL_V_REG_GROUP_D] = 0x000A, // RDCVD
    [CELL_V_REG_GROUP_E] = 0x0009, // RDCVE
    [CELL_V_REG_GROUP_F] = 0x000B  // RDCVF
};

static struct AccumulatorVoltages acc_voltages = { 0U };
// clang-format on

static bool Io_CellVoltages_ReadFromSingleSegment(
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
            cell_voltages[curr_segment]
                         [curr_cell +
                          curr_reg_group * NUM_OF_CELLS_PER_REG_GROUP] =
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

static bool
    Io_CellVoltages_ReadFromSegments(uint8_t curr_reg_group, uint8_t *rx_cell_v)
{
    bool status = true;

    for (uint8_t curr_segment = 0U; curr_segment < NUM_OF_ACCUMULATOR_SEGMENTS;
         curr_segment++)
    {
        if (!Io_CellVoltages_ReadFromSingleSegment(
                curr_segment, curr_reg_group, rx_cell_v))
        {
            status = false;
            break;
        }
    }

    return status;
}

static inline bool Io_CellVoltages_ReadVoltages(void)
{
    bool status = true;

    uint8_t tx_cmd[TOTAL_NUM_CMD_BYTES]              = { 0U };
    uint8_t rx_cell_voltages[TOTAL_NUM_OF_REG_BYTES] = { 0U };

    for (uint8_t curr_reg_group = 0U; curr_reg_group < NUM_OF_CELL_V_REG_GROUPS;
         curr_reg_group++)
    {
        Io_LTC6813_PrepareCmd(tx_cmd, cv_read_cmds[curr_reg_group]);
        if (Io_SharedSpi_TransmitAndReceive(
                ltc6813_spi, tx_cmd, TOTAL_NUM_CMD_BYTES, rx_cell_voltages,
                TOTAL_NUM_OF_REG_BYTES))
        {
            // Get the voltage from each register group for a given segment
            if (!Io_CellVoltages_ReadFromSegments(
                    curr_reg_group, rx_cell_voltages))
            {
                status = false;
                break;
            }
        }
    }

    return status;
}

static bool Io_CellVoltages_ParseSegmentRawVoltage(
    uint8_t  curr_reg_group,
    uint8_t *rx_cell_v)
{
    bool status = false;

    for (uint8_t curr_segment = 0U; curr_segment < NUM_OF_ACCUMULATOR_SEGMENTS;
         curr_segment++)
    {
        // There are max 6 segments. Therefore i can be stored as a
        // u8
        uint8_t i = (uint8_t)(curr_segment * NUM_REG_GROUP_BYTES);

        // Check if received PEC15 is equal to calculated PEC15
        const uint16_t recv_pec15 = BYTES_TO_WORD(
            rx_cell_v[i + REG_GROUP_PEC0], rx_cell_v[i + REG_GROUP_PEC1]);
        const uint16_t calc_pec15 =
            Io_LTC6813_CalculateRegGroupPec15(&rx_cell_v[i]);

        if (recv_pec15 == calc_pec15)
        {
            for (uint8_t curr_cell = 0U; curr_cell < NUM_OF_CELLS_PER_REG_GROUP;
                 curr_cell++)
            {
                // Store cell voltages
                const uint16_t cell_voltage =
                    BYTES_TO_WORD(rx_cell_v[i + 1], rx_cell_v[i]);
                cell_voltages[curr_segment]
                             [curr_cell +
                              curr_reg_group * NUM_OF_CELLS_PER_REG_GROUP] =
                                 cell_voltage;

                if (curr_reg_group != CELL_V_REG_GROUP_F)
                {
                    // Each cell voltage is represented by 2 bytes. Therefore,
                    // the cell voltage index is incremented by 2 to retrieve
                    // the next cell voltage.
                    i = (uint8_t)(i + NUM_BYTES_REG_GROUP_DATA);
                }
                else
                {
                    break;
                }
            }

            status = true;
        }
        else
        {
            status = false;
            break;
        }
    }

    return status;
}

static void Io_CellVoltages_CalculateVoltages(void)
{
    // Reset acc_voltages
    acc_voltages = RESET_ACCUMULATOR_VOLTAGE();

    // Loop through each segment and cell to get: min and max voltages, segment
    // voltage, pack voltage
    for (uint8_t curr_segment = 0U; curr_segment < NUM_OF_ACCUMULATOR_SEGMENTS;
         curr_segment++)
    {
        for (uint8_t curr_cell = 0U; curr_cell < NUM_OF_CELLS_PER_SEGMENT;
             curr_cell++)
        {
            const uint16_t curr_cell_v = cell_voltages[curr_segment][curr_cell];

            // Get the current min voltage, segment, and cell
            if (curr_cell_v < acc_voltages.min_v.voltage)
            {
                acc_voltages.min_v.voltage =
                    cell_voltages[curr_segment][curr_cell];
                acc_voltages.min_v.segment = curr_segment;
                acc_voltages.min_v.cell    = curr_cell;
            }

            // Get the current max voltage, segment, and cell
            if (curr_cell_v > acc_voltages.max_v.voltage)
            {
                acc_voltages.max_v.voltage =
                    cell_voltages[curr_segment][curr_cell];
                acc_voltages.max_v.segment = curr_segment;
                acc_voltages.max_v.cell    = curr_cell;
            }

            // Get the voltage of a given segment
            acc_voltages.segment_v[curr_segment] += curr_cell_v;
        }

        // Calculate the pack voltage
        acc_voltages.pack_v += acc_voltages.segment_v[curr_segment];
    }
}

bool Io_CellVoltages_StartAdcConversion(void)
{
    return Io_LTC6813_SendCommand(ADCV);
}

bool Io_CellVoltages_GetAllRawCellVoltages(void)
{
    bool    status                                   = true;
    uint8_t tx_cmd[TOTAL_NUM_CMD_BYTES]              = { 0U };
    uint8_t rx_cell_voltages[TOTAL_NUM_OF_REG_BYTES] = { 0U };

    if (Io_LTC6813_PollAdcConversions())
    {
        for (uint8_t curr_reg_group = 0U;
             curr_reg_group < NUM_OF_CELL_V_REG_GROUPS; curr_reg_group++)
        {
            Io_LTC6813_PrepareCmd(tx_cmd, cv_read_cmds[curr_reg_group]);

            // Sending command to read back voltages from a register group of a
            // cell monitoring device
            if (Io_SharedSpi_TransmitAndReceive(
                    ltc6813_spi, tx_cmd, TOTAL_NUM_CMD_BYTES, rx_cell_voltages,
                    TOTAL_NUM_OF_REG_BYTES))
            {
                if (!Io_CellVoltages_ParseSegmentRawVoltage(
                        curr_reg_group, rx_cell_voltages))
                {
                    status = false;
                    break;
                }
            }
        }

        // Calculate the min, max, segment and pack voltage
        Io_CellVoltages_CalculateVoltages();
    }

    return status;
}

void Io_CellVoltages_GetMinCellLocation(uint8_t *segment, uint8_t *cell)
{
    *segment = acc_voltages.min_v.segment;
    *cell    = acc_voltages.min_v.cell;
}

void Io_CellVoltages_GetMaxCellLocation(uint8_t *segment, uint8_t *cell)
{
    *segment = acc_voltages.max_v.segment;
    *cell    = acc_voltages.max_v.cell;
}

float Io_CellVoltages_GetMinCellVoltage(void)
{
    return (float)(acc_voltages.min_v.voltage) * V_PER_100UV;
}

float Io_CellVoltages_GetMaxCellVoltage(void)
{
    return (float)(acc_voltages.max_v.voltage) * V_PER_100UV;
}

float Io_CellVoltages_GetPackVoltage(void)
{
    return (float)(acc_voltages.pack_v) * V_PER_100UV;
}

float Io_CellVoltages_GetSegmentVoltage(AccumulatorSegments_E segment)
{
    return (float)(acc_voltages.segment_v[segment]) * V_PER_100UV;
}

float Io_CellVoltages_GetAvgCellVoltage(void)
{
    return ((float)acc_voltages.pack_v * V_PER_100UV) / TOTAL_NUM_OF_CELLS;
}
