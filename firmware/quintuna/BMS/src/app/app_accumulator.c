#include "app_accumulator.h"

#include <float.h>
#include <assert.h>
#include <string.h>

#include "states/app_chargeState.h"
#include "app_segments.h"
#include "app_canTx.h"
#include "app_canRx.h"
#include "app_canAlerts.h"
#include "app_math.h"
#include "app_timer.h"
#include "app_utils.h"
#include "io_ltc6813.h"

#define MAX_CELL_DISCHARGE_TEMP_DEGC (60.0f)
#define MAX_CELL_CHARGE_TEMP_DEGC (45.0f)
#define MIN_CELL_DISCHARGE_TEMP_DEGC (-20.0f)
#define MIN_CELL_CHARGE_TEMP_DEGC (0.0f)
#define MAX_CELL_VOLTAGE_NOMINAL (4.2f)
#define MIN_CELL_VOLTAGE (3.0f)

// Allows balancing of cells even if slight over-charging occurs. Occured prior to Competition 2024, where a fully
// charged pack with max cell V of 4.19 after charging reported as 4.21 after settling. Cause currently unknown, but
// this allows for these over-charged cells to be discharged back to safe limits
#define MAX_CELL_VOLTAGE_BALANCING (4.25f)

// Fault debounce durations.
#define UNDER_VOLTAGE_DEBOUNCE_DURATION_MS (500U)
#define OVER_VOLTAGE_DEBOUNCE_DURATION_MS (500U)
#define UNDER_TEMP_DEBOUNCE_DURATION_MS (1000U)
#define OVER_TEMP_DEBOUNCE_DURATION_MS (1000U)

// Cell Balancing Discharge Parameters
#define CELL_VOLTAGE_BALANCE_WINDOW_V (600e-6f) // 600uV

// Max number of PEC15 to occur before faulting
#define MAX_NUM_COMM_TRIES (3U)

#define MAX_POWER_LIMIT_W (78e3f)
#define CELL_ROLL_OFF_TEMP_DEGC (40.0f)
#define CELL_FULLY_DERATED_TEMP (60.0f)

#define TOTAL_NUM_OF_THERMISTORS (NUM_SEGMENTS * THERMISTORS_PER_SEGMENT)

// Discharging cells continuously generates too much heat.
// So balance cells for 100 ticks (the cell monitoring code runs in the 100Hz task, so 100 ticks = 1s),
// then disable discharge for the next 100 ticks to keep temperatures manageable.
// Reducing the ticks to 1 on / 1 off causes noticeable flicker, so I've set it to 1s on / 1s off to be less
// distracting.
#define BALANCE_TOLERANCE (0.01f) // 10mV
#define BALANCE_DEFAULT_FREQ (1)  // Hz
#define BALANCE_DEFAULT_DUTY (50) // %

typedef struct
{
    uint8_t segment;
    uint8_t cell;
    float   voltage;
} CellVoltage;

typedef struct
{
    uint8_t segment;
    uint8_t cell;
    float   temp;
} CellTemp;

typedef struct
{
    CellVoltage min_voltage_cell;
    CellVoltage max_voltage_cell;
    float       segment_voltages[NUM_SEGMENTS];
    float       pack_voltage;
} VoltageStats;

typedef struct
{
    CellTemp min_temp_cell;
    CellTemp max_temp_cell;
    float    avg;
} TempStats;

static TempStats    temp_stats;
static VoltageStats voltage_stats;

static TimerChannel under_voltage_fault_timer;
static TimerChannel over_voltage_fault_timer;
static TimerChannel under_temp_fault_timer;
static TimerChannel over_temp_fault_timer;

static uint32_t conceq_fails = 0;

void app_accumulator_init(void)
{
    // Voltage information
    memset(&voltage_stats, 0U, sizeof(VoltageStats));

    // Init fault debounce timers.
    app_timer_init(&under_voltage_fault_timer, UNDER_VOLTAGE_DEBOUNCE_DURATION_MS);
    app_timer_init(&over_voltage_fault_timer, OVER_VOLTAGE_DEBOUNCE_DURATION_MS);
    app_timer_init(&under_temp_fault_timer, UNDER_TEMP_DEBOUNCE_DURATION_MS);
    app_timer_init(&over_temp_fault_timer, OVER_TEMP_DEBOUNCE_DURATION_MS);
}

