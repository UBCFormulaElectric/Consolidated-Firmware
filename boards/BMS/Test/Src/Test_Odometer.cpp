#include "Test_StateMachine.h"

#define STARTING_ODOMETER_READING 1000.0f
#define FLOATS_PER_PAGE 4

namespace OdometerTest
{
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
FAKE_VALUE_FUNC(ExitCode, read_die_temperatures);
FAKE_VALUE_FUNC(bool, is_air_negative_on);
FAKE_VALUE_FUNC(bool, is_air_positive_on);
FAKE_VOID_FUNC(open_air_positive);
FAKE_VOID_FUNC(close_air_positive);
FAKE_VOID_FUNC(enable_pre_charge);
FAKE_VOID_FUNC(disable_pre_charge);
FAKE_VALUE_FUNC(bool, configure_cell_monitors);
FAKE_VALUE_FUNC(bool, write_cfg_registers);
FAKE_VALUE_FUNC(bool, start_voltage_conv);
FAKE_VALUE_FUNC(float, get_min_cell_voltage, uint8_t *, uint8_t *);
FAKE_VALUE_FUNC(float, get_max_cell_voltage, uint8_t *, uint8_t *);
FAKE_VALUE_FUNC(float, get_segment_voltage, AccumulatorSegment);
FAKE_VALUE_FUNC(float, get_pack_voltage);
FAKE_VALUE_FUNC(float, get_avg_cell_voltage);
FAKE_VALUE_FUNC(float, get_ts_voltage);
FAKE_VALUE_FUNC(float, get_low_res_current);
FAKE_VALUE_FUNC(float, get_high_res_current);
FAKE_VALUE_FUNC(bool, start_temp_conv);
FAKE_VALUE_FUNC(bool, read_cell_temperatures);
FAKE_VALUE_FUNC(float, get_min_temp_degc, uint8_t *, uint8_t *);
FAKE_VALUE_FUNC(float, get_max_temp_degc, uint8_t *, uint8_t *);
FAKE_VALUE_FUNC(float, get_avg_temp_degc);
FAKE_VALUE_FUNC(bool, enable_discharge);
FAKE_VALUE_FUNC(bool, disable_discharge);
FAKE_VALUE_FUNC(EEPROM_StatusTypeDef, read_page, uint16_t, uint8_t, uint8_t *, uint16_t);
FAKE_VALUE_FUNC(EEPROM_StatusTypeDef, write_page, uint16_t, uint8_t, uint8_t *, uint16_t);
FAKE_VALUE_FUNC(EEPROM_StatusTypeDef, page_erase, uint16_t);

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

static uint8_t static_byte_array[PAGE_SIZE];

// callback stores byte_array input into Io_Eeprom_WriteByte in static_byte_array to mimic writing to memory
static EEPROM_StatusTypeDef write_byte_callback(uint16_t page, uint8_t offset, uint8_t *byte_arr, uint16_t size)
{
    for (int i = 0; i < size; i++)
    {
        static_byte_array[i] = byte_arr[i];
    }
    return EEPROM_OK;
}

// callback stores copies stored static_byte_array into array pointed to in argument list of Io_Eeprom_ReadByte
// to mimic reading from memory
static EEPROM_StatusTypeDef read_byte_callback(uint16_t page, uint8_t offset, uint8_t *byte_arr, uint16_t size)
{
    for (int i = 0; i < size; i++)
    {
        byte_arr[i] = static_byte_array[i];
    }
    return EEPROM_OK;
}

class BmsOdometerTest : public BaseStateMachineTest
{
  protected:
    void SetUp() override
    {
        BaseStateMachineTest::SetUp();

        RESET_FAKE(read_page);
        RESET_FAKE(write_page);
        RESET_FAKE(page_erase);

        write_page_fake.custom_fake = write_byte_callback;
        read_page_fake.custom_fake  = read_byte_callback;

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
            read_cell_temperatures, get_min_temp_degc, get_max_temp_degc, get_avg_temp_degc, enable_discharge,
            disable_discharge);

        precharge_relay = App_PrechargeRelay_Create(enable_pre_charge, disable_pre_charge);

        ts = App_TractiveSystem_Create(get_ts_voltage, get_high_res_current, get_low_res_current);

        airs = App_Airs_Create(is_air_positive_closed, is_air_negative_closed, close_air_positive, open_air_positive);

