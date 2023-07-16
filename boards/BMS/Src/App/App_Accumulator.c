#include <string.h>
#include <float.h>
#include "App_Accumulator.h"
#include "App_CanAlerts.h"
#include "App_CanRx.h"

// Max number of PEC15 to occur before faulting
#define MAX_NUM_COMM_TRIES (3U)

// Discharging cells continuously generates too much heat.
// So balance cells for 100 ticks (the cell monitoring code runs in the 100Hz task, so 100 ticks = 1s),
// then disable discharge for the next 100 ticks to keep temperatures manageable.
// Reducing the ticks to 1 on / 1 off causes noticeable flicker, so I've set it to 1s on / 1s off to be less
// distracting.
#define BALANCE_DEFAULT_FREQ (1)  // Hz
#define BALANCE_DEFAULT_DUTY (50) // %
#define BALANCE_TICKS_ON (100U)
#define BALANCE_TICKS_OFF (100U)

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
    bool (*write_cfg_registers)(bool[ACCUMULATOR_NUM_SEGMENTS][ACCUMULATOR_NUM_SERIES_CELLS_PER_SEGMENT]);

    // Cell voltage monitoring functions
    bool (*start_cell_voltage_conv)(void);
    bool (*read_cell_voltages)(float[ACCUMULATOR_NUM_SEGMENTS][ACCUMULATOR_NUM_SERIES_CELLS_PER_SEGMENT]);

    // Voltage information
    VoltageStats            voltage_stats;
    AccumulatorMonitorState state;
    float                   cell_voltages[ACCUMULATOR_NUM_SEGMENTS][ACCUMULATOR_NUM_SERIES_CELLS_PER_SEGMENT];

    // Balancing information
    bool     balance_enabled;
    bool     cells_to_balance[ACCUMULATOR_NUM_SEGMENTS][ACCUMULATOR_NUM_SERIES_CELLS_PER_SEGMENT];
    uint32_t balance_pwm_ticks;
    bool     balance_pwm_high;

    // Cell temperature monitoring functions
    bool (*start_cell_temp_conv)(void);
    bool (*read_cell_temperatures)(void);
    float (*get_min_cell_temp)(uint8_t *, uint8_t *);
    float (*get_max_cell_temp)(uint8_t *, uint8_t *);
    float (*get_avg_cell_temp)(void);

    bool (*enable_balance)(void);
    bool (*disable_balance)(void);
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
 * Calculate which cells need discharging, for cell balancing.
 * @param accumulator The accumulator
 */
static void App_Accumulator_CalculateCellsToBalance(struct Accumulator *accumulator)
{
    float target_voltage = accumulator->voltage_stats.min_voltage.voltage + CELL_VOLTAGE_BALANCE_WINDOW_V;

    target_voltage = App_CanRx_Debug_CellBalancing_OverrideTarget_Get()
                         ? App_CanRx_Debug_CellBalancing_OverrideTargetValue_Get()
                         : target_voltage;
    for (uint8_t segment = 0U; segment < ACCUMULATOR_NUM_SEGMENTS; segment++)
    {
        for (uint8_t cell = 0U; cell < ACCUMULATOR_NUM_SERIES_CELLS_PER_SEGMENT; cell++)
        {
            const bool needs_discharging                 = (accumulator->cell_voltages[segment][cell] > target_voltage);
            accumulator->cells_to_balance[segment][cell] = needs_discharging;
        }
    }
}

/**
 * Balance cells.
 * @param accumulator The accumulator
 */
static void App_Accumulator_BalanceCells(struct Accumulator *accumulator)
{
    if (!accumulator->balance_enabled)
    {
        accumulator->disable_balance();
        return;
    }

    // Write to configuration register to configure cell discharging
    App_Accumulator_CalculateCellsToBalance(accumulator);
    accumulator->write_cfg_registers(accumulator->cells_to_balance);

    // Balance PWM settings
    float balance_pwm_freq = App_CanRx_Debug_CellBalancing_OverridePWM_Get()
                                 ? App_CanRx_Debug_CellBalancing_OverridePWMFrequency_Get()
                                 : BALANCE_DEFAULT_FREQ;
    uint32_t balance_pwm_duty = App_CanRx_Debug_CellBalancing_OverridePWM_Get()
                                    ? App_CanRx_Debug_CellBalancing_OverridePWMDuty_Get()
                                    : BALANCE_DEFAULT_DUTY;

    // duty_on = 100_ticks_per_sec * 1/freq_Hz * duty_percent / 100
    // TODO: verify frequency calculation. Period seems to be about double what it should be.
    uint32_t balance_ticks_on  = (uint32_t)(1.0f / balance_pwm_freq * (float)balance_pwm_duty);
    uint32_t balance_ticks_off = (uint32_t)(1.0f / balance_pwm_freq * (float)(100 - balance_pwm_duty));

    if (accumulator->balance_pwm_high)
    {
        // Enable cell discharging
        accumulator->enable_balance();
        accumulator->balance_pwm_ticks += 1;

        if (accumulator->balance_pwm_ticks >= balance_ticks_on)
        {
            // Cell discharging enabled duty cycle portion is finished
            accumulator->balance_pwm_high  = false;
            accumulator->balance_pwm_ticks = 0;
        }
    }
    else
    {
        // Disable cell discharging
        accumulator->disable_balance();
        accumulator->balance_pwm_ticks += 1;

        if (accumulator->balance_pwm_ticks >= balance_ticks_off)
        {
            // Cell discharging disabled duty cycle portion is finished
            accumulator->balance_pwm_high  = true;
            accumulator->balance_pwm_ticks = 0;
        }
    }
}