void app_accumulator_broadcast(void)
{
    // Broadcast pack voltage.
    app_canTx_BMS_PackVoltage_set(voltage_stats.pack_voltage);

    // Broadcast min/max cell voltage information.
    app_canTx_BMS_MinCellVoltage_set(voltage_stats.min_voltage_cell.voltage);
    app_canTx_BMS_MaxCellVoltage_set(voltage_stats.max_voltage_cell.voltage);
    app_canTx_BMS_MinCellVoltageSegment_set(voltage_stats.min_voltage_cell.segment);
    app_canTx_BMS_MaxCellVoltageSegment_set(voltage_stats.max_voltage_cell.segment);
    app_canTx_BMS_MinCellVoltageIdx_set(voltage_stats.min_voltage_cell.cell);
    app_canTx_BMS_MaxCellVoltageIdx_set(voltage_stats.max_voltage_cell.cell);

    // Broadcast min/max cell temp information.
    app_canTx_BMS_MinCellTemperature_set(temp_stats.min_temp_cell.temp);
    app_canTx_BMS_MinTempSegment_set(temp_stats.min_temp_cell.segment);
    app_canTx_BMS_MinTempIdx_set(temp_stats.min_temp_cell.cell);

    app_canTx_BMS_MaxCellTemperature_set(temp_stats.max_temp_cell.temp);
    app_canTx_BMS_MaxTempSegment_set(temp_stats.max_temp_cell.segment);
    app_canTx_BMS_MaxTempIdx_set(temp_stats.max_temp_cell.cell);

    // Calculate and broadcast pack power.
    const float available_power =
        MIN(app_math_linearDerating(
                temp_stats.max_temp_cell.temp, MAX_POWER_LIMIT_W, CELL_ROLL_OFF_TEMP_DEGC, CELL_FULLY_DERATED_TEMP),
            MAX_POWER_LIMIT_W);

    app_canTx_BMS_AvailablePower_set((uint32_t)available_power);
}

void app_accumulator_calculateVoltageStats(void)
{
    voltage_stats = (VoltageStats){
        .min_voltage_cell = { .voltage = FLT_MAX },
        .max_voltage_cell = { .voltage = -FLT_MAX },
        .pack_voltage     = 0.0f,
    };

    // Find the min and max voltages
    for (uint8_t segment = 0U; segment < NUM_SEGMENTS; segment++)
    {
        for (uint8_t cell = 0U; cell < CELLS_PER_SEGMENT; cell++)
        {
            if (volt_success_buf[segment][cell])
            {
                continue;
            }

            // Collect each cell voltage to find the min/max
            const float cell_voltage = cell_voltages[segment][cell];

            // Get the minimum cell voltage
            if (cell_voltage < voltage_stats.min_voltage_cell.voltage)
            {
                voltage_stats.min_voltage_cell.voltage = cell_voltage;
                voltage_stats.min_voltage_cell.segment = segment;
                voltage_stats.min_voltage_cell.cell    = cell;
            }
            // Get the maximum cell voltage
            if (cell_voltage > voltage_stats.max_voltage_cell.voltage)
            {
                voltage_stats.max_voltage_cell.voltage = cell_voltage;
                voltage_stats.max_voltage_cell.segment = segment;
                voltage_stats.max_voltage_cell.cell    = cell;
            }

            // Sum the voltages into a segment voltage
            voltage_stats.segment_voltages[segment] += cell_voltage;
        }

        // Sum the segment voltages into a pack voltage
        voltage_stats.pack_voltage += voltage_stats.segment_voltages[segment];
    }
}