        clock = App_SharedClock_Create();

        eeprom = App_Eeprom_Create(write_page, read_page, page_erase);

        odometer = App_Odometer_Create();

        world = App_BmsWorld_Create(
            imd, heartbeat_monitor, rgb_led_sequence, charger, bms_ok, imd_ok, bspd_ok, accumulator, airs,
            precharge_relay, ts, clock, eeprom, odometer);

        // Default to starting the state machine in the `init` state
        state_machine = App_SharedStateMachine_Create(world, App_GetInitState());
        App_AllStates_Init();

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
        RESET_FAKE(close_air_positive);
        RESET_FAKE(configure_cell_monitors);
        RESET_FAKE(write_cfg_registers);
        RESET_FAKE(start_voltage_conv);
        RESET_FAKE(get_min_cell_voltage);
        RESET_FAKE(get_max_cell_voltage);
        RESET_FAKE(get_segment_voltage);
        RESET_FAKE(get_pack_voltage);
        RESET_FAKE(get_avg_cell_voltage);
        RESET_FAKE(get_min_cell_voltage);
        RESET_FAKE(get_max_cell_voltage);
        RESET_FAKE(get_avg_cell_voltage);
        RESET_FAKE(get_min_cell_voltage);
        RESET_FAKE(get_max_cell_voltage);
        RESET_FAKE(get_low_res_current);
        RESET_FAKE(get_high_res_current);

        // Set initial voltages to nominal value
        set_all_cell_voltages(3.8);
        start_voltage_conv_fake.return_val = true;

        // A voltage in [3.0, 4.2] was arbitrarily chosen to prevent other
        // tests from entering the fault state
        get_min_cell_voltage_fake.return_val = 4.0f;
        get_max_cell_voltage_fake.return_val = 4.0f;

        // A temperature in [0.0, 60.0] degC to prevent other tests from entering the fault state
        get_min_temp_degc_fake.return_val = 20.0f;
        get_max_temp_degc_fake.return_val = 20.0f;

        read_page_fake.return_val  = EEPROM_OK;
        write_page_fake.return_val = EEPROM_OK;
        page_erase_fake.return_val = EEPROM_OK;

        is_air_negative_closed_fake.return_val = true;
        get_ts_voltage_fake.return_val         = 1;

        // Disable charging
        is_charger_connected_fake.return_val = false;
        App_CanRx_Debug_ChargingSwitch_StartCharging_Update(false);
        has_charger_faulted_fake.return_val = false;

        App_Eeprom_WriteErrCheckedFloat(eeprom, ODOMETER_ADDRESS, STARTING_ODOMETER_READING);
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
        TearDownObject(airs, App_Airs_Destroy);
        TearDownObject(precharge_relay, App_PrechargeRelay_Destroy);
        TearDownObject(ts, App_TractiveSystem_Destroy);
        TearDownObject(clock, App_SharedClock_Destroy);
        TearDownObject(eeprom, App_Eeprom_Destroy);
        TearDownObject(odometer, App_Odometer_Destroy);
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

