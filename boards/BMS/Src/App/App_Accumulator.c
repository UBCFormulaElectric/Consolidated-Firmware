#include <string.h>
#include <float.h>
#include "App_SharedMacros.h"
#include "App_Accumulator.h"
#include "App_CanAlerts.h"

// Max number of PEC15 to occur before faulting
#define MAX_NUM_COMM_TRIES (3U)

// Number of open wire check commands (ADOW) to send before open wire check
#define OPEN_WIRE_CHECK_NUM_ADOW_CMDS (2U)

// Time interval between running open wire check, in MS
#define OPEN_WIRE_CHECK_PERIOD_S (1U) // 5 second interval
#define TICK_RATE_HZ (100U)           // Accumulator ticked in 100Hz task
#define OPEN_WIRE_CHECK_PERIOD_TICKS (OPEN_WIRE_CHECK_PERIOD_S * TICK_RATE_HZ)

// For cell 0, cell 0 is open if V_PU(0) = 0V
#define OPEN_WIRE_CHECK_CELL_0_THRESHOLD_V (0.1f) // 100mV

// For cell N in 1-15, cell N is open if V_PU(N+1) - V_PD(N+1) < -400mV
// * V_PU(N) is pull-up voltage of cell N, i.e. result of ADOW with PUP set to 1
// * V_PD(N) is pull-down voltage of cell N, i.e. result of ADOW with PUP set to 0
#define OPEN_WIRE_CHECK_CELL_N_THRESHOLD_V (-0.4f) // -400mV

// Open wire check algorithm requires a 17th voltage reading to check if cell 16 is open wire
#define NUM_OPEN_WIRE_CHECK_READINGS_PER_SEGMENT (ACCUMULATOR_NUM_SERIES_CELLS_PER_SEGMENT + 1U)

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
    MONITOR_STATE_MACHINE,
    OPEN_WIRE_CHECK_STATE_MACHINE,
} AccumulatorStateMachine;

typedef enum
{
    MONITOR_CELL_VOLTAGES_STATE,
    MONITOR_CELL_TEMPS_STATE,
} AccumulatorMonitorState;

typedef enum
{
    OPEN_WIRE_CHECK_PU_STATE,
    OPEN_WIRE_CHECK_PD_STATE,
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

struct Accumulator
{
    uint8_t num_comm_tries;

    // Configure the cell monitoring chip
    bool (*config_monitoring_chip)(void);
    bool (*write_cfg_registers)(void);

    // Cell voltage monitoring functions
    bool (*start_cell_voltage_conv)(void);
    bool (*start_open_wire_check)(bool);
    bool (*read_cell_voltages)(float *, uint8_t);

    // State machine information
    AccumulatorStateMachine       state_machine;
    AccumulatorMonitorState       monitor_state;
    AccumulatorOpenWireCheckState open_wire_check_state;
    uint32_t                      tick_count;

    // Voltage information
    VoltageStats voltage_stats;
    float        cell_voltages[ACCUMULATOR_NUM_SEGMENTS][ACCUMULATOR_NUM_SERIES_CELLS_PER_SEGMENT];

    // Open wire check information
    uint8_t open_wire_num_pu_readings;
    uint8_t open_wire_num_pd_readings;
    float   open_wire_pu_voltages[ACCUMULATOR_NUM_SEGMENTS][NUM_OPEN_WIRE_CHECK_READINGS_PER_SEGMENT];
    float   open_wire_pd_voltages[ACCUMULATOR_NUM_SEGMENTS][NUM_OPEN_WIRE_CHECK_READINGS_PER_SEGMENT];
    bool    open_wire_cells[ACCUMULATOR_NUM_SEGMENTS][ACCUMULATOR_NUM_SERIES_CELLS_PER_SEGMENT];
    bool    open_wire_fault;

    // Cell temperature monitoring functions
    bool (*start_cell_temp_conv)(void);
    bool (*read_cell_temperatures)(void);
    float (*get_min_cell_temp)(uint8_t *, uint8_t *);
    float (*get_max_cell_temp)(uint8_t *, uint8_t *);
    float (*get_avg_cell_temp)(void);

    bool (*enable_discharge)(void);
    bool (*disable_discharge)(void);
};

/**
 * Trigger the calculation the voltage statistics for an accumulator
 * @param accumulator The accumulator to get the voltages from
 */
static void App_Accumulator_CalculateVoltageStats(struct Accumulator *accumulator)
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
            const float cell_voltage = App_Accumulator_GetCellVoltage(accumulator, segment, cell);

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

    accumulator->voltage_stats = temp_voltage_stats;
}

