#include "app_accumulator.h"
#include "app_canTx.h"
#include "app_canRx.h"
#include "app_canAlerts.h"
#include "app_math.h"
#include "ltc6813/io_ltc6813Shared.h"
#include "ltc6813/io_ltc6813CellVoltages.h"
#include "ltc6813/io_ltc6813CellTemps.h"

#include <string.h>
#include <float.h>

// Max number of PEC15 to occur before faulting
#define MAX_NUM_COMM_TRIES (3U)

// Number of open wire check commands (ADOW) to send before open wire check
#define OPEN_WIRE_CHECK_NUM_ADOW_CMDS (2)
// Number of cool down states after running OWC to let voltage settle down
#define OPEN_WIRE_CHECK_IDLE_CYCLES (4)

// Open Wire Check Modes
#define PULL_UP (1U)
#define PULL_DOWN (0U)

// Discharging cells continuously generates too much heat.
// So balance cells for 100 ticks (the cell monitoring code runs in the 100Hz task, so 100 ticks = 1s),
// then disable discharge for the next 100 ticks to keep temperatures manageable.
// Reducing the ticks to 1 on / 1 off causes noticeable flicker, so I've set it to 1s on / 1s off to be less
// distracting.
#define BALANCE_DEFAULT_FREQ (1)  // Hz
#define BALANCE_DEFAULT_DUTY (50) // %
#define BALANCE_TICKS_ON (100U)
#define BALANCE_TICKS_OFF (100U)

#define MAX_POWER_LIMIT_W (78e3f)
#define CELL_ROLL_OFF_TEMP_DEGC (40.0f)
#define CELL_FULLY_DERATED_TEMP (60.0f)

#define SEG4_CELL2_REG_GROUP (0U)
#define SEG4_CELL8_REG_GROUP (2U)
#define SEG4_CELL2_THERMISTOR (2U)
#define SEG4_CELL8_THERMISTOR (2U)

// Update the counter keeping track of the PEC15 error
#define UPDATE_PEC15_ERROR_COUNT(is_pec_ok, num_comm_tries) \
    if ((is_pec_ok))                                        \
    {                                                       \
        (num_comm_tries) = 0U;                              \
    }                                                       \
    else                                                    \
    {                                                       \
        (num_comm_tries)++;                                 \
    }

typedef enum
{
    GET_CELL_VOLTAGE_STATE = 0U,
    GET_CELL_TEMP_STATE,
} AccumulatorMonitorState;

typedef enum
{
    START_OPEN_WIRE_CHECK = 0U,
    GET_PU_CELL_VOLTAGE_STATE,
    GET_PD_CELL_VOLTAGE_STATE,
    CHECK_OPEN_WIRE_FAULT_STATE,
    IDLE_STATE,
} AccumulatorOpenWireCheckState;

typedef struct
{
    uint8_t segment;
    uint8_t cell;
    float   voltage;
} CellVoltage;

typedef struct
{
    CellVoltage min_voltage;
    CellVoltage max_voltage;
    float       segment_voltages[ACCUMULATOR_NUM_SEGMENTS];
    float       pack_voltage;
} VoltageStats;

typedef struct
{
    uint16_t owcStatus[ACCUMULATOR_NUM_SEGMENTS];
    bool     owcFaultGND[ACCUMULATOR_NUM_SEGMENTS];
    bool     owcGlobalFault;
} OWCFaults;

typedef struct
{
    // Cells information
    uint32_t                num_comm_tries;
    VoltageStats            voltage_stats;
    AccumulatorMonitorState state;

    // OWC information
    OWCFaults                     owc_faults;
    AccumulatorOpenWireCheckState owc_state;
    // Balancing information
    bool     balance_enabled;
    bool     cells_to_balance[ACCUMULATOR_NUM_SEGMENTS][ACCUMULATOR_NUM_SERIES_CELLS_PER_SEGMENT];
    uint32_t balance_pwm_ticks;
    bool     balance_pwm_high;
} Accumulator;

static Accumulator data;
static uint8_t     open_wire_pu_readings;
static uint8_t     open_wire_pd_readings;
static uint8_t     owc_idle_cycles;

