#include "states/app_states.h"

#include "app_segments.h"
#include "app_segments_internal.h"
#include "app_timer.h"
#include "app_canAlerts.h"
#include "app_canRx.h"
#include "io_ltc6813.h"

// Allows balancing of cells even if slight over-charging occurs. Occured prior to Competition 2024, where a fully
// charged pack with max cell V of 4.19 after charging reported as 4.21 after settling. Cause currently unknown, but
// this allows for these over-charged cells to be discharged back to safe limits
#define MAX_CELL_VOLTAGE_BALANCING (4.25f)
#define MAX_CELL_CHARGE_TEMP_DEGC (45.0f)

bool voltage_comm_ok[NUM_SEGMENTS];
bool temp_comm_ok[NUM_SEGMENTS];

typedef struct
{
    float    min_voltage;
    float    max_voltage;
    float    max_temp;
    uint32_t under_voltage_debounce_ms;
    uint32_t over_voltage_debounce_ms;
    uint32_t over_temp_debounce_ms;
    uint32_t comm_err_debounce_ms;
    void (*under_voltage_setter)(bool);
    void (*over_voltage_setter)(bool);
    void (*over_temp_setter)(bool);
    void (*comm_err_setter)(bool);
} ProfileConfig;

typedef struct
{
    const ProfileConfig *config;
    TimerChannel         under_voltage_fault_timer;
    TimerChannel         over_voltage_fault_timer;
    TimerChannel         over_temp_fault_timer;
    TimerChannel         comm_err_fault_timer;
} Profile;

// TODO: Get final values for these guys (check with Joe).
static const ProfileConfig warning_profile_config = {
    // voltages
    .min_voltage = MIN_CELL_VOLTAGE_WARNING_V,
    .max_voltage = MAX_CELL_VOLTAGE_WARNING_V,
    // temps
    .max_temp = MAX_CELL_TEMP_WARNING_C,
    // debounce
    .under_voltage_debounce_ms = UNDER_VOLTAGE_DEBOUNCE_WARNING_MS,
    .over_voltage_debounce_ms  = OVER_VOLTAGE_DEBOUNCE_WARNING_MS,
    .over_temp_debounce_ms     = OVER_TEMP_DEBOUNCE_WARNING_MS,
    .comm_err_debounce_ms      = COMM_ERR_DEBOUNCE_WARNING_MS,
    // setters
    .under_voltage_setter = app_canAlerts_BMS_Warning_CellUndervoltage_set,
    .over_voltage_setter  = app_canAlerts_BMS_Warning_CellOvervoltage_set,
    .over_temp_setter     = app_canAlerts_BMS_Warning_CellOvertemp_set,
    .comm_err_setter      = app_canAlerts_BMS_Warning_ModuleCommunicationError_set,
};
static const ProfileConfig fault_profile_config = {
    // voltages
    .min_voltage = MIN_CELL_VOLTAGE_FAULT_V,
    .max_voltage = MAX_CELL_VOLTAGE_FAULT_V,
    // temps
    .max_temp = MAX_CELL_TEMP_FAULT_C,
    // debounce
    .under_voltage_debounce_ms = UNDER_VOLTAGE_DEBOUNCE_FAULT_MS,
    .over_voltage_debounce_ms  = OVER_VOLTAGE_DEBOUNCE_FAULT_MS,
    .over_temp_debounce_ms     = OVER_TEMP_DEBOUNCE_FAULT_MS,
    .comm_err_debounce_ms      = COMM_ERR_DEBOUNCE_FAULT_MS,
    // setters
    .under_voltage_setter = app_canAlerts_BMS_Fault_CellUndervoltage_set,
    .over_voltage_setter  = app_canAlerts_BMS_Fault_CellOvervoltage_set,
    .over_temp_setter     = app_canAlerts_BMS_Fault_CellOvertemp_set,
    .comm_err_setter      = app_canAlerts_BMS_Fault_ModuleCommunicationError_set,
};

static Profile fault_profile;
static Profile warning_profile;

static TimerChannel cell_monitor_settle_timer;
// Time for voltage and cell temperature values to settle
#define CELL_MONITOR_TIME_TO_SETTLE_MS (300U)