/**
 * Check for open wires between LTC and cell voltage taps.
 * @param accumulator The accumulator to check open wires for
 */
static void App_Accumulator_InterpretOpenWireCheck(struct Accumulator *accumulator)
{
    bool open_wire_fault = false;

    for (uint8_t segment = 0U; segment < ACCUMULATOR_NUM_SEGMENTS; segment++)
    {
        // For cell 0, cell 0 is open if V_PU(0) = 0V
        const bool cell_0_open = accumulator->open_wire_pu_voltages[segment][0U] < OPEN_WIRE_CHECK_CELL_0_THRESHOLD_V;
        accumulator->open_wire_cells[segment][0] = cell_0_open;
        accumulator->open_wire_fault |= cell_0_open;

        for (uint8_t cell = 1U; cell < ACCUMULATOR_NUM_SERIES_CELLS_PER_SEGMENT; cell++)
        {
            // For cell N in 1-15, cell N is open if V_PU(N+1) - V_PD(N+1) < -400mV
            // * V_PU(N) is pull-up voltage of cell N, i.e. voltages read back after ADOW with PUP set to 1
            // * V_PD(N) is pull-down voltage of cell N, i.e. voltages read back after ADOW with PUP set to 0

            // TODO: THIS IS WRONG! The +1 is unnecessary
            // In the LTC datasheet, where this algorithm is from, cell 1 (first cell) connects to C1 on positive terminal
            // In our code, cell 1's voltage is read back at index 0
            // Also this index "cell" shouldn't actually be the cell, it should be the pin index C0, C1, etc. since 
            // that is what can actually go open wire.
            const float n_plus_1_pu_voltage = accumulator->open_wire_pu_voltages[segment][cell + 1];
            const float n_plus_1_pd_voltage = accumulator->open_wire_pd_voltages[segment][cell + 1];
            const bool  cell_open = (n_plus_1_pu_voltage - n_plus_1_pd_voltage) < OPEN_WIRE_CHECK_CELL_N_THRESHOLD_V;
            accumulator->open_wire_cells[segment][cell] = cell_open;
            accumulator->open_wire_fault |= cell_open;
        }
    }
}

/**
 * Run the cell voltage and temperature monitoring state machine
 * @param accumulator The accumulator
 */
static void App_Accumulator_RunMonitorStateMachine(struct Accumulator *accumulator)
{
    switch (accumulator->monitor_state)
    {
        case MONITOR_CELL_VOLTAGES_STATE:
        {
            // Attempt to read voltages from the LTCs, write output to cell voltages array
            UPDATE_PEC15_ERROR_COUNT(
                accumulator->read_cell_voltages(
                    &accumulator->cell_voltages[0][0], ACCUMULATOR_NUM_SERIES_CELLS_PER_SEGMENT),
                accumulator->num_comm_tries);

            // Calculate min/max/segment voltages
            App_Accumulator_CalculateVoltageStats(accumulator);

            // Write to configuration register to configure cell discharging
            accumulator->write_cfg_registers();
            accumulator->disable_discharge();

            // Read cell temps next
            accumulator->monitor_state = MONITOR_CELL_TEMPS_STATE;
            break;
        }
        case MONITOR_CELL_TEMPS_STATE:
        {
            UPDATE_PEC15_ERROR_COUNT(accumulator->read_cell_temperatures(), accumulator->num_comm_tries)
            accumulator->disable_discharge();

            // Read cell voltages next
            accumulator->monitor_state = MONITOR_CELL_VOLTAGES_STATE;
            break;
        }
        default:
        {
            break;
        }
    }
}

/**
 * Run the open wire check state machine
 * @param accumulator The accumulator
 */