static void app_accumulator_calculateVoltageStats(void)
{
    VoltageStats temp_voltage_stats = { .min_voltage      = { .segment = 0U, .cell = 0U, .voltage = FLT_MAX },
                                        .max_voltage      = { .segment = 0U, .cell = 0U, .voltage = 0.0f },
                                        .segment_voltages = { 0U },
                                        .pack_voltage     = 0U };

    // Find the min and max voltages
    for (uint8_t segment = 0U; segment < ACCUMULATOR_NUM_SEGMENTS; segment++)
    {
        for (uint8_t cell = 0U; cell < ACCUMULATOR_NUM_SERIES_CELLS_PER_SEGMENT; cell++)
        {
            // Collect each cell voltage to find the min/max
            const float cell_voltage = io_ltc6813CellVoltages_getCellVoltage(segment, cell);

            // Get the minimum cell voltage
            if (cell_voltage < temp_voltage_stats.min_voltage.voltage)
            {
                temp_voltage_stats.min_voltage.voltage = cell_voltage;
                temp_voltage_stats.min_voltage.segment = segment;
                temp_voltage_stats.min_voltage.cell    = cell;
            }

            // Get the maximum cell voltage
            if (cell_voltage > temp_voltage_stats.max_voltage.voltage)
            {
                temp_voltage_stats.max_voltage.voltage = cell_voltage;
                temp_voltage_stats.max_voltage.segment = segment;
                temp_voltage_stats.max_voltage.cell    = cell;
            }

            // Sum the voltages into a segment voltage
            temp_voltage_stats.segment_voltages[segment] += cell_voltage;
        }

        // Sum the segment voltages into a pack voltage
        temp_voltage_stats.pack_voltage += temp_voltage_stats.segment_voltages[segment];
    }

    data.voltage_stats = temp_voltage_stats;
}

void app_accumulator_calculateCellsToBalance(void)
{
    float target_voltage = data.voltage_stats.min_voltage.voltage + CELL_VOLTAGE_BALANCE_WINDOW_V;

    target_voltage = app_canRx_Debug_CellBalancingOverrideTarget_get()
                         ? app_canRx_Debug_CellBalancingOverrideTargetValue_get()
                         : target_voltage;
    for (uint8_t segment = 0U; segment < ACCUMULATOR_NUM_SEGMENTS; segment++)
    {
        for (uint8_t cell = 0U; cell < ACCUMULATOR_NUM_SERIES_CELLS_PER_SEGMENT; cell++)
        {
            const bool needs_discharging = (io_ltc6813CellVoltages_getCellVoltage(segment, cell) > target_voltage);
            io_ltc6813Shared_markCellForDischarge(segment, cell, needs_discharging);
        }
    }
}

void app_accumulator_balanceCells(void)
{
    if (!data.balance_enabled)
    {
        io_ltc6813Shared_disableBalance();
        return;
    }

    // Exit early if ADC conversion fails
    if (!io_ltc6813Shared_pollAdcConversions())
    {
        return;
    }

    // Write to configuration register to configure cell discharging
    io_ltc6813Shared_writeConfigurationRegisters(true);

    // Balance PWM settings
    float    balance_pwm_freq = app_canRx_Debug_CellBalancingOverridePWM_get()
                                    ? app_canRx_Debug_CellBalancingOverridePWMFrequency_get()
                                    : BALANCE_DEFAULT_FREQ;
    uint32_t balance_pwm_duty = app_canRx_Debug_CellBalancingOverridePWM_get()
                                    ? app_canRx_Debug_CellBalancingOverridePWMDuty_get()
                                    : BALANCE_DEFAULT_DUTY;

    // duty_on = 100_ticks_per_sec * 1/freq_Hz * duty_percent / 100
    // TODO: verify frequency calculation. Period seems to be about double what it should be.
    uint32_t balance_ticks_on  = (uint32_t)(1.0f / balance_pwm_freq * (float)balance_pwm_duty);
    uint32_t balance_ticks_off = (uint32_t)(1.0f / balance_pwm_freq * (float)(100 - balance_pwm_duty));

    if (data.balance_pwm_high)
    {
        // Enable cell discharging
        io_ltc6813Shared_enableBalance();
        data.balance_pwm_ticks += 1;

        if (data.balance_pwm_ticks >= balance_ticks_on)
        {
            // Cell discharging enabled duty cycle portion is finished
            data.balance_pwm_high  = false;
            data.balance_pwm_ticks = 0;
        }
    }
    else
    {
        // Disable cell discharging
        io_ltc6813Shared_disableBalance();
        data.balance_pwm_ticks += 1;

        if (data.balance_pwm_ticks >= balance_ticks_off)
        {
            // Cell discharging disabled duty cycle portion is finished
            data.balance_pwm_high  = true;
            data.balance_pwm_ticks = 0;
        }
    }
}

