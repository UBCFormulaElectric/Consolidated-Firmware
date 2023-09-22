#include "Test_Faults.h"
#include "App_Accumulator.h"

namespace FaultTest
{
FAKE_VOID_FUNC(send_non_periodic_msg_BMS_STARTUP, const struct CanMsgs_bms_startup_t *);
FAKE_VOID_FUNC(send_non_periodic_msg_BMS_WATCHDOG_TIMEOUT, const struct CanMsgs_bms_watchdog_timeout_t *);
FAKE_VALUE_FUNC(float, get_pwm_frequency);
FAKE_VALUE_FUNC(float, get_pwm_duty_cycle);
FAKE_VALUE_FUNC(uint16_t, get_seconds_since_power_on);
FAKE_VALUE_FUNC(uint32_t, get_current_ms);
FAKE_VOID_FUNC(turn_on_red_led);
FAKE_VOID_FUNC(turn_on_green_led);
FAKE_VOID_FUNC(turn_on_blue_led);
FAKE_VOID_FUNC(enable_charger);
FAKE_VOID_FUNC(disable_charger);
FAKE_VALUE_FUNC(bool, is_charger_connected);
FAKE_VALUE_FUNC(bool, has_charger_faulted);
FAKE_VALUE_FUNC(ExitCode, enable_bms_ok);
FAKE_VALUE_FUNC(ExitCode, disable_bms_ok);
FAKE_VALUE_FUNC(bool, is_bms_ok_enabled);
FAKE_VALUE_FUNC(ExitCode, enable_imd_ok);
FAKE_VALUE_FUNC(ExitCode, disable_imd_ok);
FAKE_VALUE_FUNC(bool, is_imd_ok_enabled);
FAKE_VALUE_FUNC(ExitCode, enable_bspd_ok);
FAKE_VALUE_FUNC(ExitCode, disable_bspd_ok);
FAKE_VALUE_FUNC(bool, is_bspd_ok_enabled);
FAKE_VALUE_FUNC(bool, is_air_negative_closed);
FAKE_VALUE_FUNC(bool, is_air_positive_closed);
FAKE_VOID_FUNC(open_air_positive);
FAKE_VOID_FUNC(close_air_positive);
FAKE_VOID_FUNC(enable_pre_charge);
FAKE_VOID_FUNC(disable_pre_charge);
FAKE_VALUE_FUNC(bool, configure_cell_monitors);
FAKE_VALUE_FUNC(bool, start_voltage_conv);
FAKE_VALUE_FUNC(float, get_ts_voltage);
FAKE_VALUE_FUNC(float, get_low_res_current);
FAKE_VALUE_FUNC(float, get_high_res_current);
FAKE_VALUE_FUNC(bool, start_temp_conv);
FAKE_VALUE_FUNC(bool, read_cell_temperatures);
FAKE_VOID_FUNC(thermistor_mux_select, uint8_t);
FAKE_VALUE_FUNC(float, read_thermistor_temp);
FAKE_VALUE_FUNC(float, get_min_temp_degc, uint8_t *, uint8_t *);
FAKE_VALUE_FUNC(float, get_max_temp_degc, uint8_t *, uint8_t *);
FAKE_VALUE_FUNC(float, get_avg_temp_degc);
FAKE_VALUE_FUNC(bool, enable_balance);
FAKE_VALUE_FUNC(bool, disable_balance);
FAKE_VALUE_FUNC(bool, check_imd_latched_fault);
FAKE_VALUE_FUNC(bool, check_bspd_latched_fault);
FAKE_VALUE_FUNC(bool, check_bms_latched_fault);
FAKE_VALUE_FUNC(EEPROM_StatusTypeDef, read_page, uint16_t, uint8_t, uint8_t *, uint16_t);
FAKE_VALUE_FUNC(EEPROM_StatusTypeDef, write_page, uint16_t, uint8_t, uint8_t *, uint16_t);
FAKE_VALUE_FUNC(EEPROM_StatusTypeDef, page_erase, uint16_t);

static float saved_soc = 100.0f;

static bool
    write_cfg_registers(bool cells_to_balance[ACCUMULATOR_NUM_SEGMENTS][ACCUMULATOR_NUM_SERIES_CELLS_PER_SEGMENT])
{
    // Couldn't figure out how to create a function with a 2D-array as a parameter using FFF
    UNUSED(cells_to_balance);
    return true;
}

static float cell_voltages[ACCUMULATOR_NUM_SEGMENTS][ACCUMULATOR_NUM_SERIES_CELLS_PER_SEGMENT];

static bool read_cell_voltages(float voltages[ACCUMULATOR_NUM_SEGMENTS][ACCUMULATOR_NUM_SERIES_CELLS_PER_SEGMENT])
{
    for (uint8_t segment = 0; segment < ACCUMULATOR_NUM_SEGMENTS; segment++)
    {
        for (uint8_t cell = 0; cell < ACCUMULATOR_NUM_SERIES_CELLS_PER_SEGMENT; cell++)
        {
            voltages[segment][cell] = cell_voltages[segment][cell];
        }
    }

    return true;
}

static void set_cell_voltage(AccumulatorSegment segment, uint8_t cell, float voltage)
{
    if (segment < ACCUMULATOR_NUM_SEGMENTS && cell < ACCUMULATOR_NUM_SERIES_CELLS_PER_SEGMENT)
    {
        cell_voltages[segment][cell] = voltage;
    }
}

static void set_all_cell_voltages(float voltage)
{
    for (uint8_t segment = 0; segment < ACCUMULATOR_NUM_SEGMENTS; segment++)
    {
        for (uint8_t cell = 0; cell < ACCUMULATOR_NUM_SERIES_CELLS_PER_SEGMENT; cell++)
        {
            set_cell_voltage((AccumulatorSegment)segment, cell, voltage);
        }
    }
}

class BmsFaultTest : public BaseStateMachineTest
{
  protected:
    void SetUp() override
    {
        BaseStateMachineTest::SetUp();

        App_CanTx_Init();
        App_CanRx_Init();

        imd =
            App_Imd_Create(get_pwm_frequency, IMD_FREQUENCY_TOLERANCE, get_pwm_duty_cycle, get_seconds_since_power_on);

        heartbeat_monitor = App_SharedHeartbeatMonitor_Create(
            get_current_ms, HEARTBEAT_MONITOR_TIMEOUT_PERIOD_MS, HEARTBEAT_MONITOR_BOARDS_TO_CHECK);

        rgb_led_sequence = App_SharedRgbLedSequence_Create(turn_on_red_led, turn_on_green_led, turn_on_blue_led);

        charger = App_Charger_Create(enable_charger, disable_charger, is_charger_connected, has_charger_faulted);

        bms_ok = App_OkStatus_Create(enable_bms_ok, disable_bms_ok, is_bms_ok_enabled);

        imd_ok = App_OkStatus_Create(enable_imd_ok, disable_imd_ok, is_imd_ok_enabled);

        bspd_ok = App_OkStatus_Create(enable_bspd_ok, disable_bspd_ok, is_bspd_ok_enabled);

        accumulator = App_Accumulator_Create(
            configure_cell_monitors, write_cfg_registers, start_voltage_conv, read_cell_voltages, start_temp_conv,
            read_cell_temperatures, get_min_temp_degc, get_max_temp_degc, get_avg_temp_degc, enable_balance,
            disable_balance, check_imd_latched_fault, check_bspd_latched_fault, check_bms_latched_fault,
            thermistor_mux_select, read_thermistor_temp);

        precharge_relay = App_PrechargeRelay_Create(enable_pre_charge, disable_pre_charge);

        ts = App_TractiveSystem_Create(get_ts_voltage, get_high_res_current, get_low_res_current);

        airs = App_Airs_Create(is_air_positive_closed, is_air_negative_closed, close_air_positive, open_air_positive);

        clock = App_SharedClock_Create();

        eeprom = App_Eeprom_Create(write_page, read_page, page_erase);

        soc_stats = App_SocStats_Create(saved_soc, DEFAULT_SOC_ADDR, accumulator);

        world = App_BmsWorld_Create(
            imd, heartbeat_monitor, rgb_led_sequence, charger, bms_ok, imd_ok, bspd_ok, accumulator, soc_stats, airs,
            precharge_relay, ts, clock, eeprom);

        // Default to starting the state machine in the `init` state
        state_machine = App_SharedStateMachine_Create(world, App_GetInitState());
        App_AllStates_Init();

        RESET_FAKE(send_non_periodic_msg_BMS_STARTUP);
        RESET_FAKE(send_non_periodic_msg_BMS_WATCHDOG_TIMEOUT);
        RESET_FAKE(get_pwm_frequency);
        RESET_FAKE(get_pwm_duty_cycle);
        RESET_FAKE(get_seconds_since_power_on);
        RESET_FAKE(get_current_ms);
        RESET_FAKE(turn_on_red_led);
        RESET_FAKE(turn_on_green_led);
        RESET_FAKE(turn_on_blue_led);
        RESET_FAKE(enable_charger);
        RESET_FAKE(disable_charger);
        RESET_FAKE(enable_bms_ok);
        RESET_FAKE(disable_bms_ok);
        RESET_FAKE(is_bms_ok_enabled);
        RESET_FAKE(enable_imd_ok);
        RESET_FAKE(disable_imd_ok);
        RESET_FAKE(is_imd_ok_enabled);
        RESET_FAKE(enable_bspd_ok);
        RESET_FAKE(disable_bspd_ok);
        RESET_FAKE(is_bspd_ok_enabled);
        RESET_FAKE(is_air_negative_closed);
        RESET_FAKE(is_air_positive_closed);
        RESET_FAKE(configure_cell_monitors);
        RESET_FAKE(start_voltage_conv);
        RESET_FAKE(get_low_res_current);
        RESET_FAKE(get_high_res_current);
        RESET_FAKE(read_page);
        RESET_FAKE(write_page);
        RESET_FAKE(page_erase);

        // Set initial voltages to nominal value
        set_all_cell_voltages(3.8);
        start_voltage_conv_fake.return_val = true;

        // A temperature in [0.0, 60.0] degC to prevent other tests from entering the fault state
        get_min_temp_degc_fake.return_val = 20.0f;
        get_max_temp_degc_fake.return_val = 20.0f;
    }