static void App_Accumulator_RunOpenWireCheckStateMachine(struct Accumulator *accumulator, bool *finished)
{
    *finished = false;

    switch (accumulator->open_wire_check_state)
    {
        case OPEN_WIRE_CHECK_PU_STATE:
        {
            accumulator->open_wire_num_pu_readings++;

            if (accumulator->open_wire_num_pu_readings >= OPEN_WIRE_CHECK_NUM_ADOW_CMDS)
            {
                // If required number of open wire check commands have been run, read voltages
                UPDATE_PEC15_ERROR_COUNT(
                    accumulator->read_cell_voltages(
                        &accumulator->open_wire_pu_voltages[0][0], NUM_OPEN_WIRE_CHECK_READINGS_PER_SEGMENT),
                    accumulator->num_comm_tries);

                // Run with pull-down current next
                accumulator->open_wire_check_state = OPEN_WIRE_CHECK_PD_STATE;
            }
            break;
        }
        case OPEN_WIRE_CHECK_PD_STATE:
        {
            accumulator->open_wire_num_pd_readings++;

            if (accumulator->open_wire_num_pd_readings >= OPEN_WIRE_CHECK_NUM_ADOW_CMDS)
            {
                // If required number of open wire check commands have been run, read voltages
                UPDATE_PEC15_ERROR_COUNT(
                    accumulator->read_cell_voltages(
                        &accumulator->open_wire_pd_voltages[0][0], NUM_OPEN_WIRE_CHECK_READINGS_PER_SEGMENT),
                    accumulator->num_comm_tries);

                // Check has completed, interpret results
                App_Accumulator_InterpretOpenWireCheck(accumulator);

                // Reset open wire check process
                accumulator->open_wire_num_pu_readings = 0;
                accumulator->open_wire_num_pd_readings = 0;
                accumulator->open_wire_check_state     = OPEN_WIRE_CHECK_PU_STATE;
                *finished                              = true;
            }
            break;
        }
        default:
        {
            break;
        }
    }
}

/**
 * Start the ADC voltage conversion for the next state.
 * @param accumulator The accumulator
 */
static void App_Accumulator_StartNextConversion(struct Accumulator *accumulator)
{
    switch (accumulator->state_machine)
    {
        case MONITOR_STATE_MACHINE:
        {
            switch (accumulator->monitor_state)
            {
                case MONITOR_CELL_VOLTAGES_STATE:
                {
                    accumulator->start_cell_voltage_conv();
                    break;
                }
                case MONITOR_CELL_TEMPS_STATE:
                {
                    accumulator->start_cell_temp_conv();
                    break;
                }
                default:
                {
                    break;
                }
            }
            break;
        }
        case OPEN_WIRE_CHECK_STATE_MACHINE:
        {
            switch (accumulator->open_wire_check_state)
            {
                case OPEN_WIRE_CHECK_PU_STATE:
                {
                    accumulator->start_open_wire_check(true);
                    break;
                }
                case OPEN_WIRE_CHECK_PD_STATE:
                {
                    accumulator->start_open_wire_check(false);
                    break;
                }
                default:
                {
                    break;
                }
            }
            break;
        }
        default:
        {
            break;
        }
    }
}

struct Accumulator *App_Accumulator_Create(
    bool (*config_monitoring_chip)(void),
    bool (*write_cfg_registers)(void),
    bool (*start_voltage_conv)(void),
    bool (*start_open_wire_check)(bool),
    bool (*read_cell_voltages)(float *, uint8_t),
    bool (*start_cell_temp_conv)(void),
    bool (*read_cell_temperatures)(void),
    float (*get_min_cell_temp)(uint8_t *, uint8_t *),
    float (*get_max_cell_temp)(uint8_t *, uint8_t *),
    float (*get_avg_cell_temp)(void),
    bool (*enable_discharge)(void),
    bool (*disable_discharge)(void))
{
    struct Accumulator *accumulator = malloc(sizeof(struct Accumulator));
    assert(accumulator != NULL);
    memset(accumulator, 0U, sizeof(struct Accumulator));

    accumulator->config_monitoring_chip = config_monitoring_chip;
    accumulator->write_cfg_registers    = write_cfg_registers;

    // Cell voltage monitoring functions
    accumulator->num_comm_tries          = 0U;
    accumulator->read_cell_voltages      = read_cell_voltages;
    accumulator->start_cell_voltage_conv = start_voltage_conv;
    accumulator->start_open_wire_check   = start_open_wire_check;

    // State machine information
    accumulator->state_machine         = MONITOR_STATE_MACHINE;
    accumulator->monitor_state         = MONITOR_CELL_VOLTAGES_STATE;
    accumulator->open_wire_check_state = OPEN_WIRE_CHECK_PU_STATE;

    // Voltage information
    memset(&accumulator->voltage_stats, 0U, sizeof(VoltageStats));

    // Cell temperature monitoring functions
    accumulator->start_cell_temp_conv   = start_cell_temp_conv;
    accumulator->read_cell_temperatures = read_cell_temperatures;
    accumulator->get_min_cell_temp      = get_min_cell_temp;
    accumulator->get_max_cell_temp      = get_max_cell_temp;
    accumulator->get_avg_cell_temp      = get_avg_cell_temp;

    accumulator->enable_discharge  = enable_discharge;
    accumulator->disable_discharge = disable_discharge;

    return accumulator;
}