void app_accumulator_init(void)
{
    // Cell voltage monitoring functions
    data.num_comm_tries = 0U;

    // Voltage information
    memset(&data.voltage_stats, 0U, sizeof(VoltageStats));
    data.state = GET_CELL_VOLTAGE_STATE;

    // Balancing information
    memset(&data.cells_to_balance, 0U, sizeof(data.cells_to_balance));

    open_wire_pu_readings = 0;
    open_wire_pd_readings = 0;
    owc_idle_cycles       = 0;
    data.owc_state        = START_OPEN_WIRE_CHECK;
}

void app_accumulator_writeDefaultConfig(void)
{
    // Configure the cell monitoring chips. Disable discharge at startup
    io_ltc6813Shared_setCfgRegsToDefaultSettings();
}

void app_accumulator_runCellMeasurements(void)
{
    switch (data.state)
    {
        case GET_CELL_VOLTAGE_STATE:
        {
            // Attempt to read voltages from the LTCs, write output to cell voltages array
            UPDATE_PEC15_ERROR_COUNT(io_ltc6813CellVoltages_readVoltages(), data.num_comm_tries);

            // Calculate min/max/segment voltages
            app_accumulator_calculateVoltageStats();

            // Start cell voltage conversions for the next cycle
            io_ltc6813CellTemps_startAdcConversion();
            data.state = GET_CELL_TEMP_STATE;
            break;
        }
        case GET_CELL_TEMP_STATE:
        {
            UPDATE_PEC15_ERROR_COUNT(io_ltc6813CellTemps_readTemperatures(), data.num_comm_tries)

            // Start cell voltage conversions for the next cycle
            io_ltc6813CellVoltages_startAdcConversion();

            data.state = GET_CELL_VOLTAGE_STATE;
            break;
        }
        default:
        {
            break;
        }
    }
}

static void app_accumulator_owcCalculateFaults(void)
{
    OWCFaults owcFaults = { .owcStatus = { 0U }, .owcFaultGND = { 0U }, .owcGlobalFault = 0U };

    owcFaults.owcGlobalFault = io_ltc6813CellVoltages_getGlobalOpenWireFault();

    if (owcFaults.owcGlobalFault)
    {
        for (uint8_t segment = 0; segment < ACCUMULATOR_NUM_SEGMENTS; segment++)
        {
            if (io_ltc6813CellVoltages_getOpenWireFault(segment, 0))
            {
                owcFaults.owcFaultGND[segment] = true;
                owcFaults.owcStatus[segment]   = (uint16_t)1;
            }
            else
            {
                for (uint8_t cell = 1; cell < ACCUMULATOR_NUM_SERIES_CELLS_PER_SEGMENT; cell++)
                {
                    if (io_ltc6813CellVoltages_getOpenWireFault(segment, cell))
                    {
                        owcFaults.owcStatus[segment] |= ((uint16_t)(1 << cell));
                    }
                }
            }
        }
    }

    data.owc_faults = owcFaults;
}