static void profileInit(Profile *profile, const ProfileConfig *config)
{
    app_timer_init(&profile->under_voltage_fault_timer, config->under_voltage_debounce_ms);
    app_timer_init(&profile->over_voltage_fault_timer, config->over_voltage_debounce_ms);
    app_timer_init(&profile->over_temp_fault_timer, config->over_temp_debounce_ms);
    app_timer_init(&profile->comm_err_fault_timer, config->comm_err_debounce_ms);
    profile->config = config;

    app_timer_init(&cell_monitor_settle_timer, CELL_MONITOR_TIME_TO_SETTLE_MS);
    app_timer_restart(&cell_monitor_settle_timer);
}

static bool checkProfile(Profile *profile)
{
    // if we are charging, max cell temp is 45C not 60C
    const bool  is_charging             = app_stateMachine_getCurrentState() == &charge_state;
    const float max_allowable_cell_temp = is_charging ? MAX_CELL_CHARGE_TEMP_DEGC : profile->config->max_temp;

    // Check if balancing is enabled. For safety reasons, also check if current state is charge state, as we do not want
    // to adjust max cell voltage thresholds if actively charging.
    const bool cell_balancing_enabled = app_canRx_Debug_CellBalancingRequest_get() && !is_charging;

    // Allows balancing of cells even if slight over-charging occurs. Occured prior to Competition 2024, where a fully
    // charged pack with max cell V of 4.19 after charging reported as 4.21 after settling. Cause currently unknown, but
    // this allows for these over-charged cells to be discharged back to safe limits
    const float max_allowable_cell_voltage =
        cell_balancing_enabled ? MAX_CELL_VOLTAGE_BALANCING : profile->config->max_voltage;

    const bool over_temp_condition     = max_cell_temp.value > max_allowable_cell_temp;
    const bool over_voltage_condition  = max_cell_voltage.value > max_allowable_cell_voltage;
    const bool under_voltage_condition = min_cell_voltage.value < profile->config->min_voltage;

    // See if any segments have a comm err present.
    bool comm_ok = true;
    for (uint8_t segment = 0; segment < NUM_SEGMENTS; segment++)
    {
        comm_ok &= ALL_COMM_OK(segment);
    }
    const bool comm_err = !comm_ok;

    // Wait for cell voltage and temperature measurements to settle. We expect to read back valid values from the
    // monitoring chips within 3 cycles
    const bool settle_time_expired = app_timer_updateAndGetState(&cell_monitor_settle_timer) == TIMER_STATE_EXPIRED;

    const bool under_voltage_debounced =
        app_timer_runIfCondition(&profile->under_voltage_fault_timer, under_voltage_condition) == TIMER_STATE_EXPIRED &&
        settle_time_expired;
    const bool over_voltage_debounced =
        app_timer_runIfCondition(&profile->over_voltage_fault_timer, over_voltage_condition) == TIMER_STATE_EXPIRED &&
        settle_time_expired;
    const bool over_temp_debounced =
        app_timer_runIfCondition(&profile->over_temp_fault_timer, over_temp_condition) == TIMER_STATE_EXPIRED &&
        settle_time_expired;
    const bool comm_err_debounced =
        app_timer_runIfCondition(&profile->comm_err_fault_timer, comm_err) == TIMER_STATE_EXPIRED &&
        settle_time_expired;

    profile->config->under_voltage_setter(under_voltage_debounced);
    profile->config->over_voltage_setter(over_voltage_debounced);
    profile->config->over_temp_setter(over_temp_debounced);
    profile->config->comm_err_setter(comm_err_debounced);

    return under_voltage_debounced || over_voltage_debounced || over_temp_debounced || comm_err_debounced;
}

void app_segments_initFaults(void)
{
    profileInit(&fault_profile, &fault_profile_config);
    profileInit(&warning_profile, &warning_profile_config);
}

bool app_segments_checkWarnings(void)
{
    return checkProfile(&warning_profile);
}

bool app_segments_checkFaults(void)
{
    return checkProfile(&fault_profile);
}