void App_Accumulator_Destroy(struct Accumulator *accumulator)
{
    free(accumulator);
}

void App_Accumulator_InitRunOnEntry(const struct Accumulator *const accumulator)
{
    // Configure the cell monitoring chips. Disable discharge at startup
    accumulator->config_monitoring_chip();
}

bool App_Accumulator_HasCommunicationError(const struct Accumulator *const accumulator)
{
    return accumulator->num_comm_tries >= MAX_NUM_COMM_TRIES;
}

float App_Accumulator_GetCellVoltage(
    const struct Accumulator *const accumulator,
    AccumulatorSegment              segment,
    uint8_t                         cell)
{
    if (segment >= ACCUMULATOR_NUM_SEGMENTS || cell >= ACCUMULATOR_NUM_SERIES_CELLS_PER_SEGMENT)
    {
        return 0.0f;
    }

    return accumulator->cell_voltages[segment][cell];
}

float App_Accumulator_GetMaxVoltage(const struct Accumulator *const accumulator, uint8_t *segment, uint8_t *cell)
{
    *segment = accumulator->voltage_stats.max_voltage.segment;
    *cell    = accumulator->voltage_stats.max_voltage.cell;
    return accumulator->voltage_stats.max_voltage.voltage;
}

float App_Accumulator_GetMinVoltage(const struct Accumulator *const accumulator, uint8_t *segment, uint8_t *cell)
{
    *segment = accumulator->voltage_stats.min_voltage.segment;
    *cell    = accumulator->voltage_stats.min_voltage.cell;
    return accumulator->voltage_stats.min_voltage.voltage;
}

float App_Accumulator_GetAverageCellVoltage(const struct Accumulator *const accumulator, uint8_t segment)
{
    if (segment >= ACCUMULATOR_NUM_SEGMENTS)
    {
        return 0.0f;
    }

    return accumulator->voltage_stats.segment_voltages[segment] / ACCUMULATOR_NUM_SERIES_CELLS_PER_SEGMENT;
}

float App_Accumulator_GetSegmentVoltage(const struct Accumulator *const accumulator, uint8_t segment)
{
    if (segment >= ACCUMULATOR_NUM_SEGMENTS)
    {
        return 0.0f;
    }

    return accumulator->voltage_stats.segment_voltages[segment];
}

float App_Accumulator_GetAverageSegmentVoltage(const struct Accumulator *const accumulator)
{
    return accumulator->voltage_stats.pack_voltage / ACCUMULATOR_NUM_SEGMENTS;
}

float App_Accumulator_GetAccumulatorVoltage(const struct Accumulator *accumulator)
{
    return accumulator->voltage_stats.pack_voltage;
}

float App_Accumulator_GetMinCellTempDegC(
    const struct Accumulator *const accumulator,
    uint8_t *                       segment,
    uint8_t *                       thermistor)
{
    return accumulator->get_min_cell_temp(segment, thermistor);
}

float App_Accumulator_GetMaxCellTempDegC(
    const struct Accumulator *const accumulator,
    uint8_t *                       segment,
    uint8_t *                       thermistor)
{
    return accumulator->get_max_cell_temp(segment, thermistor);
}

