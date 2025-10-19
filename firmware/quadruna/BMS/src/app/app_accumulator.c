#include "app_accumulator.h"

#include "states/app_chargeState.h"
#include "app_canTx.h"
#include "app_canRx.h"
#include "app_canAlerts.h"
#include "app_math.h"
#include "app_timer.h"
#include "utils.h"
#include "io_ltc6813.h"
#include "io_log.h"
#include "io_canTx.h"

#include <assert.h>
#include <float.h>
#include <string.h>

// Max number of PEC15 to occur before faulting
#define MAX_NUM_COMM_TRIES (3U)

// Number of open wire check commands (ADOW) to send before open wire check
#define OPEN_WIRE_CHECK_NUM_ADOW_CMDS (2)
// Number of cool down states after running OWC to let voltage settle down
#define OPEN_WIRE_CHECK_IDLE_CYCLES (4)

#define MAX_POWER_LIMIT_W (78e3f)
#define CELL_ROLL_OFF_TEMP_DEGC (40.0f)
#define CELL_FULLY_DERATED_TEMP (60.0f)

// Update the counter keeping track of the PEC15 error
#define UPDATE_PEC15_ERROR_COUNT(is_pec_ok, conceq_fails) \
    if ((is_pec_ok))                                      \
    {                                                     \
        (conceq_fails) = 0U;                              \
    }                                                     \
    else                                                  \
    {                                                     \
        (conceq_fails)++;                                 \
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
    // uint16_t cell_owc_status[NUM_SEGMENTS];
    bool owc_fault_gnd[NUM_SEGMENTS];
    bool owc_global_fault;
} OWCFaults;

static struct
{
    // Cells information
    AccumulatorMonitorState measurement_state;

    // OWC information
    OWCFaults                     owc_faults;
    AccumulatorOpenWireCheckState owc_state;
} data;

static uint8_t open_wire_pu_readings;
static uint8_t open_wire_pd_readings;
static uint8_t owc_idle_cycles;

static TimerChannel under_voltage_fault_timer;
static TimerChannel over_voltage_fault_timer;
static TimerChannel under_temp_fault_timer;
static TimerChannel over_temp_fault_timer;

// THE EDWIN LINE
static uint32_t conceq_fails                                                = 0;
static bool     voltage_read_success[NUM_SEGMENTS][VOLTAGE_REGISTER_GROUPS] = { false };
static bool     temp_read_success[NUM_SEGMENTS][AUX_REGISTER_GROUPS]        = { false };
float           cell_voltages[NUM_SEGMENTS][CELLS_PER_SEGMENT];
static float    cell_temps[NUM_SEGMENTS][THERMISTORS_PER_SEGMENT];

static float cell_voltage_pu[NUM_SEGMENTS][CELLS_PER_SEGMENT];
static bool  cell_voltage_pu_success[NUM_SEGMENTS][VOLTAGE_REGISTER_GROUPS] = { false };
static float cell_voltage_pd[NUM_SEGMENTS][CELLS_PER_SEGMENT];
static bool  cell_voltage_pd_success[NUM_SEGMENTS][VOLTAGE_REGISTER_GROUPS] = { false };
static bool  cell_owc_status[NUM_SEGMENTS][CELLS_PER_SEGMENT];

typedef struct
{
    uint8_t segment;
    uint8_t cell;
    float   voltage;
} CellVoltage;
typedef struct
{
    CellVoltage min_voltage_cell;
    CellVoltage max_voltage_cell;
    float       segment_voltages[NUM_SEGMENTS];
    float       pack_voltage;
} VoltageStats;
static VoltageStats voltage_stats;

