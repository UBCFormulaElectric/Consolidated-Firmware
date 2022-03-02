#include <string.h>
#include "Io_SharedSpi.h"
#include "Io_LTC6813.h"
#include "Io_NewCellVoltages.h"

#define NUM_OF_CELLS_PER_REG_GROUP (3U)
#define NUM_OF_CELLS_PER_SEGMENT (16U)
#define TOTAL_NUM_OF_CELLS \
    (NUM_OF_CELLS_PER_SEGMENT * NUM_OF_ACCUMULATOR_SEGMENTS)

// Command used to start ADC conversions
#define MD (1U)
#define DCP (0U)
#define CH (0U)
#define ADCV ((0x260 + (MD << 7) + (DCP << 4) + CH))

// Commands used to read back cell voltages
#define RDCVA (0x0004U)
#define RDCVB (0x0006U)
#define RDCVC (0x0008U)
#define RDCVD (0x000AU)
#define RDCVE (0x0009U)
#define RDCVF (0x000BU)

enum RegGroupWordFormat
{
    REG_GROUP_WORD_PEC = 3U,
    NUM_REG_GROUP_WORDS,
};
#define TOTAL_NUM_OF_REG_WORDS \
    (NUM_REG_GROUP_WORDS * NUM_OF_ACCUMULATOR_SEGMENTS)

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
static uint16_t cell_voltages[NUM_OF_ACCUMULATOR_SEGMENTS][CELL_V_REG_GROUP_F]
                             [NUM_OF_CELLS_PER_REG_GROUP] = { 0U };

static inline float Io_ConvertToVoltage(uint32_t v_100uv)
{
    return (float)v_100uv * V_PER_100UV;
}

static void Io_UpdateCellVoltages(void)
{
    struct Voltages temp_voltages = { 0U };

    for (uint8_t curr_segment = 0U; curr_segment < NUM_OF_ACCUMULATOR_SEGMENTS;
         curr_segment++)
    {
        for (uint8_t curr_reg_group = 0U;
             curr_reg_group < NUM_OF_CELL_V_REG_GROUPS; curr_reg_group++)
        {
            for (uint8_t curr_cell = 0U; curr_cell < NUM_OF_CELLS_PER_REG_GROUP;
                 curr_cell++)
            {
                const uint8_t curr_cell_voltage = (uint8_t)(
                    cell_voltages[curr_segment][curr_reg_group][curr_cell]);
                const uint8_t curr_cell_index = (uint8_t)(
                    curr_reg_group * NUM_OF_CELLS_PER_REG_GROUP + curr_cell);

                // Get the minimum cell voltage
                if (curr_cell_voltage > voltages.max.cell)
                {
                    temp_voltages.min.voltage = curr_cell_voltage;
                    temp_voltages.min.segment = curr_segment;
                    temp_voltages.min.cell    = curr_cell_index;
                }

                // Get the maximum cell voltage
                if (curr_cell_voltage < voltages.min.cell)
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
    uint16_t rx_buffer[TOTAL_NUM_OF_REG_WORDS])
{
    for (uint8_t curr_segment = 0U; curr_segment < NUM_OF_ACCUMULATOR_SEGMENTS;
         curr_segment++)
    {
        // Set the starting index to read cell voltages for the current segment
        // from rx_buffer
        uint8_t start_index = (uint8_t)(curr_segment * NUM_REG_GROUP_WORDS);

        // Calculate PEC15 from the data received on rx_buffer
        const uint16_t calc_pec15 = Io_LTC6813_CalculateRegGroupPec15(
            (uint8_t *)&rx_buffer[start_index]);

        // Read PEC15 from the rx_buffer
        const uint16_t recv_pec15 =
            CHANGE_WORD_ENDIANNESS(rx_buffer[start_index + REG_GROUP_WORD_PEC]);

        if (recv_pec15 == calc_pec15)
        {
            // Store register group data into the cell voltage array
            memcpy(
                (uint8_t *)&cell_voltages[curr_segment][curr_reg_group],
                (uint8_t *)&rx_buffer[start_index], NUM_OF_BYTES_IN_REG_GROUP);
        }
        else
        {
            return false;
        }
    }

    return true;
}

bool Io_NewCellVoltages_ReadVoltages(void)
{
    bool status = false;

    if (Io_LTC6813_PollAdcConversions())
    {
        uint16_t rx_buffer[TOTAL_NUM_OF_REG_WORDS] = { 0U };

        for (uint8_t curr_reg_group = 0U;
             curr_reg_group < NUM_OF_CELL_V_REG_GROUPS; curr_reg_group++)
        {
            // Prepare the command used to read data back from a register group
            uint16_t tx_cmd[NUM_OF_CMD_WORDS] = {
                [CMD_WORD]  = cv_read_cmds[curr_reg_group],
                [CMD_PEC15] = 0U,
            };
            Io_LTC6813_PrepareCmd(tx_cmd);

            // Transmit the command and receive data stored in register group
            if (Io_SharedSpi_TransmitAndReceive(
                    ltc6813_spi, (uint8_t *)tx_cmd, TOTAL_NUM_CMD_BYTES,
                    (uint8_t *)rx_buffer, TOTAL_NUM_OF_REG_BYTES))
            {
                // Parse cell voltages from the current register group from all
                // segments
                if (!Io_ParseCellVoltageFromAllSegments(
                        curr_reg_group, rx_buffer))
                {
                    return false;
                }
            }
        }

        status = true;
    }

    // Update min/max cell segment, index and voltage
    // Update pack voltage and segment voltages
    Io_UpdateCellVoltages();

    return status;
}

bool Io_NewCellVoltages_StartAdcConversion(void)
{
    return Io_LTC6813_SendCommand(ADCV);
}

void Io_NewCellVoltages_GetMinCellLocation(uint8_t *segment, uint8_t *cell)
{
    *segment = voltages.min.segment;
    *cell    = voltages.min.cell;
}

void Io_NewCellVoltages_GetMaxCellLocation(uint8_t *segment, uint8_t *cell)
{
    *segment = voltages.max.segment;
    *cell    = voltages.max.cell;
}

float Io_NewCellVoltages_GetMinCellVoltage(void)
{
    return Io_ConvertToVoltage(voltages.min.voltage);
}

float Io_NewCellVoltages_GetMaxCellVoltage(void)
{
    return Io_ConvertToVoltage(voltages.max.voltage);
}

float Io_NewCellVoltages_GetPackVoltage(void)
{
    return Io_ConvertToVoltage(voltages.pack);
}

float Io_NewCellVoltages_GetSegmentVoltage(AccumulatorSegments_E segment)
{
    return Io_ConvertToVoltage(voltages.segment[segment]);
}

float Io_NewCellVoltages_GetAvgCellVoltage(void)
{
    return Io_ConvertToVoltage(voltages.pack) / TOTAL_NUM_OF_CELLS;
}
