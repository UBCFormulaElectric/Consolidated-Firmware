#include <string.h>
#include "hw_spis.h"
#include "ltc6813/io_ltc6813Shared.h"
#include "ltc6813/io_ltc6813CellVoltages.h"
#include "app_utils.h"

// Discharge permitted
#define DCP (0U)

// Cell selection for ADC conversion
#define CH (0U)

// ADC mode selection
#define MD (1U)

// ADOW mode selection
#define PUP_PU (1U) // Pull-up current
#define PUP_PD (0U) // Pull-down current

// Command used to start ADC conversions
#define ADCV ((uint16_t)(((0x0060U + (MD << 7U) + (DCP << 4U) + CH) << 8U) | 0x0002U))

// Commands used to read back cell voltages
#define RDCVA (0x0400U)
#define RDCVB (0x0600U)
#define RDCVC (0x0800U)
#define RDCVD (0x0A00U)
#define RDCVE (0x0900U)
#define RDCVF (0x0B00U)

// Command used to start open wire check with pull-up current
#define ADOW_PU ((uint16_t)(((0x028U + (MD << 7U) + (PUP_PU << 6U) + (DCP << 4U) + CH) << 8U) | 0x0002U))

// Command used to start open wire check with pull-down current
#define ADOW_PD ((uint16_t)(((0x028U + (MD << 7U) + (PUP_PD << 6U) + (DCP << 4U) + CH) << 8U) | 0x0002U))

// Command used to start Filtered open wire check with pull-up current (MD1 = 1, MD0 = 1)
#define ADOW_PU_FIL ((uint16_t)(((0x028U + (MD << 7U) + (PUP_PU << 6U) + (DCP << 4U) + CH) << 8U) | 0x0003U))

// Command used to start Filtered open wire check with pull-up current (MD1 = 1, MD0 = 1)
#define ADOW_PD_FIL ((uint16_t)(((0x028U + (MD << 7U) + (PUP_PD << 6U) + (DCP << 4U) + CH) << 8U) | 0x0003U))

// For cell 0, cell 0 is open if V_PU(0) = 0V
#define OPEN_WIRE_CHECK_CELL_0_THRESHOLD_V (0.1f) // 100mV

// For cell N in 1-15, cell N is open if V_PU(N+1) - V_PD(N+1) < -400mV
// * V_PU(N) is pull-up voltage of cell N, i.e. result of ADOW with PUP set to 1
// * V_PD(N) is pull-down voltage of cell N, i.e. result of ADOW with PUP set to 0
#define OPEN_WIRE_CHECK_CELL_N_THRESHOLD_V (-0.4f) // -400mV

// clang-format on

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

static float cell_voltages[ACCUMULATOR_NUM_SEGMENTS][ACCUMULATOR_NUM_SERIES_CELLS_PER_SEGMENT];