    void CheckInRangeCanSignalsInGivenState(
        const State *state,
        float        min_value,
        float        max_value,
        float &      fake_value,
        float (*value_can_signal_getter)(const struct BmsCanTxInterface *),
        uint8_t (*out_of_range_can_signal_getter)(const struct BmsCanTxInterface *),
        uint8_t ok_choice,
        uint8_t underflow_choice,
        uint8_t overflow_choice)
    {
        SetInitialState(state);

        // Normal range
        fake_value = (min_value + max_value) / 2;
        LetTimePass(state_machine, 1000);
        ASSERT_EQ(fake_value, value_can_signal_getter(can_tx_interface));
        ASSERT_EQ(ok_choice, out_of_range_can_signal_getter(can_tx_interface));

        // Underflow range
        fake_value = std::nextafter(min_value, std::numeric_limits<float>::lowest());
        LetTimePass(state_machine, 1000);
        ASSERT_EQ(underflow_choice, out_of_range_can_signal_getter(can_tx_interface));

        // Overflow range
        fake_value = std::nextafter(max_value, std::numeric_limits<float>::max());
        LetTimePass(state_machine, 1000);
        ASSERT_EQ(overflow_choice, out_of_range_can_signal_getter(can_tx_interface));
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
    struct Airs *             airs;
    struct PrechargeRelay *   precharge_relay;
    struct TractiveSystem *   ts;
    struct Clock *            clock;
    struct Eeprom *           eeprom;
    struct Odometer *         odometer;
};

TEST_F(BmsOdometerTest, test_odometer_val_read_from_eeprom)
{
    SetInitialState(App_GetInitState());

    // Let accumulator startup count expire
    LetTimePass(state_machine, 1000);

    ASSERT_EQ(App_Odometer_GetReading(odometer), STARTING_ODOMETER_READING);
}

TEST_F(BmsOdometerTest, test_odometer_val_increases)
{
    float wheelspeed = 10.0f;

    App_CanRx_INVR_MotorPositionInfo_MotorSpeed_Update(wheelspeed);
    App_CanRx_INVL_MotorPositionInfo_MotorAngle_Update(wheelspeed);

    SetInitialState(App_GetInitState());

    LetTimePass(state_machine, 100);

    ASSERT_EQ(App_Odometer_GetReading(odometer), STARTING_ODOMETER_READING);

    App_SharedStateMachine_SetNextState(state_machine, App_GetDriveState());

    LetTimePass(state_machine, 100);

    ASSERT_GT(App_Odometer_GetReading(odometer), STARTING_ODOMETER_READING);
}

TEST_F(BmsOdometerTest, test_odometer_val_constant_with_negative_wheelspin)
{
    float wheelspeed = -10.0f;

    App_CanRx_INVR_MotorPositionInfo_MotorSpeed_Update(wheelspeed);
    App_CanRx_INVL_MotorPositionInfo_MotorAngle_Update(wheelspeed);

    SetInitialState(App_GetInitState());

    LetTimePass(state_machine, 100);

    ASSERT_EQ(App_Odometer_GetReading(odometer), STARTING_ODOMETER_READING);

    App_SharedStateMachine_SetNextState(state_machine, App_GetDriveState());

    LetTimePass(state_machine, 100);

    ASSERT_EQ(App_Odometer_GetReading(odometer), STARTING_ODOMETER_READING);
}

TEST_F(BmsOdometerTest, test_odometer_val_resets_with_can_message)
{
    float wheelspeed = 10.0f;

    App_CanRx_INVR_MotorPositionInfo_MotorSpeed_Update(wheelspeed);
    App_CanRx_INVL_MotorPositionInfo_MotorAngle_Update(wheelspeed);

    SetInitialState(App_GetInitState());

    LetTimePass(state_machine, 100);

    ASSERT_EQ(App_Odometer_GetReading(odometer), STARTING_ODOMETER_READING);

    App_SharedStateMachine_SetNextState(state_machine, App_GetDriveState());

    LetTimePass(state_machine, 100);

    ASSERT_GT(App_Odometer_GetReading(odometer), STARTING_ODOMETER_READING);

    App_CanRx_Debug_ResetOdometer_ResetOdometer_Update(true);

    wheelspeed = 0.0f;

    App_CanRx_INVR_MotorPositionInfo_MotorSpeed_Update(wheelspeed);
    App_CanRx_INVL_MotorPositionInfo_MotorAngle_Update(wheelspeed);

    LetTimePass(state_machine, 1000);

    ASSERT_EQ(App_Odometer_GetReading(odometer), 0.0f);
}

TEST_F(BmsOdometerTest, test_odometer_val_written_to_eeprom)
{
    float wheelspeed      = 10.0f;
    int   time_to_wait_ms = 31000; // 30s write interval + 1s to give time for write

    App_CanRx_INVR_MotorPositionInfo_MotorSpeed_Update(wheelspeed);
    App_CanRx_INVL_MotorPositionInfo_MotorAngle_Update(wheelspeed);

    SetInitialState(App_GetInitState());

    LetTimePass(state_machine, 100);

    ASSERT_EQ(App_Odometer_GetReading(odometer), STARTING_ODOMETER_READING);

    App_SharedStateMachine_SetNextState(state_machine, App_GetDriveState());

    LetTimePass(state_machine, 100);

    ASSERT_GT(App_Odometer_GetReading(odometer), STARTING_ODOMETER_READING);

    LetTimePass(state_machine, time_to_wait_ms);

    ASSERT_GT(App_Odometer_ReadValFromEeprom(eeprom, ODOMETER_ADDRESS), STARTING_ODOMETER_READING);
}

} // namespace OdometerTest