float App_Accumulator_GetAvgCellTempDegC(const struct Accumulator *const accumulator)
{
    return accumulator->get_avg_cell_temp();
}

bool App_Accumulator_EnableDischarge(const struct Accumulator *const accumulator)
{
    return accumulator->enable_discharge();
}

bool App_Accumulator_DisableDischarge(const struct Accumulator *const accumulator)
{
    return accumulator->disable_discharge();
}

void App_Accumulator_RunOnTick100Hz(struct Accumulator *const accumulator)
{
    switch (accumulator->state_machine)
    {
        case MONITOR_STATE_MACHINE:
        {
            // Run cell voltage/temperature monitoring state machine
            App_Accumulator_RunMonitorStateMachine(accumulator);

            // If open wire check period has elapsed, switch state machines
            if (accumulator->tick_count >= OPEN_WIRE_CHECK_PERIOD_TICKS)
            {
                accumulator->tick_count    = 0U;
                accumulator->state_machine = OPEN_WIRE_CHECK_STATE_MACHINE;
            }
            else
            {
                accumulator->tick_count++;
            }

            // Start conversion for next state
            App_Accumulator_StartNextConversion(accumulator);
            break;
        }
        case OPEN_WIRE_CHECK_STATE_MACHINE:
        {
            // Run open wire check state machine
            bool check_finished = false;
            App_Accumulator_RunOpenWireCheckStateMachine(accumulator, &check_finished);

            if (check_finished)
            {
                // Back to cell monitoring once check has completed
                accumulator->state_machine = MONITOR_STATE_MACHINE;
            }

            // Start conversion for next state
            App_Accumulator_StartNextConversion(accumulator);
            break;
        }
        default:
        {
            break;
        }
    }
}

bool App_Accumulator_CheckFaults(struct Accumulator *const accumulator, struct TractiveSystem *const ts)
{
    uint8_t throwaway_segment = 0U;
    uint8_t throwaway_loc     = 0U;

    float max_allowable_cell_temp = MAX_CELL_DISCHARGE_TEMP_DEGC;
    float min_allowable_cell_temp = MIN_CELL_DISCHARGE_TEMP_DEGC;

    // if we are charging, max cell temp is 45C not 60C
    if (App_TractiveSystem_GetCurrent(ts) < -3.0f)
    {
        max_allowable_cell_temp = MAX_CELL_CHARGE_TEMP_DEGC;
        min_allowable_cell_temp = MIN_CELL_CHARGE_TEMP_DEGC;
    }

    bool overtemp_fault =
        App_Accumulator_GetMaxCellTempDegC(accumulator, &throwaway_segment, &throwaway_loc) > max_allowable_cell_temp;
    bool undertemp_fault =
        App_Accumulator_GetMinCellTempDegC(accumulator, &throwaway_segment, &throwaway_loc) < min_allowable_cell_temp;
    bool overvoltage_fault =
        App_Accumulator_GetMaxVoltage(accumulator, &throwaway_segment, &throwaway_loc) > MAX_CELL_VOLTAGE;
    bool undervoltage_fault =
        App_Accumulator_GetMinVoltage(accumulator, &throwaway_segment, &throwaway_loc) < MIN_CELL_VOLTAGE;
    bool communication_fault = App_Accumulator_HasCommunicationError(accumulator);
    bool open_wire_fault     = accumulator->open_wire_fault;

    App_CanAlerts_SetFault(BMS_FAULT_CELL_UNDERVOLTAGE, undervoltage_fault);
    App_CanAlerts_SetFault(BMS_FAULT_CELL_OVERVOLTAGE, overvoltage_fault);
    App_CanAlerts_SetFault(BMS_FAULT_CELL_UNDERTEMP, undertemp_fault);
    App_CanAlerts_SetFault(BMS_FAULT_CELL_OVERTEMP, overtemp_fault);
    App_CanAlerts_SetFault(BMS_FAULT_MODULE_COMM_ERROR, communication_fault);
    App_CanAlerts_SetFault(BMS_FAULT_OPEN_WIRE_CELL, open_wire_fault);

    return (
        overtemp_fault || undertemp_fault || overvoltage_fault || undervoltage_fault || communication_fault ||
        open_wire_fault);
}