// Open Wire Check voltage measurements
static float owc_cell_V_PU[ACCUMULATOR_NUM_SEGMENTS][ACCUMULATOR_NUM_SERIES_CELLS_PER_SEGMENT]; // Pull-up Measurement
static float owc_cell_V_PD[ACCUMULATOR_NUM_SEGMENTS][ACCUMULATOR_NUM_SERIES_CELLS_PER_SEGMENT]; // Pull-down Measurement
static bool  owc_status[ACCUMULATOR_NUM_SEGMENTS][ACCUMULATOR_NUM_SERIES_CELLS_PER_SEGMENT] = { 0U };
static bool  owc_fault;

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
static bool parseCellVoltageFromAllSegments(
    float                    voltages[ACCUMULATOR_NUM_SEGMENTS][ACCUMULATOR_NUM_SERIES_CELLS_PER_SEGMENT],
    CellVoltageRegGroup      curr_reg_group,
    const LtcRegGroupPayload rx_buffer[ACCUMULATOR_NUM_SEGMENTS])
{
    bool status = true;

    for (uint8_t curr_segment = 0U; curr_segment < ACCUMULATOR_NUM_SEGMENTS; curr_segment++)
    {
        if (!io_ltc6813Shared_checkRegGroupPec15(&rx_buffer[curr_segment]))
        {
            status = false;
            continue;
        }

        for (uint8_t reading = 0U; reading < NUM_READINGS_PER_REG_GROUP; reading++)
        {
            // Write out voltage readings
            const float reading_voltage = CONVERT_100UV_TO_VOLTAGE(rx_buffer[curr_segment].words[reading]);

            const int reading_cell_idx = (uint8_t)curr_reg_group * NUM_READINGS_PER_REG_GROUP + reading;

            if (reading < ACCUMULATOR_NUM_SERIES_CELLS_PER_SEGMENT)
            {
                voltages[curr_segment][reading_cell_idx] = reading_voltage;
            }
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

    bool               status = true;
    LtcRegGroupPayload rx_buffer[ACCUMULATOR_NUM_SEGMENTS];

    for (uint8_t curr_reg_group = 0U; curr_reg_group < NUM_OF_CELL_V_REG_GROUPS; curr_reg_group++)
    {
        // Prepare the command used to read data back from a register group.
        if (!io_ltc6813Shared_readRegGroup(cv_read_cmds[curr_reg_group], rx_buffer))
        {
            status = false;
            continue;
        }

        // If SPI communication or parsing fails, save result but continue to update data for remaining cell register
        // groups.
        status &= parseCellVoltageFromAllSegments(cell_voltages, curr_reg_group, rx_buffer);
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

bool io_ltc6813CellVoltages_owcStart(bool pull_up)
{
    if (pull_up)
    {
        return io_ltc6813Shared_sendCommand(ADOW_PU_FIL);
    }
    else
    {
        return io_ltc6813Shared_sendCommand(ADOW_PD_FIL);
    }
}

bool io_ltc6813CellVoltages_owcReadVoltages(bool pull_up)
{
    // Exit early if ADC conversion fails
    if (!io_ltc6813Shared_pollAdcConversions())
    {
        return false;
    }

    bool               status = true;
    LtcRegGroupPayload rx_buffer[ACCUMULATOR_NUM_SEGMENTS];

    for (uint8_t curr_reg_group = 0U; curr_reg_group < NUM_OF_CELL_V_REG_GROUPS; curr_reg_group++)
    {
        // Transmit the command and receive data stored in register group.
        if (!io_ltc6813Shared_readRegGroup(cv_read_cmds[curr_reg_group], rx_buffer))
        {
            status = false;
            continue;
        }

        // If SPI communication or parsing fails, save result but continue to update data for remaining cell register
        // groups.
        status &= parseCellVoltageFromAllSegments(pull_up ? owc_cell_V_PU : owc_cell_V_PD, curr_reg_group, rx_buffer);
    }

    return status;
}

void io_ltc6813CellVoltages_checkOpenWireStatus(void)
{
    float cell_voltage_delta[ACCUMULATOR_NUM_SEGMENTS][ACCUMULATOR_NUM_SERIES_CELLS_PER_SEGMENT];

    for (int segment = 0; segment < ACCUMULATOR_NUM_SEGMENTS; segment++)
    {
        // Check for cell 0
        owc_status[segment][0] = (owc_cell_V_PU[segment][0] < OPEN_WIRE_CHECK_CELL_0_THRESHOLD_V);
        owc_fault |= owc_status[segment][0];

        // Check for cell 1 to 14
        for (uint8_t cell = 1; cell < ACCUMULATOR_NUM_SERIES_CELLS_PER_SEGMENT - 1; cell++)
        {
            cell_voltage_delta[segment][cell] = owc_cell_V_PD[segment][cell] - owc_cell_V_PD[segment][cell];
            owc_status[segment][cell]         = cell_voltage_delta[segment][cell] < OPEN_WIRE_CHECK_CELL_N_THRESHOLD_V;

            // Secondary Test discovered during testing
            owc_status[segment][cell] |=
                ((APPROX_EQUAL(owc_cell_V_PD[segment][cell], owc_cell_V_PD[segment][cell], 0.01)) &&
                 (owc_cell_V_PD[segment][cell] > 5.0f));

            owc_fault |= owc_status[segment][cell];
        }

        // Check for cell 15
        owc_status[segment][15] = (owc_cell_V_PD[segment][15] < OPEN_WIRE_CHECK_CELL_0_THRESHOLD_V);
        owc_fault |= owc_status[segment][15];
    }
}

bool io_ltc6813CellVoltages_getGlobalOpenWireFault(void)
{
    return owc_fault;
}

bool io_ltc6813CellVoltages_getOpenWireFault(uint8_t segment, uint8_t cell)
{
    return owc_status[segment][cell];
}
