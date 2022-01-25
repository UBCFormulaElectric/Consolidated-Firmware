#include "Io_SharedSpi.h"
#include "Io_LTC6813.h"
#include "Io_CellVoltages.h"
#include "App_Accumulator.h"

#define NUM_OF_CELLS_PER_SEGMENT (16U)
#define NUM_OF_CELLS_PER_REG_GROUP (3U)
#define TOTAL_NUM_OF_CELLS \
    (NUM_OF_CELLS_PER_SEGMENT * NUM_OF_ACCUMULATOR_SEGMENTS)
#define TOTAL_NUM_OF_REG_BYTES (NUM_REG_BYTES * NUM_OF_ACCUMULATOR_SEGMENTS)

// Cell voltage read back is 2 bytes wide
#define RX_CELL_VOLTAGE_SIZE (2U)

// Conversion factor used to convert raw voltages (100µV) to voltages (V)
#define V_PER_100UV (1E-4f)

#define RESET_ACC_VOLTAGE(x)                                                   \
    (struct AccumulatorVoltages)                                               \
    {                                                                          \
        .pack_v = 0U, .segment_v = { 0U },                                     \
        .min_v = { .voltage = 0xFFFFU, .cell = 0U, .segment = 0U }, .max_v = { \
            .voltage = 0U,                                                     \
            .cell    = 0U,                                                     \
            .segment = 0U                                                      \
        }                                                                      \
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

struct CellVoltages
{
    uint8_t segment;
    uint8_t cell;
    float   voltage;
};

struct AccumulatorVoltages
{
    uint32_t            segment_v[NUM_OF_ACCUMULATOR_SEGMENTS];
    uint32_t            pack_v;
    struct CellVoltages min_v;
    struct CellVoltages max_v;
};

extern struct SharedSpi *ltc6813_spi;
extern uint16_t          cell_voltages[NUM_OF_ACCUMULATOR_SEGMENTS]
                             [NUM_OF_CELLS_PER_SEGMENT];
uint16_t cell_voltages[NUM_OF_ACCUMULATOR_SEGMENTS]
                      [NUM_OF_CELLS_PER_SEGMENT] = { { 0U } };

static const uint16_t cv_read_cmds[NUM_OF_CELL_V_REG_GROUPS] = {
    0x0004, // RDCVA
    0x0006, // RDCVB
    0x0008, // RDCVC
    0x000A, // RDCVD
    0x0009, // RDCVE
    0x000B  // RDCVF
};

static struct AccumulatorVoltages acc_voltages = { 0U };

/**
 * Parse raw cell voltages received from the cell monitoring chip and perform
 * PEC15 checks.
 * @param current_ic The current cell monitoring chip to parse cell voltages
 * for.
 * @param curr_reg_group The current register group on the given chip to
 * parse cell voltages for.
 * @param rx_cell_v The buffer containing the cell voltages read from the
 * cell monitoring chip.
 * @return True if the PEC15 check was successful. Else, false.
 */
static bool Io_CellVoltages_ParseSegmentRawVoltage(
    enum AccumulatorSegments current_ic,
    enum CellVoltageRegGroup curr_reg_group,
    uint8_t *                rx_cell_v);

static bool Io_CellVoltages_ParseSegmentRawVoltage(
    enum AccumulatorSegments current_ic,
    enum CellVoltageRegGroup curr_reg_group,
    uint8_t *                rx_cell_v)
{
    uint32_t cell_v_index = (uint32_t)(current_ic * NUM_REG_BYTES);

    for (uint8_t current_cell = 0U; current_cell < NUM_OF_CELLS_PER_REG_GROUP;
         current_cell++)
    {
        const uint16_t cell_voltage = (uint16_t)(
            rx_cell_v[cell_v_index] | (rx_cell_v[cell_v_index + 1] << 8));

        cell_voltages[current_ic]
                     [current_cell +
                      curr_reg_group * NUM_OF_CELLS_PER_REG_GROUP] =
                         cell_voltage;

        if (curr_reg_group == CELL_V_REG_GROUP_F)
        {
            // Since 16 cells are monitored for each accumulator segment and
            // there are 3 cell voltages per register group, ignore the last 2
            // cell voltages read back from CELL_VOLTAGE_REGISTER_F. The cell
            // voltage index is incremented by NUM_OF_REGS_IN_GROUP to retrieve
            // the PEC15 bytes for the register group.
            cell_v_index += NUM_OF_REGS_IN_GROUP;
            break;
        }
        else
        {
            // Each cell voltage is represented by 2 bytes. Therefore,
            // the cell voltage index is incremented by 2 to retrieve the next
            // cell voltage.
            cell_v_index += RX_CELL_VOLTAGE_SIZE;
        }
    }

    const uint16_t recv_pec15 = (uint16_t)(
        (rx_cell_v[cell_v_index] << 8) | (rx_cell_v[cell_v_index + 1]));
    const uint16_t calc_pec15 = Io_LTC6813_CalculatePec15(
        &rx_cell_v[current_ic * NUM_REG_BYTES], NUM_OF_REGS_IN_GROUP);

    // Compare calculated PEC15 with the PEC15 received
    return recv_pec15 == calc_pec15;
}

static void Io_CellVoltages_CalculateVoltages(void)
{
    // Reset acc_voltages
    acc_voltages = RESET_ACC_VOLTAGE();

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

bool Io_CellVoltages_GetAllRawCellVoltages(void)
{
    bool    status                                   = true;
    uint8_t tx_cmd[NUM_TX_CMD_BYTES]                 = { 0U };
    uint8_t rx_cell_voltages[TOTAL_NUM_OF_REG_BYTES] = { 0U };

    if (Io_LTC6813_PollAdcConversions())
    {
        for (enum CellVoltageRegGroup curr_reg_group = CELL_V_REG_GROUP_A;
             curr_reg_group < NUM_OF_CELL_V_REG_GROUPS; curr_reg_group++)
        {
            Io_LTC6813_PackCmd(tx_cmd, cv_read_cmds[curr_reg_group]);
            Io_LTC6813_PackPec15(tx_cmd, NUM_OF_CMD_BYTES);

            // Sending command to read back voltages from a register group of a
            // cell monitoring device
            if (Io_SharedSpi_TransmitAndReceive(
                    ltc6813_spi, tx_cmd, NUM_TX_CMD_BYTES, rx_cell_voltages,
                    TOTAL_NUM_OF_REG_BYTES))
            {
                for (enum AccumulatorSegments curr_segment = 0U;
                     curr_segment < NUM_OF_ACCUMULATOR_SEGMENTS; curr_segment++)
                {
                    if (!Io_CellVoltages_ParseSegmentRawVoltage(
                            curr_segment, curr_reg_group, rx_cell_voltages))
                    {
                        status = false;
                        break;
                    }
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

float Io_CellVoltages_GetSegmentVoltage(enum AccumulatorSegments segment)
{
    return (float)(acc_voltages.segment_v[segment]) * V_PER_100UV;
}

float Io_CellVoltages_GetAvgCellVoltage(void)
{
    return ((float)acc_voltages.pack_v * V_PER_100UV) / TOTAL_NUM_OF_CELLS;
}