void app_accumulator_calculateTemperatureStats(void)
{
    temp_stats =
        (TempStats){ .min_temp_cell = { .temp = FLT_MAX }, .max_temp_cell = { .temp = -FLT_MAX }, .avg = 0.0f };
    float sum_temp = 0U;

    for (uint8_t curr_segment = 0U; curr_segment < NUM_SEGMENTS; curr_segment++)
    {
        for (uint8_t curr_cell_index = 0U; curr_cell_index < THERMISTORS_PER_SEGMENT; curr_cell_index++)
        {
            if (volt_success_buf[curr_segment][curr_cell_index])
            {
                continue;
            }

            const float curr_cell_temp = cell_temps[curr_segment][curr_cell_index];

            // Get the minimum cell voltage
            if (curr_cell_temp < temp_stats.min_temp_cell.temp)
            {
                temp_stats.min_temp_cell.temp    = curr_cell_temp;
                temp_stats.min_temp_cell.segment = curr_segment;
                temp_stats.min_temp_cell.cell    = curr_cell_index;
            }

            // Get the maximum cell voltage
            if (curr_cell_temp > temp_stats.max_temp_cell.temp)
            {
                temp_stats.max_temp_cell.temp    = curr_cell_temp;
                temp_stats.max_temp_cell.segment = curr_segment;
                temp_stats.max_temp_cell.cell    = curr_cell_index;
            }

            sum_temp += curr_cell_temp;
        }
    }

    // Calculate the average temperature
    temp_stats.avg = sum_temp / TOTAL_NUM_OF_THERMISTORS;
}

// void app_accumulator_calculateCellsToBalance(void)
// {
//     const float target_voltage = app_canRx_Debug_CellBalancingOverrideTarget_get()
//                                      ? app_canRx_Debug_CellBalancingOverrideTargetValue_get()
//                                      : voltage_stats.min_voltage_cell.voltage + CELL_VOLTAGE_BALANCE_WINDOW_V;

//     float balancing_excess_voltage = 0;
//     for (uint8_t segment = 0U; segment < NUM_SEGMENTS; segment++)
//     {
//         for (uint8_t cell = 0U; cell < CELLS_PER_SEGMENT; cell++)
//         {
//             // this is equivalent to abs(cell_voltages[segment][cell] - target_voltage) > BALANCE_TOLERANCE
//             // as cell_voltages[segment][cell] > target_voltage
//             cells_to_balance[segment][cell] = cell_voltages[segment][cell] - target_voltage > BALANCE_TOLERANCE;
//             balancing_excess_voltage += MAX(0, cell_voltages[segment][cell] - (target_voltage + BALANCE_TOLERANCE));
//         }
//     }

//     app_canTx_BMS_ExcessVoltage_set(balancing_excess_voltage);
// }

bool app_accumulator_checkFaults(void)
{
    // if we are charging, max cell temp is 45C not 60C
    const bool  charger_connected       = true; // TODO charger
    const float max_allowable_cell_temp = charger_connected ? MAX_CELL_CHARGE_TEMP_DEGC : MAX_CELL_DISCHARGE_TEMP_DEGC;
    const float min_allowable_cell_temp = charger_connected ? MIN_CELL_CHARGE_TEMP_DEGC : MIN_CELL_DISCHARGE_TEMP_DEGC;

    // Check if balancing is enabled. For safety reasons, also check if current state is charge state, as we do not want
    // to adjust max cell voltage thresholds if actively charging.
    const bool cell_balancing_enabled =
        app_canRx_Debug_CellBalancingRequest_get() && app_stateMachine_getCurrentState() != app_chargeState_get();

    // Allows balancing of cells even if slight over-charging occurs. Occured prior to Competition 2024, where a fully
    // charged pack with max cell V of 4.19 after charging reported as 4.21 after settling. Cause currently unknown, but
    // this allows for these over-charged cells to be discharged back to safe limits
    const float max_cell_voltage = cell_balancing_enabled ? MAX_CELL_VOLTAGE_BALANCING : MAX_CELL_VOLTAGE_NOMINAL;

    const bool over_temp_condition     = temp_stats.max_temp_cell.temp > max_allowable_cell_temp;
    const bool under_temp_condition    = temp_stats.min_temp_cell.temp < min_allowable_cell_temp;
    const bool over_voltage_condition  = voltage_stats.max_voltage_cell.voltage > max_cell_voltage;
    const bool under_voltage_condition = voltage_stats.min_voltage_cell.voltage < MIN_CELL_VOLTAGE;

    const bool over_temp_fault =
        app_timer_runIfCondition(&over_temp_fault_timer, over_temp_condition) == TIMER_STATE_EXPIRED;
    const bool under_temp_fault =
        app_timer_runIfCondition(&under_temp_fault_timer, under_temp_condition) == TIMER_STATE_EXPIRED;
    const bool over_voltage_fault =
        app_timer_runIfCondition(&over_voltage_fault_timer, over_voltage_condition) == TIMER_STATE_EXPIRED;
    const bool under_voltage_fault =
        app_timer_runIfCondition(&under_voltage_fault_timer, under_voltage_condition) == TIMER_STATE_EXPIRED;

    app_canAlerts_BMS_Fault_CellUndervoltage_set(under_voltage_fault);
    app_canAlerts_BMS_Fault_CellOvervoltage_set(over_voltage_fault);
    app_canAlerts_BMS_Fault_CellUndertemp_set(under_temp_fault);
    app_canAlerts_BMS_Fault_CellOvertemp_set(over_temp_fault);

    const bool communication_fault = conceq_fails >= MAX_NUM_COMM_TRIES;
    app_canAlerts_BMS_Fault_ModuleCommunicationError_set(communication_fault);

    return over_temp_fault || under_temp_fault || over_voltage_fault || under_voltage_fault || communication_fault;
}

