#include <string.h>
#include "Io_SharedSpi.h"
#include "Io_LTC6813.h"
#include "Io_CellVoltages.h"
#include "App_Accumulator.h"

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
// static struct Voltages   voltages = { 0U };

static const uint16_t cv_read_cmds[NUM_OF_CELL_V_REG_GROUPS] = {
    [CELL_V_REG_GROUP_A] = RDCVA, [CELL_V_REG_GROUP_B] = RDCVB,
    [CELL_V_REG_GROUP_C] = RDCVC, [CELL_V_REG_GROUP_D] = RDCVD,
    [CELL_V_REG_GROUP_E] = RDCVE, [CELL_V_REG_GROUP_F] = RDCVF,
};

static uint16_t new_cv[NUM_OF_ACCUMULATOR_SEGMENTS][CELL_V_REG_GROUP_F]
                      [NUM_OF_CELLS_PER_REG_GROUP] = { 0U };

INLINE_FORCE static bool Io_ParseCellVoltageFromAllSegments(
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

        // Change the endianness of the data received on the rx buffer
        // from big endian to small endian
        Io_ChangeRxBufferSegmentEndianness(&rx_buffer[start_index]);

        // Read PEC15 from the rx_buffer
        const uint16_t recv_pec15 = rx_buffer[start_index + CMD_PEC15];

        if (recv_pec15 == calc_pec15)
        {
            // Store register group data into the cell voltage array
            memcpy(
                (uint8_t *)&new_cv[curr_segment][curr_reg_group],
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

            // Transmit the command and receive data stored in register group
            Io_LTC6813_PrepareCmd(tx_cmd);
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

    return status;
}

// bool Io_CellVoltages_StartAdcConversion(void)
//{
//    return Io_LTC6813_SendCommand(ADCV);
//}
//
// void Io_CellVoltages_GetMinCellLocation(uint8_t *segment, uint8_t *cell)
//{
//    *segment = voltages.min.segment;
//    *cell    = voltages.min.cell;
//}
//
// void Io_CellVoltages_GetMaxCellLocation(uint8_t *segment, uint8_t *cell)
//{
//    *segment = voltages.max.segment;
//    *cell    = voltages.max.cell;
//}
//
// float Io_CellVoltages_GetMinCellVoltage(void)
//{
//    return (float)(voltages.min.voltage) * V_PER_100UV;
//}
//
// float Io_CellVoltages_GetMaxCellVoltage(void)
//{
//    return (float)(voltages.max.voltage) * V_PER_100UV;
//}
//
// float Io_CellVoltages_GetPackVoltage(void)
//{
//    return (float)(voltages.pack) * V_PER_100UV;
//}
//
// float Io_CellVoltages_GetSegmentVoltage(AccumulatorSegments_E segment)
//{
//    return (float)(voltages.segment[segment]) * V_PER_100UV;
//}
//
// float Io_CellVoltages_GetAvgCellVoltage(void)
//{
//    return ((float)voltages.pack * V_PER_100UV) / TOTAL_NUM_OF_CELLS;
//}
