#include "io_ltc6813.h"

#include "io_ltc6813_internal.h"
#include "hw_spis.h"

#include <assert.h>
#include <string.h>

/**
 * Clears the register groups which contain the cell voltage data
 * @return success of operation
 */
static bool clearCellRegisters()
{
#define CLRCELL (0x0711)
    return io_ltc6813_sendCommand(CLRCELL);
}

// TODO assert that for each speed that the ADCOPT is correct
bool io_ltc6813_startCellsAdcConversion(const ADCSpeed speed)
{
    if (!clearCellRegisters())
        return false;
    const uint16_t adc_speed_factor = speed & 0x3;
// Cell selection for ADC conversion
#define CH (000U)
// Discharge permitted
#define DCP (0U)
#define ADCV (0x260 | CH | DCP << 4 | adc_speed_factor)
    return io_ltc6813_sendCommand(ADCV);
}

/**
 * This functions works by iterating through all register groups, and for each register group asking each segment
 * what is the value of the register group in that segment
 */
void io_ltc6813_readVoltages(
    float cell_voltages[NUM_SEGMENTS][CELLS_PER_SEGMENT],
    bool  success[NUM_SEGMENTS][VOLTAGE_REGISTER_GROUPS])
{
    memset(success, false, NUM_SEGMENTS * VOLTAGE_REGISTER_GROUPS);
    memset(cell_voltages, 0, NUM_SEGMENTS * CELLS_PER_SEGMENT * sizeof(float));
    // Exit early if ADC conversion fails
    if (!io_ltc6813_pollAdcConversions())
    {
        return;
    }

    for (uint8_t curr_reg_group = 0U; curr_reg_group < VOLTAGE_REGISTER_GROUPS; curr_reg_group++)
    {
        // maps the register group number to the command to read that register group
#define RDCVA (0x04U)
#define RDCVB (0x06U)
#define RDCVC (0x08U)
#define RDCVD (0x0AU)
#define RDCVE (0x09U)
#define RDCVF (0x0BU)
        static const uint16_t cv_read_cmds[6] = { RDCVA, RDCVB, RDCVC, RDCVD, RDCVE, RDCVF };

        // Transmit the command and receive data stored in register group.
        const ltc6813_tx tx_cmd = io_ltc6813_build_tx_cmd(cv_read_cmds[curr_reg_group]);
        // as per table 40-45
        typedef struct __attribute__((__packed__))
        {
            uint16_t a;
            uint16_t b;
            uint16_t c;
            PEC      pec;
        } VoltageRegGroup;
        VoltageRegGroup rx_buffer[NUM_SEGMENTS];
        static_assert(sizeof(VoltageRegGroup) == 8);

        const bool voltage_read_success = hw_spi_transmitThenReceive(
            &ltc6813_spi, (uint8_t *)&tx_cmd, sizeof(tx_cmd), (uint8_t *)rx_buffer, sizeof(rx_buffer));
        if (!voltage_read_success)
        {
            continue;
        }

        for (uint8_t seg_idx = 0U; seg_idx < NUM_SEGMENTS; seg_idx++)
        {
            const VoltageRegGroup *seg_reg_group = &rx_buffer[(NUM_SEGMENTS - 1) - seg_idx];
            // Calculate PEC15 from the data received on rx_buffer
            if (!io_ltc6813_check_pec((uint8_t *)seg_reg_group, 6, &seg_reg_group->pec))
            {
                success[seg_idx][curr_reg_group] = false;
                continue;
            }
            // fuck it we already here
            success[seg_idx][curr_reg_group] = true;

            // Conversion factor used to convert raw voltages (100µV) to voltages (V)
#define V_PER_100UV (1E-4f)
#define CONVERT_100UV_TO_VOLTAGE(v_100uv) ((float)v_100uv * V_PER_100UV)
            cell_voltages[seg_idx][curr_reg_group * 3 + 0] = CONVERT_100UV_TO_VOLTAGE(seg_reg_group->a);
            cell_voltages[seg_idx][curr_reg_group * 3 + 1] = CONVERT_100UV_TO_VOLTAGE(seg_reg_group->b);
            if (curr_reg_group * 3 + 2 > 14)
                continue;
            cell_voltages[seg_idx][curr_reg_group * 3 + 2] = CONVERT_100UV_TO_VOLTAGE(seg_reg_group->c);
        }
    }
}