static void calculateVoltageStats(void)
{
    uint8_t voltage_fails = 0;
    for (int i = 0; i < NUM_SEGMENTS; i++)
    {
        for (int j = 0; j < VOLTAGE_REGISTER_GROUPS; j++)
        {
            if (!voltage_read_success[i][j])
            {
                voltage_fails++;
            }
        }
    }
    if (voltage_fails > 0)
    {
        conceq_fails++;
    }
    else
    {
        conceq_fails = 0;
    }

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

typedef struct
{
    uint8_t segment;
    uint8_t cell;
    float   temp;
} CellTemp;
typedef struct
{
    CellTemp min_temp_cell;
    CellTemp max_temp_cell;
    float    avg;
} TempStats;
static TempStats temp_stats;

static void calculateTemperatureStats(void)
{
    uint8_t temp_fails = 0;
    for (int i = 0; i < NUM_SEGMENTS; i++)
    {
        for (int j = 0; j < AUX_REGISTER_GROUPS; j++)
        {
            if (!temp_read_success[i][j])
            {
                temp_fails++;
            }
        }
    }
    if (temp_fails > 0)
    {
        conceq_fails++;
    }
    else
    {
        conceq_fails = 0;
    }

    temp_stats =
        (TempStats){ .min_temp_cell = { .temp = FLT_MAX }, .max_temp_cell = { .temp = -FLT_MAX }, .avg = 0.0f };
    float sum_temp = 0U;
    for (uint8_t curr_segment = 0U; curr_segment < NUM_SEGMENTS; curr_segment++)
    {
        for (uint8_t curr_cell_index = 0U; curr_cell_index < THERMISTORS_PER_SEGMENT; curr_cell_index++)
        {
            const float curr_cell_temp = cell_temps[curr_segment][curr_cell_index];

            // Physical locations of thermistors 1 and 2 are swapped, as well as 7 and 8
            // TODO this is bugged
            if (curr_cell_index == 1)
                curr_cell_index = 2;
            else if (curr_cell_index == 2)
                curr_cell_index = 1;
            else if (curr_cell_index == 7)
                curr_cell_index = 8;
            else if (curr_cell_index == 8)
                curr_cell_index = 7;

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
#define TOTAL_NUM_OF_THERMISTORS (NUM_SEGMENTS * THERMISTORS_PER_SEGMENT)
    temp_stats.avg = sum_temp / TOTAL_NUM_OF_THERMISTORS;
}

static void owcCalculateFaults(void)
{
    // OWCFaults owcFaults = { .cell_owc_status = { 0U }, .owc_fault_gnd = { 0U }, .owc_global_fault = 0U };
    //
    // owcFaults.owc_global_fault = io_ltc6813CellVoltages_getGlobalOpenWireFault();
    //
    // if (owcFaults.owc_global_fault)
    // {
    //     for (uint8_t segment = 0; segment < NUM_SEGMENTS; segment++)
    //     {
    //         if (io_ltc6813CellVoltages_getOpenWireFault(segment, 0))
    //         {
    //             owcFaults.owc_fault_gnd[segment] = true;
    //             owcFaults.cell_owc_status[segment]    = (uint16_t)1;
    //         }
    //         else
    //         {
    //             for (uint8_t cell = 1; cell < CELLS_PER_SEGMENT; cell++)
    //             {
    //                 if (io_ltc6813CellVoltages_getOpenWireFault(segment, cell))
    //                 {
    //                     owcFaults.cell_owc_status[segment] |= ((uint16_t)(1 << cell));
    //                 }
    //             }
    //         }
    //     }
    // }
    //
    // data.owc_faults = owcFaults;
}

static void calculateCellsToBalance(bool cells_to_balance[NUM_SEGMENTS][CELLS_PER_SEGMENT])
{
#define BALANCE_TOLERANCE (0.01f) // 10mV

    const float target_voltage           = app_canRx_Debug_CellBalancingOverrideTarget_get()
                                               ? app_canRx_Debug_CellBalancingOverrideTargetValue_get()
                                               : voltage_stats.min_voltage_cell.voltage + CELL_VOLTAGE_BALANCE_WINDOW_V;
    float       balancing_excess_voltage = 0;
    for (uint8_t segment = 0U; segment < NUM_SEGMENTS; segment++)
    {
        for (uint8_t cell = 0U; cell < CELLS_PER_SEGMENT; cell++)
        {
            // this is equivalent to abs(cell_voltages[segment][cell] - target_voltage) > BALANCE_TOLERANCE
            // as cell_voltages[segment][cell] > target_voltage
            cells_to_balance[segment][cell] = cell_voltages[segment][cell] - target_voltage > BALANCE_TOLERANCE;
            balancing_excess_voltage += MAX(0, cell_voltages[segment][cell] - (target_voltage + BALANCE_TOLERANCE));
        }
    }
    app_canTx_BMS_ExcessVoltage_set(balancing_excess_voltage);
}

void app_accumulator_balanceCells(void)
{
    // Exit early if ADC conversion fails
    if (IS_EXIT_ERR(io_ltc6813_pollAdcConversions()))
    {
        return;
    }

    // Write to configuration register to configure cell discharging
    bool cells_to_balance[NUM_SEGMENTS][CELLS_PER_SEGMENT];
    calculateCellsToBalance(cells_to_balance);
    io_ltc6813_writeConfigurationRegisters(cells_to_balance);

// Discharging cells continuously generates too much heat.
// So balance cells for 100 ticks (the cell monitoring code runs in the 100Hz task, so 100 ticks = 1s),
// then disable discharge for the next 100 ticks to keep temperatures manageable.
// Reducing the ticks to 1 on / 1 off causes noticeable flicker, so I've set it to 1s on / 1s off to be less
// distracting.
#define BALANCE_DEFAULT_FREQ (1)  // Hz
#define BALANCE_DEFAULT_DUTY (50) // %
    // Balance PWM settings
    const float    balance_pwm_freq = app_canRx_Debug_CellBalancingOverridePWM_get()
                                          ? app_canRx_Debug_CellBalancingOverridePWMFrequency_get()
                                          : BALANCE_DEFAULT_FREQ;
    const uint32_t balance_pwm_duty = app_canRx_Debug_CellBalancingOverridePWM_get()
                                          ? app_canRx_Debug_CellBalancingOverridePWMDuty_get()
                                          : BALANCE_DEFAULT_DUTY;

    static uint32_t balance_pwm_ticks;
    static bool     balance_pwm_high;
    // duty_on = 100_ticks_per_sec * 1/freq_Hz * duty_percent / 100
    // TODO: verify frequency calculation. Period seems to be about double what it should be.
    if (balance_pwm_high)
    {
        const uint32_t balance_ticks_on = (uint32_t)(1.0f / balance_pwm_freq * (float)balance_pwm_duty);
        // Enable cell discharging
        io_ltc6813_sendBalanceCommand();
        balance_pwm_ticks += 1;

        if (balance_pwm_ticks >= balance_ticks_on)
        {
            // Cell discharging enabled duty cycle portion is finished
            balance_pwm_high  = false;
            balance_pwm_ticks = 0;
        }
    }
    else
    {
        const uint32_t balance_ticks_off = (uint32_t)(1.0f / balance_pwm_freq * (float)(100 - balance_pwm_duty));
        // Disable cell discharging
        io_ltc6813_sendStopBalanceCommand();
        balance_pwm_ticks += 1;

        if (balance_pwm_ticks >= balance_ticks_off)
        {
            // Cell discharging disabled duty cycle portion is finished
            balance_pwm_high  = true;
            balance_pwm_ticks = 0;
        }
    }
    app_canTx_BMS_BalancingOn_set(balance_pwm_high);
    io_canTx_BMS_BalancingInfo_sendAperiodic();
}

void app_accumulator_init(void)
{
    // Voltage information
    memset(&voltage_stats, 0U, sizeof(VoltageStats));
    data.measurement_state = GET_CELL_VOLTAGE_STATE;

    // Balancing information
    // memset(&data.cells_to_balance, 0U, sizeof(data.cells_to_balance));

    open_wire_pu_readings = 0;
    open_wire_pd_readings = 0;
    owc_idle_cycles       = 0;
    data.owc_state        = START_OPEN_WIRE_CHECK;

    // Init fault debounce timers.
    app_timer_init(&under_voltage_fault_timer, UNDER_VOLTAGE_DEBOUNCE_DURATION_MS);
    app_timer_init(&over_voltage_fault_timer, OVER_VOLTAGE_DEBOUNCE_DURATION_MS);
    app_timer_init(&under_temp_fault_timer, UNDER_TEMP_DEBOUNCE_DURATION_MS);
    app_timer_init(&over_temp_fault_timer, OVER_TEMP_DEBOUNCE_DURATION_MS);
}

void app_accumulator_writeDefaultConfig(void)
{
    // Configure the cell monitoring chips. Disable discharge at startup
    io_ltc6813_writeConfigurationRegisters(NULL);
    io_ltc6813_writeConfigurationRegisters(NULL);
}

void app_accumulator_runCellMeasurements(void)
{
    switch (data.measurement_state)
    {
        case GET_CELL_VOLTAGE_STATE:
        {
            // Attempt to read voltages from the LTCs, write output to cell voltages array
            io_ltc6813_readVoltages(cell_voltages, voltage_read_success);
            calculateVoltageStats();

            // next state
            io_ltc6813_startThermistorsAdcConversion(ADCSpeed_3kHz);
            data.measurement_state = GET_CELL_TEMP_STATE;
            break;
        }
        case GET_CELL_TEMP_STATE:
        {
            float vref = 0.0f;
            io_ltc6813_readTemperatures(cell_temps, &vref, temp_read_success);
            calculateTemperatureStats();
            LOG_INFO("%d", (uint16_t)(vref * 10000));

            // next state
            io_ltc6813_startCellsAdcConversion(ADCSpeed_3kHz);
            data.measurement_state = GET_CELL_VOLTAGE_STATE;
            break;
        }
        default:
            LOG_ERROR("Measurement state is invalid: %d is not valid", data.measurement_state);
            assert(0);
    }
    // Start cell voltage conversions for the next cycle
}

static void parseOWCData(
    float owc_cell_V_PU[NUM_SEGMENTS][CELLS_PER_SEGMENT],
    float owc_cell_V_PD[NUM_SEGMENTS][CELLS_PER_SEGMENT],
    bool  owc_status[NUM_SEGMENTS][CELLS_PER_SEGMENT])
{
    // For cell 0, cell 0 is open if V_PU(0) = 0V
#define OPEN_WIRE_CHECK_CELL_0_THRESHOLD_V (0.1f) // 100mV

    // For cell N in 1-15, cell N is open if V_PU(N+1) - V_PD(N+1) < -400mV
    // * V_PU(N) is pull-up voltage of cell N, i.e. result of ADOW with PUP set to 1
    // * V_PD(N) is pull-down voltage of cell N, i.e. result of ADOW with PUP set to 0
#define OPEN_WIRE_CHECK_CELL_N_THRESHOLD_V (-0.4f) // -400mV

    for (int segment = 0; segment < NUM_SEGMENTS; segment++)
    {
        // Check for cell idx 0
        owc_status[segment][0] = (owc_cell_V_PU[segment][0] < OPEN_WIRE_CHECK_CELL_0_THRESHOLD_V);
        // owc_fault |= cell_owc_status[segment][0];

        // Check for cell idx 1-14
        for (uint8_t cell = 1; cell < CELLS_PER_SEGMENT - 1; cell++)
        {
            const float cell_voltage_delta = owc_cell_V_PU[segment][cell] - owc_cell_V_PD[segment][cell];
            owc_status[segment][cell]      = cell_voltage_delta < OPEN_WIRE_CHECK_CELL_N_THRESHOLD_V;

            // Secondary Test discovered during testing
            owc_status[segment][cell] |=
                APPROX_EQUAL_FLOAT(owc_cell_V_PU[segment][cell], owc_cell_V_PD[segment][cell], 0.01f) &&
                owc_cell_V_PD[segment][cell] > 5.0f;
            // owc_fault |= cell_owc_status[segment][cell];
        }

        // Check for cell idx 15
        owc_status[segment][CELLS_PER_SEGMENT - 1] =
            owc_cell_V_PD[segment][CELLS_PER_SEGMENT - 1] < OPEN_WIRE_CHECK_CELL_0_THRESHOLD_V;
        // owc_fault |= cell_owc_status[segment][15];
    }
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
            const bool Mutex_Acquired = true; // TODO this line is just a reminder to fix it with proper code
            if (Mutex_Acquired)
            {
                io_ltc6813CellVoltages_owcPull(PULL_UP);
                open_wire_pu_readings++;
                data.owc_state = GET_PU_CELL_VOLTAGE_STATE;
            }
            break;
        }
        case GET_PU_CELL_VOLTAGE_STATE:
        {
            if (open_wire_pu_readings >= OPEN_WIRE_CHECK_NUM_ADOW_CMDS)
            {
                io_ltc6813_readVoltages(cell_voltage_pu, cell_voltage_pu_success);
                io_ltc6813CellVoltages_owcPull(PULL_DOWN);
                open_wire_pd_readings++;
                data.owc_state = GET_PD_CELL_VOLTAGE_STATE;
            }
            else
            {
                io_ltc6813CellVoltages_owcPull(PULL_UP);
                open_wire_pu_readings++;
            }
            break;
        }
        case GET_PD_CELL_VOLTAGE_STATE:
        {
            if (open_wire_pd_readings >= OPEN_WIRE_CHECK_NUM_ADOW_CMDS)
            {
                io_ltc6813_readVoltages(cell_voltage_pd, cell_voltage_pd_success);
                data.owc_state = CHECK_OPEN_WIRE_FAULT_STATE;
            }
            else
            {
                io_ltc6813CellVoltages_owcPull(PULL_DOWN);
                open_wire_pd_readings++;
            }
            break;
        }
        case CHECK_OPEN_WIRE_FAULT_STATE:
        {
            parseOWCData(cell_voltage_pu, cell_voltage_pd, cell_owc_status);
            data.owc_state = IDLE_STATE;
            owcCalculateFaults();
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

    // Broadcast OWC information
    // app_canTx_BMS_Segment0_OWC_Cells_Status_set(data.owc_faults.cell_owc_status[0]);
    // app_canTx_BMS_Segment1_OWC_Cells_Status_set(data.owc_faults.cell_owc_status[1]);
    // app_canTx_BMS_Segment2_OWC_Cells_Status_set(data.owc_faults.cell_owc_status[2]);
    // app_canTx_BMS_Segment3_OWC_Cells_Status_set(data.owc_faults.cell_owc_status[3]);

    // Calculate and broadcast pack power.
    const float available_power =
        MIN(app_math_linearDerating(
                temp_stats.max_temp_cell.temp, MAX_POWER_LIMIT_W, TEMP_WARNING_THRESHOLD, TEMP_FAULT_THRESHOLD),
            MAX_POWER_LIMIT_W);

    app_canTx_BMS_AvailablePower_set((uint32_t)available_power);
}

bool app_accumulator_checkFaults(void)
{
    // if we are charging, max cell temp is 45C not 60C
    const float max_allowable_cell_temp =
        app_canRx_BRUSA_IsConnected_get() ? MAX_CELL_CHARGE_TEMP_DEGC : MAX_CELL_DISCHARGE_TEMP_DEGC;
    const float min_allowable_cell_temp =
        app_canRx_BRUSA_IsConnected_get() ? MIN_CELL_CHARGE_TEMP_DEGC : MIN_CELL_DISCHARGE_TEMP_DEGC;

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
    app_canTx_BMS_ModuleCommunication_NumCommTries_set(conceq_fails);
    app_canTx_BMS_ModuleCommunication_MonitorState_set((CAN_AccumulatorMonitorState)data.measurement_state);
    app_canAlerts_BMS_Fault_ModuleCommunicationError_set(communication_fault);

    // const bool owc_fault = data.owc_faults.owc_global_fault;
    // app_canAlerts_BMS_Warning_OpenWireCheckFault_set(data.owc_faults.owc_global_fault);
    // app_canAlerts_BMS_Warning_OpenWireCheck_Segment0_GND_set(data.owc_faults.owc_fault_gnd[0]);
    // app_canAlerts_BMS_Warning_OpenWireCheck_Segment1_GND_set(data.owc_faults.owc_fault_gnd[1]);
    // app_canAlerts_BMS_Warning_OpenWireCheck_Segment2_GND_set(data.owc_faults.owc_fault_gnd[2]);
    // app_canAlerts_BMS_Warning_OpenWireCheck_Segment3_GND_set(data.owc_faults.owc_fault_gnd[3]);
    // app_canAlerts_BMS_Warning_OpenWireCheck_Segment4_GND_set(data.owc_faults.owc_fault_gnd[4]);

    return over_temp_fault || under_temp_fault || over_voltage_fault || under_voltage_fault || communication_fault;
}

float app_accumulator_getPackVoltage(void)
{
    return voltage_stats.pack_voltage;
}

float app_accumulator_getMinCellVoltage(uint8_t *segment, uint8_t *cell)
{
    if (segment != NULL)
    {
        *segment = voltage_stats.min_voltage_cell.segment;
    }

    if (cell != NULL)
    {
        *cell = voltage_stats.min_voltage_cell.cell;
    }

    return voltage_stats.min_voltage_cell.voltage;
}

float app_accumulator_getMaxCellVoltage(uint8_t *segment, uint8_t *cell)
{
    if (segment != NULL)
    {
        *segment = voltage_stats.max_voltage_cell.segment;
    }

    if (cell != NULL)
    {
        *cell = voltage_stats.max_voltage_cell.cell;
    }

    return voltage_stats.max_voltage_cell.voltage;
}