    void TearDown() override
    {
        TearDownObject(world, App_BmsWorld_Destroy);
        TearDownObject(state_machine, App_SharedStateMachine_Destroy);
        TearDownObject(imd, App_Imd_Destroy);
        TearDownObject(heartbeat_monitor, App_SharedHeartbeatMonitor_Destroy);
        TearDownObject(rgb_led_sequence, App_SharedRgbLedSequence_Destroy);
        TearDownObject(charger, App_Charger_Destroy);
        TearDownObject(bms_ok, App_OkStatus_Destroy);
        TearDownObject(imd_ok, App_OkStatus_Destroy);
        TearDownObject(bspd_ok, App_OkStatus_Destroy);
        TearDownObject(accumulator, App_Accumulator_Destroy);
        TearDownObject(soc_stats, App_SocStats_Destroy);
        TearDownObject(airs, App_Airs_Destroy);
        TearDownObject(precharge_relay, App_PrechargeRelay_Destroy);
        TearDownObject(ts, App_TractiveSystem_Destroy);
        TearDownObject(clock, App_SharedClock_Destroy);
        TearDownObject(eeprom, App_Eeprom_Destroy);
    }

    void SetInitialState(const struct State *const initial_state)
    {
        TearDownObject(state_machine, App_SharedStateMachine_Destroy);
        state_machine = App_SharedStateMachine_Create(world, initial_state);
        ASSERT_EQ(initial_state, App_SharedStateMachine_GetCurrentState(state_machine));
    }