bool app_accumulator_runOpenWireCheck(void)
{
    bool is_finished = false;

    switch (data.owc_state)
    {
        case START_OPEN_WIRE_CHECK:
        {
            // update the number of commands that've been run already before starting Open Wire Check
            open_wire_pu_readings = 0;
            open_wire_pd_readings = 0;
            owc_idle_cycles       = 0;

            // Set up or acquire the Mutex for iso-SPI
            bool Mutex_Acquired = true; // this line is just a reminder to fix it with proper code

            if (Mutex_Acquired)
            {
                io_ltc6813CellVoltages_owcStart(PULL_UP);
                open_wire_pu_readings++;
                data.owc_state = GET_PU_CELL_VOLTAGE_STATE;
            }
            break;
        }
        case GET_PU_CELL_VOLTAGE_STATE:
        {
            if (open_wire_pu_readings >= OPEN_WIRE_CHECK_NUM_ADOW_CMDS)
            {
                UPDATE_PEC15_ERROR_COUNT(io_ltc6813CellVoltages_owcReadVoltages(PULL_UP), data.num_comm_tries);

                io_ltc6813CellVoltages_owcStart(PULL_DOWN);
                open_wire_pd_readings++;
                data.owc_state = GET_PD_CELL_VOLTAGE_STATE;
            }
            else
            {
                io_ltc6813CellVoltages_owcStart(PULL_UP);
                open_wire_pu_readings++;
            }
            break;
        }
        case GET_PD_CELL_VOLTAGE_STATE:
        {
            if (open_wire_pd_readings >= OPEN_WIRE_CHECK_NUM_ADOW_CMDS)
            {
                UPDATE_PEC15_ERROR_COUNT(io_ltc6813CellVoltages_owcReadVoltages(PULL_DOWN), data.num_comm_tries);

                data.owc_state = CHECK_OPEN_WIRE_FAULT_STATE;
            }
            else
            {
                io_ltc6813CellVoltages_owcStart(PULL_DOWN);
                open_wire_pd_readings++;
            }
            break;
        }
        case CHECK_OPEN_WIRE_FAULT_STATE:
        {
            io_ltc6813CellVoltages_checkOpenWireStatus();
            data.owc_state = IDLE_STATE;

            app_accumulator_owcCalculateFaults();

            break;
        }
        case IDLE_STATE:
        {
            owc_idle_cycles++;

            if (owc_idle_cycles >= OPEN_WIRE_CHECK_IDLE_CYCLES)
            {
                data.owc_state = START_OPEN_WIRE_CHECK;
                is_finished    = true;
            }
            else
            {
                data.owc_state = IDLE_STATE;
            }

            break;
        }
        default:
        {
            break;
        }
    }

    return is_finished;
}

void app_accumulator_broadcast(void)
{
    // Broadcast pack voltage.
    app_canTx_BMS_PackVoltage_set(data.voltage_stats.pack_voltage);

    // Broadcast min/max cell voltage information.
    app_canTx_BMS_MinCellVoltage_set(data.voltage_stats.min_voltage.voltage);
    app_canTx_BMS_MaxCellVoltage_set(data.voltage_stats.max_voltage.voltage);
    app_canTx_BMS_MinCellVoltageSegment_set(data.voltage_stats.min_voltage.segment);
    app_canTx_BMS_MaxCellVoltageSegment_set(data.voltage_stats.max_voltage.segment);
    app_canTx_BMS_MinCellVoltageIdx_set(data.voltage_stats.min_voltage.cell);
    app_canTx_BMS_MaxCellVoltageIdx_set(data.voltage_stats.max_voltage.cell);

    // Get the min and max cell temperature and check to see if the temperatures
    // are in range
    uint8_t     min_segment   = 0U;
    uint8_t     min_loc       = 0U;
    uint8_t     max_segment   = 0U;
    uint8_t     max_loc       = 0U;
    const float min_cell_temp = io_ltc6813CellTemps_getMinTempDegC(&min_segment, &min_loc);
    const float max_cell_temp = io_ltc6813CellTemps_getMaxTempDegC(&max_segment, &max_loc);

    // Broadcast min/max cell temp information.
    app_canTx_BMS_MinCellTemperature_set(min_cell_temp);
    app_canTx_BMS_MaxCellTemperature_set(max_cell_temp);
    app_canTx_BMS_MinTempSegment_set(min_segment);
    app_canTx_BMS_MaxTempSegment_set(max_segment);
    app_canTx_BMS_MinTempIdx_set(min_loc);
    app_canTx_BMS_MaxTempIdx_set(max_loc);

    // Broadcast OWC information
    app_canTx_BMS_Segment0_OWC_Cells_Status_set(data.owc_faults.owcStatus[0]);
    app_canTx_BMS_Segment1_OWC_Cells_Status_set(data.owc_faults.owcStatus[1]);
    app_canTx_BMS_Segment2_OWC_Cells_Status_set(data.owc_faults.owcStatus[2]);
    app_canTx_BMS_Segment3_OWC_Cells_Status_set(data.owc_faults.owcStatus[3]);
    app_canTx_BMS_Segment4_OWC_Cells_Status_set(data.owc_faults.owcStatus[4]);

    // Calculate and broadcast pack power.
    const float available_power =
        MIN(app_math_linearDerating(max_cell_temp, MAX_POWER_LIMIT_W, CELL_ROLL_OFF_TEMP_DEGC, CELL_FULLY_DERATED_TEMP),
            MAX_POWER_LIMIT_W);

    app_canTx_BMS_AvailablePower_set(available_power);
    app_canTx_BMS_Seg4Cell2Temp_set(
        io_ltc6813CellTemperatures_getSpecificCellTempDegC(4, SEG4_CELL2_REG_GROUP, SEG4_CELL2_THERMISTOR));
    app_canTx_BMS_Seg4Cell2Temp_set(
        io_ltc6813CellTemperatures_getSpecificCellTempDegC(4, SEG4_CELL8_REG_GROUP, SEG4_CELL8_THERMISTOR));
}