// void app_accumulator_balanceCells(void)
// {
//     // Exit early if ADC conversion fails
//     if (IS_EXIT_ERR(io_ltc6813_pollAdcConversions()))
//     {
//         return;
//     }

//     // Write to configuration register to configure cell discharging
//     bool cells_to_balance[NUM_SEGMENTS][CELLS_PER_SEGMENT];
//     calculateCellsToBalance(cells_to_balance);
//     io_ltc6813_writeConfigurationRegisters(cells_to_balance);

//     // Balance PWM settings
//     const float    balance_pwm_freq = app_canRx_Debug_CellBalancingOverridePWM_get()
//                                           ? app_canRx_Debug_CellBalancingOverridePWMFrequency_get()
//                                           : BALANCE_DEFAULT_FREQ;
//     const uint32_t balance_pwm_duty = app_canRx_Debug_CellBalancingOverridePWM_get()
//                                           ? app_canRx_Debug_CellBalancingOverridePWMDuty_get()
//                                           : BALANCE_DEFAULT_DUTY;

//     static uint32_t balance_pwm_ticks;
//     static bool     balance_pwm_high;
//     // duty_on = 100_ticks_per_sec * 1/freq_Hz * duty_percent / 100
//     // TODO: verify frequency calculation. Period seems to be about double what it should be.
//     if (balance_pwm_high)
//     {
//         const uint32_t balance_ticks_on = (uint32_t)(1.0f / balance_pwm_freq * (float)balance_pwm_duty);
//         // Enable cell discharging
//         io_ltc6813_sendBalanceCommand();
//         balance_pwm_ticks += 1;

//         if (balance_pwm_ticks >= balance_ticks_on)
//         {
//             // Cell discharging enabled duty cycle portion is finished
//             balance_pwm_high  = false;
//             balance_pwm_ticks = 0;
//         }
//     }
//     else
//     {
//         const uint32_t balance_ticks_off = (uint32_t)(1.0f / balance_pwm_freq * (float)(100 - balance_pwm_duty));
//         // Disable cell discharging
//         io_ltc6813_sendStopBalanceCommand();
//         balance_pwm_ticks += 1;

//         if (balance_pwm_ticks >= balance_ticks_off)
//         {
//             // Cell discharging disabled duty cycle portion is finished
//             balance_pwm_high  = true;
//             balance_pwm_ticks = 0;
//         }
//     }
//     app_canTx_BMS_BalancingOn_set(balance_pwm_high);
//     io_canTx_BMS_BalancingInfo_sendAperiodic();
// }