    std::vector<const struct State *> GetAllStates(void)
    {
        return std::vector<const struct State *>{
            App_GetInitState(), App_GetPreChargeState(), App_GetDriveState(), App_GetChargeState(), App_GetFaultState(),
        };
    }

    void UpdateClock(struct StateMachine *state_machine, uint32_t current_time_ms) override
    {
        struct BmsWorld *world = App_SharedStateMachine_GetWorld(state_machine);
        struct Clock *   clock = App_BmsWorld_GetClock(world);
        App_SharedClock_SetCurrentTimeInMilliseconds(clock, current_time_ms);
    }

    void UpdateSignals(struct StateMachine *state_machine, uint32_t current_time_ms) override
    {
        // BMS doesn't use any signals currently
        UNUSED(state_machine);
        UNUSED(current_time_ms);
    }

    struct World *            world;
    struct StateMachine *     state_machine;
    struct BmsCanTxInterface *can_tx_interface;
    struct BmsCanRxInterface *can_rx_interface;
    struct Imd *              imd;
    struct HeartbeatMonitor * heartbeat_monitor;
    struct RgbLedSequence *   rgb_led_sequence;
    struct Charger *          charger;
    struct OkStatus *         bms_ok;
    struct OkStatus *         imd_ok;
    struct OkStatus *         bspd_ok;
    struct Accumulator *      accumulator;
    struct SocStats *         soc_stats;
    struct Airs *             airs;
    struct PrechargeRelay *   precharge_relay;
    struct TractiveSystem *   ts;
    struct Clock *            clock;
    struct Eeprom *           eeprom;
};

TEST_F(BmsFaultTest, check_state_transition_to_fault_state_from_all_states_overvoltage)
{
    // Test that any cell can cause an overvoltage fault
    for (uint8_t segment = 0; segment < ACCUMULATOR_NUM_SEGMENTS; segment++)
    {
        for (uint8_t cell = 0; cell < ACCUMULATOR_NUM_SERIES_CELLS_PER_SEGMENT; cell++)
        {
            // Reset test
            TearDown();
            SetUp();

            // Let accumulator startup count expire
            LetTimePass(state_machine, 1000);
            ASSERT_EQ(App_GetInitState(), App_SharedStateMachine_GetCurrentState(state_machine));
            ASSERT_FALSE(App_CanAlerts_GetFault(BMS_FAULT_CELL_OVERVOLTAGE));

            // Set cell voltage critically high and confirm fault is set
            set_cell_voltage((AccumulatorSegment)segment, cell, MAX_CELL_VOLTAGE + 0.1f);
            LetTimePass(state_machine, 20);
            ASSERT_EQ(App_GetFaultState(), App_SharedStateMachine_GetCurrentState(state_machine));
            ASSERT_TRUE(App_CanAlerts_GetFault(BMS_FAULT_CELL_OVERVOLTAGE));

            LetTimePass(state_machine, 1000);
            ASSERT_EQ(App_GetFaultState(), App_SharedStateMachine_GetCurrentState(state_machine));
            ASSERT_TRUE(App_CanAlerts_GetFault(BMS_FAULT_CELL_OVERVOLTAGE));

            // Clear fault, should transition back to init
            set_cell_voltage((AccumulatorSegment)segment, cell, MAX_CELL_VOLTAGE - 0.1f);
            LetTimePass(state_machine, 20);
            ASSERT_EQ(App_GetInitState(), App_SharedStateMachine_GetCurrentState(state_machine));
            ASSERT_FALSE(App_CanAlerts_GetFault(BMS_FAULT_CELL_OVERVOLTAGE));
        }
    }
}

TEST_F(BmsFaultTest, check_state_transition_to_fault_state_from_all_states_undervoltage)
{
    // Test that any cell can cause an undervoltage fault
    for (uint8_t segment = 0; segment < ACCUMULATOR_NUM_SEGMENTS; segment++)
    {
        for (uint8_t cell = 0; cell < ACCUMULATOR_NUM_SERIES_CELLS_PER_SEGMENT; cell++)
        {
            // Reset test
            TearDown();
            SetUp();

            // Let accumulator startup count expire
            LetTimePass(state_machine, 1000);
            ASSERT_EQ(App_GetInitState(), App_SharedStateMachine_GetCurrentState(state_machine));
            ASSERT_FALSE(App_CanAlerts_GetFault(BMS_FAULT_CELL_UNDERVOLTAGE));

            // Set cell voltage critically low and confirm fault is set
            set_cell_voltage((AccumulatorSegment)segment, cell, MIN_CELL_VOLTAGE - 0.1f);
            LetTimePass(state_machine, 20);
            ASSERT_EQ(App_GetFaultState(), App_SharedStateMachine_GetCurrentState(state_machine));
            ASSERT_TRUE(App_CanAlerts_GetFault(BMS_FAULT_CELL_UNDERVOLTAGE));

            LetTimePass(state_machine, 1000);
            ASSERT_EQ(App_GetFaultState(), App_SharedStateMachine_GetCurrentState(state_machine));
            ASSERT_TRUE(App_CanAlerts_GetFault(BMS_FAULT_CELL_UNDERVOLTAGE));

            // Clear fault, should transition back to init
            set_cell_voltage((AccumulatorSegment)segment, cell, MIN_CELL_VOLTAGE + 0.1f);
            LetTimePass(state_machine, 20);
            ASSERT_EQ(App_GetInitState(), App_SharedStateMachine_GetCurrentState(state_machine));
            ASSERT_FALSE(App_CanAlerts_GetFault(BMS_FAULT_CELL_UNDERVOLTAGE));
        }
    }
}

TEST_F(BmsFaultTest, check_state_transition_to_fault_state_from_all_states_overtemp_init_state)
{
    // Set TS current negative to trigger discharging condition in tempertature check
    get_high_res_current_fake.return_val   = -10.0f;
    get_low_res_current_fake.return_val    = -10.0f;
    is_air_negative_closed_fake.return_val = true;

    SetInitialState(App_GetInitState());

    is_charger_connected_fake.return_val = true;
    App_CanRx_Debug_ChargingSwitch_StartCharging_Update(false);

    // Let accumulator startup count expire
    LetTimePass(state_machine, 1000);
    ASSERT_EQ(App_GetInitState(), App_SharedStateMachine_GetCurrentState(state_machine));
    ASSERT_FALSE(App_CanAlerts_GetFault(BMS_FAULT_CELL_OVERTEMP));

    // In Discharge state, acceptible temp range is (-20, 60), should be unaffected by temp of 46 C
    get_max_temp_degc_fake.return_val = MAX_CELL_CHARGE_TEMP_DEGC + 1.0f;
    LetTimePass(state_machine, 10);
    ASSERT_EQ(App_GetInitState(), App_SharedStateMachine_GetCurrentState(state_machine));
    ASSERT_FALSE(App_CanAlerts_GetFault(BMS_FAULT_CELL_OVERTEMP));

    LetTimePass(state_machine, 1000);

    get_max_temp_degc_fake.return_val = MAX_CELL_DISCHARGE_TEMP_DEGC + 1.0f;
    LetTimePass(state_machine, 10);

    ASSERT_EQ(App_GetFaultState(), App_SharedStateMachine_GetCurrentState(state_machine));
    ASSERT_TRUE(App_CanAlerts_GetFault(BMS_FAULT_CELL_OVERTEMP));

    // Confirm stays in fault indefinitely
    LetTimePass(state_machine, 1000);
    ASSERT_EQ(App_GetFaultState(), App_SharedStateMachine_GetCurrentState(state_machine));
    ASSERT_TRUE(App_CanAlerts_GetFault(BMS_FAULT_CELL_OVERTEMP));

    // Clear fault, should transition back to init
    get_max_temp_degc_fake.return_val      = MAX_CELL_DISCHARGE_TEMP_DEGC - 1.0f;
    has_charger_faulted_fake.return_val    = false;
    is_air_negative_closed_fake.return_val = false;
    LetTimePass(state_machine, 10);
    ASSERT_EQ(App_GetInitState(), App_SharedStateMachine_GetCurrentState(state_machine));
    ASSERT_FALSE(App_CanAlerts_GetFault(BMS_FAULT_CELL_OVERTEMP));
}

TEST_F(BmsFaultTest, check_state_transition_to_fault_state_from_all_states_overtemp_charge_state)
{
    // Set charger conditions such that charger faults do not trigger
    is_charger_connected_fake.return_val   = true;
    is_air_negative_closed_fake.return_val = true;
    has_charger_faulted_fake.return_val    = false;

    // Set TS current positive to trigger charging condition in temperature check
    get_high_res_current_fake.return_val = 10.0f;
    get_low_res_current_fake.return_val  = 10.0f;

    SetInitialState(App_GetChargeState());
    App_CanRx_Debug_ChargingSwitch_StartCharging_Update(true);

    // Let accumulator startup count expire
    LetTimePass(state_machine, 1000);
    ASSERT_EQ(App_GetChargeState(), App_SharedStateMachine_GetCurrentState(state_machine));
    ASSERT_FALSE(App_CanAlerts_GetFault(BMS_FAULT_CELL_OVERTEMP));

    // In Charge state acceptible temp range is (0, 45)
    get_max_temp_degc_fake.return_val = MAX_CELL_CHARGE_TEMP_DEGC + 1.0f;
    LetTimePass(state_machine, 10);
    ASSERT_EQ(App_GetFaultState(), App_SharedStateMachine_GetCurrentState(state_machine));
    ASSERT_TRUE(App_CanAlerts_GetFault(BMS_FAULT_CELL_OVERTEMP));

    // Confirm stays in fault indefinitely
    LetTimePass(state_machine, 1000);
    ASSERT_EQ(App_GetFaultState(), App_SharedStateMachine_GetCurrentState(state_machine));
    ASSERT_TRUE(App_CanAlerts_GetFault(BMS_FAULT_CELL_OVERTEMP));

    // Clear fault, should transition back to init
    get_max_temp_degc_fake.return_val      = MAX_CELL_CHARGE_TEMP_DEGC - 1.0f;
    is_air_negative_closed_fake.return_val = false; // Negative contactor has to open to go back to init
    LetTimePass(state_machine, 10);
    ASSERT_EQ(App_GetInitState(), App_SharedStateMachine_GetCurrentState(state_machine));
    ASSERT_FALSE(App_CanAlerts_GetFault(BMS_FAULT_CELL_OVERTEMP));
}

TEST_F(BmsFaultTest, check_state_transition_to_fault_state_from_all_states_undertemp_init_state)
{
    // Set TS current negative to trigger discharging condition in tempertature check
    get_high_res_current_fake.return_val   = -10.0f;
    get_low_res_current_fake.return_val    = -10.0f;
    is_air_negative_closed_fake.return_val = true;

    SetInitialState(App_GetInitState());

    // Let accumulator startup count expire
    LetTimePass(state_machine, 1000);
    ASSERT_EQ(App_GetInitState(), App_SharedStateMachine_GetCurrentState(state_machine));
    ASSERT_FALSE(App_CanAlerts_GetFault(BMS_FAULT_CELL_UNDERTEMP));

    // In Discharge state, acceptible temp range is (-20, 60), should be unaffected by temp of -1 C
    get_min_temp_degc_fake.return_val = MIN_CELL_CHARGE_TEMP_DEGC - 1.0f;
    LetTimePass(state_machine, 10);
    ASSERT_EQ(App_GetInitState(), App_SharedStateMachine_GetCurrentState(state_machine));
    ASSERT_FALSE(App_CanAlerts_GetFault(BMS_FAULT_CELL_UNDERTEMP));

    LetTimePass(state_machine, 1000);

    get_min_temp_degc_fake.return_val = MIN_CELL_DISCHARGE_TEMP_DEGC - 1.0f;
    LetTimePass(state_machine, 10);
    ASSERT_EQ(App_GetFaultState(), App_SharedStateMachine_GetCurrentState(state_machine));
    ASSERT_TRUE(App_CanAlerts_GetFault(BMS_FAULT_CELL_UNDERTEMP));

    // Confirm stays in fault indefinitely
    LetTimePass(state_machine, 1000);
    ASSERT_EQ(App_GetFaultState(), App_SharedStateMachine_GetCurrentState(state_machine));
    ASSERT_TRUE(App_CanAlerts_GetFault(BMS_FAULT_CELL_UNDERTEMP));

    // Clear fault, should transition back to init
    get_min_temp_degc_fake.return_val      = MIN_CELL_DISCHARGE_TEMP_DEGC + 1.0f;
    is_air_negative_closed_fake.return_val = false; // Negative contactor has to open to go back to init
    LetTimePass(state_machine, 10);
    ASSERT_EQ(App_GetInitState(), App_SharedStateMachine_GetCurrentState(state_machine));
    ASSERT_FALSE(App_CanAlerts_GetFault(BMS_FAULT_CELL_UNDERTEMP));
}

TEST_F(BmsFaultTest, check_state_transition_to_fault_state_from_all_states_undertemp_charge_state)
{
    // Set charger conditions such that charger faults do not trigger
    is_charger_connected_fake.return_val   = true;
    is_air_negative_closed_fake.return_val = true;
    has_charger_faulted_fake.return_val    = false;
    App_CanRx_Debug_ChargingSwitch_StartCharging_Update(true);

    // Set TS current positive to trigger charging condition in tempertature check and above threshold to remain
    // charging
    get_high_res_current_fake.return_val = 10.0f;
    get_low_res_current_fake.return_val  = 10.0f;

    SetInitialState(App_GetChargeState());

    // Let accumulator startup count expire
    LetTimePass(state_machine, 1000);
    ASSERT_EQ(App_GetChargeState(), App_SharedStateMachine_GetCurrentState(state_machine));
    ASSERT_FALSE(App_CanAlerts_GetFault(BMS_FAULT_CELL_UNDERTEMP));

    // In Charge state acceptable temp range is (0, 45)
    get_min_temp_degc_fake.return_val = MIN_CELL_CHARGE_TEMP_DEGC - 1.0f;
    LetTimePass(state_machine, 10);
    ASSERT_EQ(App_GetFaultState(), App_SharedStateMachine_GetCurrentState(state_machine));
    ASSERT_TRUE(App_CanAlerts_GetFault(BMS_FAULT_CELL_UNDERTEMP));

    // Confirm stays in fault indefinitely
    LetTimePass(state_machine, 1000);
    ASSERT_EQ(App_GetFaultState(), App_SharedStateMachine_GetCurrentState(state_machine));
    ASSERT_TRUE(App_CanAlerts_GetFault(BMS_FAULT_CELL_UNDERTEMP));

    // Clear fault, should transition back to init
    get_min_temp_degc_fake.return_val      = MIN_CELL_CHARGE_TEMP_DEGC + 1.0f;
    is_air_negative_closed_fake.return_val = false; // Negative contactor has to open to go back to init
    LetTimePass(state_machine, 10);
    ASSERT_EQ(App_GetInitState(), App_SharedStateMachine_GetCurrentState(state_machine));
    ASSERT_FALSE(App_CanAlerts_GetFault(BMS_FAULT_CELL_UNDERTEMP));
}

TEST_F(BmsFaultTest, check_state_transition_to_fault_state_from_all_states_ts_discharge_overcurrent)
{
    // Set TS current negative to trigger discharging condition
    get_high_res_current_fake.return_val = -10.0f;
    get_low_res_current_fake.return_val  = -10.0f;

    SetInitialState(App_GetInitState());

    // Let accumulator startup count expire
    LetTimePass(state_machine, 1000);
    ASSERT_EQ(App_GetInitState(), App_SharedStateMachine_GetCurrentState(state_machine));
    ASSERT_FALSE(App_CanAlerts_GetFault(BMS_FAULT_TS_OVERCURRENT));

    // Max acceptable discharge current is -88.5A*3 = -265.5A
    get_high_res_current_fake.return_val = MAX_TS_DISCHARGE_CURRENT_AMPS - 1.0f;
    get_low_res_current_fake.return_val  = MAX_TS_DISCHARGE_CURRENT_AMPS - 1.0f;
    LetTimePass(state_machine, 10);
    ASSERT_EQ(App_GetFaultState(), App_SharedStateMachine_GetCurrentState(state_machine));
    ASSERT_TRUE(App_CanAlerts_GetFault(BMS_FAULT_TS_OVERCURRENT));

    // Confirm stays in fault indefinitely
    LetTimePass(state_machine, 1000);
    ASSERT_EQ(App_GetFaultState(), App_SharedStateMachine_GetCurrentState(state_machine));
    ASSERT_TRUE(App_CanAlerts_GetFault(BMS_FAULT_TS_OVERCURRENT));

    // Clear fault, should transition back to init
    get_high_res_current_fake.return_val = MAX_TS_DISCHARGE_CURRENT_AMPS + 1.0f;
    get_low_res_current_fake.return_val  = MAX_TS_DISCHARGE_CURRENT_AMPS + 1.0f;
    LetTimePass(state_machine, 10);
    ASSERT_EQ(App_GetInitState(), App_SharedStateMachine_GetCurrentState(state_machine));
    ASSERT_FALSE(App_CanAlerts_GetFault(BMS_FAULT_TS_OVERCURRENT));
}

TEST_F(BmsFaultTest, check_state_transition_to_fault_state_from_all_states_ts_charge_overcurrent)
{
    // Set charger conditions such that charger faults do not trigger
    is_charger_connected_fake.return_val   = true;
    is_air_negative_closed_fake.return_val = true;
    has_charger_faulted_fake.return_val    = false;
    App_CanRx_Debug_ChargingSwitch_StartCharging_Update(true);

    // Set TS current above cutoff threshold to keep state machine in charge state
    get_high_res_current_fake.return_val = 10.0f;
    get_low_res_current_fake.return_val  = 10.0f;

    SetInitialState(App_GetChargeState());

    // Let accumulator startup count expire
    LetTimePass(state_machine, 1000);
    ASSERT_EQ(App_GetChargeState(), App_SharedStateMachine_GetCurrentState(state_machine));
    ASSERT_FALSE(App_CanAlerts_GetFault(BMS_FAULT_TS_OVERCURRENT));

    // Max acceptable charge current is 23.6A * 3 = 70.8A
    get_high_res_current_fake.return_val = MAX_TS_CHARGE_CURRENT_AMPS + 1.0f;
    get_low_res_current_fake.return_val  = MAX_TS_CHARGE_CURRENT_AMPS + 1.0f;
    LetTimePass(state_machine, 10);
    ASSERT_EQ(App_GetFaultState(), App_SharedStateMachine_GetCurrentState(state_machine));
    ASSERT_TRUE(App_CanAlerts_GetFault(BMS_FAULT_TS_OVERCURRENT));

    // Confirm stays in fault indefinitely
    LetTimePass(state_machine, 1000);
    ASSERT_EQ(App_GetFaultState(), App_SharedStateMachine_GetCurrentState(state_machine));
    ASSERT_TRUE(App_CanAlerts_GetFault(BMS_FAULT_TS_OVERCURRENT));

    // Clear fault, should transition back to init
    get_high_res_current_fake.return_val   = MAX_TS_CHARGE_CURRENT_AMPS - 1.0f;
    get_low_res_current_fake.return_val    = MAX_TS_CHARGE_CURRENT_AMPS - 1.0f;
    is_air_negative_closed_fake.return_val = false; // Negative contactor has to open to go back to init
    LetTimePass(state_machine, 10);
    ASSERT_EQ(App_GetInitState(), App_SharedStateMachine_GetCurrentState(state_machine));
    ASSERT_FALSE(App_CanAlerts_GetFault(BMS_FAULT_TS_OVERCURRENT));
}

TEST_F(BmsFaultTest, check_precharge_fault_combinations)
{
    // Possible combinations of is_charger_connected, is_ts_rising_slowly and is_ts_rising_quickly
    bool inputs[10][4] = { { false, false, false, false }, { false, true, false, false }, { false, false, true, false },
                           { false, true, true, false },   { true, false, false, false }, { true, true, false, false },
                           { true, false, true, false },   { true, true, true, false },   { false, false, false, true },
                           { true, false, false, true } };
    // Possible outputs based on above combinations of inputs
    bool expected_output[10]            = { false, true, true, true, false, true, false, true, true, true };
    bool precharge_fault_limit_exceeded = false;

    for (int i = 0; i < 10; i++)
    {
        ASSERT_EQ(
            expected_output[i], App_PrechargeRelay_CheckFaults(
                                    precharge_relay, inputs[i][0], inputs[i][1], inputs[i][2], inputs[i][3],
                                    &precharge_fault_limit_exceeded));

        const bool precharge_limit_geq_3 = App_PrechargeRelay_GetFaultCounterVal(precharge_relay) >= 3;
        ASSERT_EQ(precharge_fault_limit_exceeded, precharge_limit_geq_3);
        ASSERT_EQ(App_CanAlerts_GetFault(BMS_FAULT_PRECHARGE_ERROR), precharge_limit_geq_3);
    }
    // Check to see if counter is non-zero before resetting
    ASSERT_NE(App_PrechargeRelay_GetFaultCounterVal(precharge_relay), 0);

    // counter should be zero after reset
    App_PrechargeRelay_ResetFaultCounterVal(precharge_relay);
    ASSERT_EQ(App_PrechargeRelay_GetFaultCounterVal(precharge_relay), 0);
}

TEST_F(BmsFaultTest, check_state_transition_fault_state_precharge_fault)
{
    SetInitialState(App_GetInitState());

    // reset ts_voltage to 0 so state will transition from init to pre-charge
    get_ts_voltage_fake.return_val = 0;

    for (int i = 1; i <= 3; i++)
    {
        // Close negative contactor with charger disconnected, precharge should start
        is_air_negative_closed_fake.return_val = true;
        is_charger_connected_fake.return_val   = false;
        App_CanRx_Debug_ChargingSwitch_StartCharging_Update(false);
        LetTimePass(state_machine, 10U);
        ASSERT_EQ(App_GetPreChargeState(), App_SharedStateMachine_GetCurrentState(state_machine));
        ASSERT_FALSE(App_CanAlerts_GetFault(BMS_FAULT_PRECHARGE_ERROR));

        // 3.8V nominal cell voltage * total # of cells to give estimate of nominal pack voltage
        // trying to fool precahrge into thinking that ts_voltage is rising too quickly
        get_ts_voltage_fake.return_val = 3.8f * ACCUMULATOR_NUM_SERIES_CELLS_TOTAL;
        LetTimePass(state_machine, 10U);

        if (i < 3)
        {
            // 3x precharge attempts haven't been exceeded, so back to init
            ASSERT_EQ(App_GetInitState(), App_SharedStateMachine_GetCurrentState(state_machine));
            ASSERT_FALSE(App_CanAlerts_GetFault(BMS_FAULT_PRECHARGE_ERROR));

            // reset ts_voltage to 0 so state will transition from init to pre-charge
            get_ts_voltage_fake.return_val = 0;
        }
        else
        {
            // 3x precharge attempts have failed, so back transition to fault state indefinitely
            ASSERT_EQ(App_GetFaultState(), App_SharedStateMachine_GetCurrentState(state_machine));
            ASSERT_TRUE(App_CanAlerts_GetFault(BMS_FAULT_PRECHARGE_ERROR));
            LetTimePass(state_machine, 1000U);
            ASSERT_EQ(App_GetFaultState(), App_SharedStateMachine_GetCurrentState(state_machine));
            ASSERT_TRUE(App_CanAlerts_GetFault(BMS_FAULT_PRECHARGE_ERROR));

            // Can't transition out of fault state due to precharge failure!
        }
    }
}

TEST_F(BmsFaultTest, check_state_transition_fault_state_heartbeat_timeout)
{
    struct
    {
        bool fsm_mia;
        bool dcm_mia;
        bool pdm_mia;
    } test_params[3] = {
        { true, false, false },
        { false, true, false },
        { false, false, true },
    };

    for (int i = 0; i < 3; i++)
    {
        TearDown();
        SetUp();

        // Check in all heartbeats within timeout period
        get_current_ms_fake.return_val += HEARTBEAT_MONITOR_TIMEOUT_PERIOD_MS - 10U;
        LetTimePass(state_machine, HEARTBEAT_MONITOR_TIMEOUT_PERIOD_MS - 10U);
        ASSERT_EQ(App_GetInitState(), App_SharedStateMachine_GetCurrentState(state_machine));
        ASSERT_FALSE(App_CanAlerts_GetFault(BMS_FAULT_MISSING_HEARTBEAT));

        App_CanRx_FSM_Vitals_Heartbeat_Update(true);
        App_CanRx_DCM_Vitals_Heartbeat_Update(true);
        App_CanRx_PDM_Vitals_Heartbeat_Update(true);

        get_current_ms_fake.return_val += 10;
        LetTimePass(state_machine, 10);
        ASSERT_EQ(App_GetInitState(), App_SharedStateMachine_GetCurrentState(state_machine));
        ASSERT_FALSE(App_CanAlerts_GetFault(BMS_FAULT_MISSING_HEARTBEAT));

        // Fail to check in a heartbeat, BMS should fault
        get_current_ms_fake.return_val += HEARTBEAT_MONITOR_TIMEOUT_PERIOD_MS - 10U;
        LetTimePass(state_machine, HEARTBEAT_MONITOR_TIMEOUT_PERIOD_MS - 10U);
        ASSERT_EQ(App_GetInitState(), App_SharedStateMachine_GetCurrentState(state_machine));
        ASSERT_FALSE(App_CanAlerts_GetFault(BMS_FAULT_MISSING_HEARTBEAT));

        App_CanRx_FSM_Vitals_Heartbeat_Update(!test_params[i].fsm_mia);
        App_CanRx_DCM_Vitals_Heartbeat_Update(!test_params[i].dcm_mia);
        App_CanRx_PDM_Vitals_Heartbeat_Update(!test_params[i].pdm_mia);

        get_current_ms_fake.return_val += 10;
        LetTimePass(state_machine, 10);
        ASSERT_EQ(App_GetFaultState(), App_SharedStateMachine_GetCurrentState(state_machine));
        ASSERT_TRUE(App_CanAlerts_GetFault(BMS_FAULT_MISSING_HEARTBEAT));

        // Stay faulted indefinitely
        get_current_ms_fake.return_val += 1000;
        LetTimePass(state_machine, 1000);
        ASSERT_EQ(App_GetFaultState(), App_SharedStateMachine_GetCurrentState(state_machine));
        ASSERT_TRUE(App_CanAlerts_GetFault(BMS_FAULT_MISSING_HEARTBEAT));

        // Check heartbeats back in, fault should clear and transition back to init
        App_CanRx_FSM_Vitals_Heartbeat_Update(true);
        App_CanRx_DCM_Vitals_Heartbeat_Update(true);
        App_CanRx_PDM_Vitals_Heartbeat_Update(true);

        get_current_ms_fake.return_val += HEARTBEAT_MONITOR_TIMEOUT_PERIOD_MS;
        LetTimePass(state_machine, HEARTBEAT_MONITOR_TIMEOUT_PERIOD_MS);
        ASSERT_EQ(App_GetInitState(), App_SharedStateMachine_GetCurrentState(state_machine));
        ASSERT_FALSE(App_CanAlerts_GetFault(BMS_FAULT_MISSING_HEARTBEAT));
    }
}

TEST_F(BmsFaultTest, check_state_transition_fault_state_from_fault_over_can)
{
    void (*update_rx_fault[4])(bool) = {
        App_CanRx_DCM_Faults_DCM_FAULT_STATE_FAULT_Update,
        App_CanRx_FSM_Faults_FSM_FAULT_STATE_FAULT_Update,
        App_CanRx_PDM_Faults_PDM_FAULT_DUMMY_Update,
        App_CanRx_DIM_Faults_DIM_FAULT_MISSING_HEARTBEAT_Update,
    };

    for (int i = 0; i < 4; i++)
    {
        // Reset test
        TearDown();
        SetUp();

        // Let accumulator startup count expire
        LetTimePass(state_machine, 1000);
        ASSERT_EQ(App_GetInitState(), App_SharedStateMachine_GetCurrentState(state_machine));
        ASSERT_FALSE(App_CanAlerts_GetFault(BMS_FAULT_CELL_OVERVOLTAGE));

        // Set received fault
        update_rx_fault[i](true);
        LetTimePass(state_machine, 20);
        ASSERT_EQ(App_GetFaultState(), App_SharedStateMachine_GetCurrentState(state_machine));

        // Confirm stays in fault indefinitely
        LetTimePass(state_machine, 1000);
        ASSERT_EQ(App_GetFaultState(), App_SharedStateMachine_GetCurrentState(state_machine));

        // Clear fault, should transition back to init
        update_rx_fault[i](false);
        LetTimePass(state_machine, 10);
        ASSERT_EQ(App_GetInitState(), App_SharedStateMachine_GetCurrentState(state_machine));
    }
}

TEST_F(BmsFaultTest, check_state_transition_to_fault_disables_bms_ok)
{
    // Let accumulator startup count expire
    LetTimePass(state_machine, 1000);
    ASSERT_EQ(App_GetInitState(), App_SharedStateMachine_GetCurrentState(state_machine));
    ASSERT_EQ(enable_bms_ok_fake.call_count, 0);
    ASSERT_EQ(disable_bms_ok_fake.call_count, 0);

    // Set cell voltage critically high and confirm fault is set
    set_cell_voltage((AccumulatorSegment)0U, 0U, MAX_CELL_VOLTAGE + 0.1f);
    LetTimePass(state_machine, 20);
    ASSERT_EQ(App_GetFaultState(), App_SharedStateMachine_GetCurrentState(state_machine));

    // BMS OK status should be disabled upon entering fault state, which is latched in hardware
    ASSERT_EQ(enable_bms_ok_fake.call_count, 0);
    ASSERT_EQ(disable_bms_ok_fake.call_count, 1);
}

} // namespace FaultTest
