#include "app_accumulator.h"
#include "App_CanTx.h"
#include "App_CanRx.h"
#include "App_CanAlerts.h"
#include "App_SharedProcessing.h"
#include "ltc6813/io_ltc6813Shared.h"
#include "ltc6813/io_ltc6813CellVoltages.h"
#include "ltc6813/io_ltc6813CellTemps.h"

#include <string.h>
#include <float.h>

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

#define MAX_POWER_LIMIT_W (78e3f)
#define CELL_ROLL_OFF_TEMP_DEGC (40.0f)
#define CELL_FULLY_DERATED_TEMP (60.0f)

#define SEG4_CELL2_REG_GROUP (0U)
#define SEG4_CELL8_REG_GROUP (2U)
#define SEG4_CELL2_THERMISTOR (2U)
#define SEG4_CELL8_THERMISTOR (2U)

// Update the counter keeping track of the PEC15 error
#define UPDATE_PEC15_ERROR_COUNT(is_pec_ok, num_comm_tries)                                                            \
    if ((is_pec_ok))                                                                                                   \
    {                                                                                                                  \
        (num_comm_tries) = 0U;                                                                                         \
    }                                                                                                                  \
    else                                                                                                               \
    {                                                                                                                  \
        (num_comm_tries)++;                                                                                            \
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

typedef struct
{
    // Cells information
    uint8_t                 num_comm_tries;
    VoltageStats            voltage_stats;
    AccumulatorMonitorState state;

    // Balancing information
    bool     balance_enabled;
    bool     cells_to_balance[ACCUMULATOR_NUM_SEGMENTS][ACCUMULATOR_NUM_SERIES_CELLS_PER_SEGMENT];
    uint32_t balance_pwm_ticks;
    bool     balance_pwm_high;
} Accumulator;

static Accumulator data;

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

static void app_accumulator_calculateCellsToBalance(void)
{
    float target_voltage = data.voltage_stats.min_voltage.voltage + CELL_VOLTAGE_BALANCE_WINDOW_V;

    target_voltage = App_CanRx_Debug_CellBalancingOverrideTarget_Get()
                         ? App_CanRx_Debug_CellBalancingOverrideTargetValue_Get()
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

static void app_accumulator_balanceCells(void)
{
    if (!data.balance_enabled)
    {
        io_ltc6813Shared_disableBalance();
        return;
    }

    // Write to configuration register to configure cell discharging
    app_accumulator_calculateCellsToBalance();
    io_ltc6813Shared_writeConfigurationRegisters(true);

    // Balance PWM settings
    float    balance_pwm_freq = App_CanRx_Debug_CellBalancingOverridePWM_Get()
                                    ? App_CanRx_Debug_CellBalancingOverridePWMFrequency_Get()
                                    : BALANCE_DEFAULT_FREQ;
    uint32_t balance_pwm_duty = App_CanRx_Debug_CellBalancingOverridePWM_Get()
                                    ? App_CanRx_Debug_CellBalancingOverridePWMDuty_Get()
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
}

void app_accumulator_writeDefaultConfig()
{
    // Configure the cell monitoring chips. Disable discharge at startup
    io_ltc6813Shared_setCfgRegsToDefaultSettings();
}

void app_accumulator_runOnTick100Hz(void)
{
    switch (data.state)
    {
        case GET_CELL_VOLTAGE_STATE:
        {
            // Attempt to read voltages from the LTCs, write output to cell voltages array
            UPDATE_PEC15_ERROR_COUNT(io_ltc6813CellVoltages_readVoltages(), data.num_comm_tries);

            // Calculate min/max/segment voltages
            app_accumulator_calculateVoltageStats();

            // Configure cell balancing
            app_accumulator_balanceCells();

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

void app_accumulator_broadcast(void)
{
    // Broadcast pack voltage.
    App_CanTx_BMS_PackVoltage_Set(data.voltage_stats.pack_voltage);

    // Broadcast min/max cell voltage information.
    App_CanTx_BMS_MinCellVoltage_Set(data.voltage_stats.min_voltage.voltage);
    App_CanTx_BMS_MaxCellVoltage_Set(data.voltage_stats.max_voltage.voltage);
    App_CanTx_BMS_MinCellVoltageSegment_Set(data.voltage_stats.min_voltage.segment);
    App_CanTx_BMS_MaxCellVoltageSegment_Set(data.voltage_stats.max_voltage.segment);
    App_CanTx_BMS_MinCellVoltageIdx_Set(data.voltage_stats.min_voltage.cell);
    App_CanTx_BMS_MaxCellVoltageIdx_Set(data.voltage_stats.min_voltage.cell);

    // Get the min and max cell temperature and check to see if the temperatures
    // are in range
    uint8_t     min_segment   = 0U;
    uint8_t     min_loc       = 0U;
    uint8_t     max_segment   = 0U;
    uint8_t     max_loc       = 0U;
    const float min_cell_temp = io_ltc6813CellTemps_getMinTempDegC(&min_segment, &min_loc);
    const float max_cell_temp = io_ltc6813CellTemps_getMaxTempDegC(&max_segment, &max_loc);

    // Broadcast min/max cell temp information.
    App_CanTx_BMS_MinCellTemperature_Set(min_cell_temp);
    App_CanTx_BMS_MaxCellTemperature_Set(max_cell_temp);
    App_CanTx_BMS_MinTempSegment_Set(min_segment);
    App_CanTx_BMS_MaxTempSegment_Set(max_segment);
    App_CanTx_BMS_MinTempIdx_Set(min_loc);
    App_CanTx_BMS_MaxTempIdx_Set(max_loc);

    // Calculate and broadcast pack power.
    const float available_power =
        MIN(App_SharedProcessing_LinearDerating(
                max_cell_temp, MAX_POWER_LIMIT_W, CELL_ROLL_OFF_TEMP_DEGC, CELL_FULLY_DERATED_TEMP),
            MAX_POWER_LIMIT_W);

    App_CanTx_BMS_AvailablePower_Set(available_power);
    App_CanTx_BMS_Seg4Cell2Temp_Set(
        io_ltc6813CellTemperatures_getSpecificCellTempDegC(4, SEG4_CELL2_REG_GROUP, SEG4_CELL2_THERMISTOR));
    App_CanTx_BMS_Seg4Cell2Temp_Set(
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

    App_CanAlerts_BMS_Fault_CellUndervoltage_Set(undervoltage_fault);
    App_CanAlerts_BMS_Fault_CellOvervoltage_Set(overvoltage_fault);
    App_CanAlerts_BMS_Fault_CellUndertemp_Set(undertemp_fault);
    App_CanAlerts_BMS_Fault_CellOvertemp_Set(overtemp_fault);
    App_CanAlerts_BMS_Fault_ModuleCommunicationError_Set(communication_fault);

    const bool acc_fault =
        overtemp_fault || undertemp_fault || overvoltage_fault || undervoltage_fault || communication_fault;

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