bool app_accumulator_checkFaults(void)
{
    uint8_t throwaway_segment = 0U;
    uint8_t throwaway_loc     = 0U;

    float max_allowable_cell_temp = MAX_CELL_DISCHARGE_TEMP_DEGC;
    float min_allowable_cell_temp = MIN_CELL_DISCHARGE_TEMP_DEGC;

    // if we are charging, max cell temp is 45C not 60C
    if (app_tractiveSystem_getCurrent() > 3.0f)
    {
        max_allowable_cell_temp = MAX_CELL_CHARGE_TEMP_DEGC;
        min_allowable_cell_temp = MIN_CELL_CHARGE_TEMP_DEGC;
    }

    bool overtemp_fault =
        io_ltc6813CellTemps_getMaxTempDegC(&throwaway_segment, &throwaway_loc) > max_allowable_cell_temp;
    bool undertemp_fault =
        io_ltc6813CellTemps_getMinTempDegC(&throwaway_segment, &throwaway_loc) < min_allowable_cell_temp;
    bool overvoltage_fault   = data.voltage_stats.max_voltage.voltage > MAX_CELL_VOLTAGE;
    bool undervoltage_fault  = data.voltage_stats.min_voltage.voltage < MIN_CELL_VOLTAGE;
    bool communication_fault = data.num_comm_tries >= MAX_NUM_COMM_TRIES;

    app_canAlerts_BMS_Fault_CellUndervoltage_set(undervoltage_fault);
    app_canAlerts_BMS_Fault_CellOvervoltage_set(overvoltage_fault);
    app_canAlerts_BMS_Fault_CellUndertemp_set(undertemp_fault);
    app_canAlerts_BMS_Fault_CellOvertemp_set(overtemp_fault);
    app_canTx_BMS_NumCommTries_set(data.num_comm_tries);
    app_canTx_BMS_AccumulatorMonitorState_set((CAN_AccumulatorMonitorState)data.state);
    app_canAlerts_BMS_Fault_ModuleCommunicationError_set(communication_fault);

    bool owc_fault = data.owc_faults.owcGlobalFault;

    app_canAlerts_BMS_Fault_OpenWireCheckFault_set(data.owc_faults.owcGlobalFault);
    app_canAlerts_BMS_Fault_OpenWireCheck_Segment0_GND_set(data.owc_faults.owcFaultGND[0]);
    app_canAlerts_BMS_Fault_OpenWireCheck_Segment1_GND_set(data.owc_faults.owcFaultGND[1]);
    app_canAlerts_BMS_Fault_OpenWireCheck_Segment2_GND_set(data.owc_faults.owcFaultGND[2]);
    app_canAlerts_BMS_Fault_OpenWireCheck_Segment3_GND_set(data.owc_faults.owcFaultGND[3]);
    app_canAlerts_BMS_Fault_OpenWireCheck_Segment4_GND_set(data.owc_faults.owcFaultGND[4]);

    const bool acc_fault = overtemp_fault || undertemp_fault || overvoltage_fault || undervoltage_fault ||
                           communication_fault || owc_fault;

    return acc_fault;
}

void app_accumulator_enableBalancing(bool enabled)
{
    data.balance_enabled = enabled;
}

float app_accumulator_getPackVoltage(void)
{
    return data.voltage_stats.pack_voltage;
}

float app_accumulator_getMinCellVoltage(uint8_t *segment, uint8_t *cell)
{
    if (segment != NULL)
    {
        *segment = data.voltage_stats.min_voltage.segment;
    }

    if (cell != NULL)
    {
        *cell = data.voltage_stats.min_voltage.cell;
    }

    return data.voltage_stats.min_voltage.voltage;
}

float app_accumulator_getMaxCellVoltage(uint8_t *segment, uint8_t *cell)
{
    if (segment != NULL)
    {
        *segment = data.voltage_stats.max_voltage.segment;
    }

    if (cell != NULL)
    {
        *cell = data.voltage_stats.max_voltage.cell;
    }

    return data.voltage_stats.max_voltage.voltage;
}