struct Accumulator *App_Accumulator_Create(
    bool (*config_monitoring_chip)(void),
    bool (*write_cfg_registers)(bool[ACCUMULATOR_NUM_SEGMENTS][ACCUMULATOR_NUM_SERIES_CELLS_PER_SEGMENT]),
    bool (*start_voltage_conv)(void),
    bool (*read_cell_voltages)(float[ACCUMULATOR_NUM_SEGMENTS][ACCUMULATOR_NUM_SERIES_CELLS_PER_SEGMENT]),
    bool (*start_cell_temp_conv)(void),
    bool (*read_cell_temperatures)(void),
    float (*get_min_cell_temp)(uint8_t *, uint8_t *),
    float (*get_max_cell_temp)(uint8_t *, uint8_t *),
    float (*get_avg_cell_temp)(void),
    bool (*enable_balance)(void),
    bool (*disable_balance)(void))
{
    struct Accumulator *accumulator = malloc(sizeof(struct Accumulator));
    assert(accumulator != NULL);

    accumulator->config_monitoring_chip = config_monitoring_chip;
    accumulator->write_cfg_registers    = write_cfg_registers;

    // Cell voltage monitoring functions
    accumulator->num_comm_tries          = 0U;
    accumulator->read_cell_voltages      = read_cell_voltages;
    accumulator->start_cell_voltage_conv = start_voltage_conv;

    // Voltage information
    memset(&accumulator->voltage_stats, 0U, sizeof(VoltageStats));
    accumulator->state = GET_CELL_VOLTAGE_STATE;

    // Cell temperature monitoring functions
    accumulator->start_cell_temp_conv   = start_cell_temp_conv;
    accumulator->read_cell_temperatures = read_cell_temperatures;
    accumulator->get_min_cell_temp      = get_min_cell_temp;
    accumulator->get_max_cell_temp      = get_max_cell_temp;
    accumulator->get_avg_cell_temp      = get_avg_cell_temp;

    // Balancing information
    memset(&accumulator->cells_to_balance, 0U, sizeof(accumulator->cells_to_balance));
    accumulator->balance_pwm_ticks = 0;
    accumulator->balance_enabled   = false;
    accumulator->balance_pwm_high  = false;

    accumulator->enable_balance  = enable_balance;
    accumulator->disable_balance = disable_balance;

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

bool App_Accumulator_EnableBalance(const struct Accumulator *const accumulator)
{
    return accumulator->enable_balance();
}

bool App_Accumulator_DisableBalance(const struct Accumulator *const accumulator)
{
    return accumulator->disable_balance();
}

void App_Accumulator_RunOnTick100Hz(struct Accumulator *const accumulator)
{
    switch (accumulator->state)
    {
        case GET_CELL_VOLTAGE_STATE:
        {
            // Attempt to read voltages from the LTCs, write output to cell voltages array
            UPDATE_PEC15_ERROR_COUNT(
                accumulator->read_cell_voltages(accumulator->cell_voltages), accumulator->num_comm_tries);

            // Calculate min/max/segment voltages
            App_Accumulator_CalculateVoltageStats(accumulator);

            // Configure cell balancing
            App_Accumulator_BalanceCells(accumulator);

            // Start cell voltage conversions for the next cycle
            accumulator->start_cell_temp_conv();
            accumulator->state = GET_CELL_TEMP_STATE;
            break;
        }
        case GET_CELL_TEMP_STATE:
        {
            UPDATE_PEC15_ERROR_COUNT(accumulator->read_cell_temperatures(), accumulator->num_comm_tries)

            // Start cell voltage conversions for the next cycle
            accumulator->start_cell_voltage_conv();

            accumulator->state = GET_CELL_VOLTAGE_STATE;
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
    if (App_TractiveSystem_GetCurrent(ts) > 3.0f)
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

    App_CanAlerts_SetFault(BMS_FAULT_CELL_UNDERVOLTAGE, undervoltage_fault);
    App_CanAlerts_SetFault(BMS_FAULT_CELL_OVERVOLTAGE, overvoltage_fault);
    App_CanAlerts_SetFault(BMS_FAULT_CELL_UNDERTEMP, undertemp_fault);
    App_CanAlerts_SetFault(BMS_FAULT_CELL_OVERTEMP, overtemp_fault);
    App_CanAlerts_SetFault(BMS_FAULT_MODULE_COMM_ERROR, communication_fault);

    return (overtemp_fault || undertemp_fault || overvoltage_fault || undervoltage_fault || communication_fault);
}

void App_Accumulator_EnableBalancing(struct Accumulator *const accumulator, bool enabled)
{
    accumulator->balance_enabled = enabled;
}

bool App_Accumulator_BalancingEnabled(struct Accumulator *const accumulator)
{
    return accumulator->balance_enabled;
}
