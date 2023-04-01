
#include <math.h>
#include "Test_Pdm.h"
#include "Test_BaseStateMachineTest.h"
#include "states/App_DriveState.h"
#include "states/App_FaultState.h"

extern "C"
{
#include "App_SharedHeartbeatMonitor.h"
#include "App_SharedStateMachine.h"
#include "App_SharedMacros.h"
#include "states/App_InitState.h"
#include "states/App_DriveState.h"
#include "states/App_FaultState.h"
#include "configs/App_VoltageLimits.h"
#include "configs/App_CurrentLimits.h"
#include "configs/App_HeartbeatMonitorConfig.h"
}

namespace StateMachineTest
{
FAKE_VOID_FUNC(send_non_periodic_msg_PDM_STARTUP, const struct CanMsgs_pdm_startup_t *);
FAKE_VOID_FUNC(send_non_periodic_msg_PDM_AIR_SHUTDOWN, const struct CanMsgs_pdm_air_shutdown_t *);
FAKE_VOID_FUNC(send_non_periodic_msg_PDM_MOTOR_SHUTDOWN, const struct CanMsgs_pdm_motor_shutdown_t *);
FAKE_VOID_FUNC(send_non_periodic_msg_PDM_WATCHDOG_TIMEOUT, const struct CanMsgs_pdm_watchdog_timeout_t *);

FAKE_VALUE_FUNC(float, GetVbatVoltage);
FAKE_VALUE_FUNC(float, Get22vAuxVoltage);
FAKE_VALUE_FUNC(float, Get24vAccVoltage);

FAKE_VOID_FUNC(EfuseSetChannelPointer, EfuseChannel, bool);
FAKE_VALUE_FUNC(bool, EfuseIsChannelEnabledPointer, EfuseChannel);
FAKE_VALUE_FUNC(float, EfuseGetChannelCurrentPointer, EfuseChannel);
FAKE_VOID_FUNC(EfuseStandbyResetPointer, EfuseChannel);

FAKE_VALUE_FUNC(int, Efuse1MaxAttempts);
FAKE_VALUE_FUNC(int, Efuse2MaxAttempts);
FAKE_VALUE_FUNC(int, Efuse3MaxAttempts);
FAKE_VALUE_FUNC(int, Efuse4MaxAttempts);

FAKE_VALUE_FUNC(uint32_t, get_current_ms);
FAKE_VOID_FUNC(heartbeat_timeout_callback, enum HeartbeatOneHot, enum HeartbeatOneHot);

FAKE_VOID_FUNC(turn_on_red_led);
FAKE_VOID_FUNC(turn_on_green_led);
FAKE_VOID_FUNC(turn_on_blue_led);

FAKE_VALUE_FUNC(bool, do_low_voltage_battery_have_charge_fault);
FAKE_VALUE_FUNC(bool, do_low_voltage_battery_have_boost_controller_fault);

class PdmStateMachineTest : public BaseStateMachineTest
{
  protected:
    void SetUp() override
    {
        BaseStateMachineTest::SetUp();

        heartbeat_monitor = App_SharedHeartbeatMonitor_Create(
            get_current_ms, HEARTBEAT_MONITOR_TIMEOUT_PERIOD_MS, HEARTBEAT_MONITOR_BOARDS_TO_CHECK);

        rgb_led_sequence = App_SharedRgbLedSequence_Create(turn_on_red_led, turn_on_green_led, turn_on_blue_led);

        low_voltage_battery = App_LowVoltageBattery_Create(
            do_low_voltage_battery_have_charge_fault, do_low_voltage_battery_have_boost_controller_fault);

        clock = App_SharedClock_Create();

        state_machine = App_SharedStateMachine_Create(world, App_GetInitState());

        efuse1 = App_Efuse_Create(
            EFUSE_CHANNEL_AIR, EFUSE_CHANNEL_LVPWR, EfuseSetChannelPointer, EfuseIsChannelEnabledPointer,
            EfuseGetChannelCurrentPointer, EfuseStandbyResetPointer, EFUSE1_AIR_MIN_CURRENT, EFUSE1_AIR_MAX_CURRENT,
            EFUSE1_LV_POWER_MIN_CURRENT, EFUSE1_LV_POWER_MAX_CURRENT, Efuse1MaxAttempts());
        efuse2 = App_Efuse_Create(
            EFUSE_CHANNEL_EMETER, EFUSE_CHANNEL_AUX, EfuseSetChannelPointer, EfuseIsChannelEnabledPointer,
            EfuseGetChannelCurrentPointer, EfuseStandbyResetPointer, EFUSE2_AUX_MIN_CURRENT, EFUSE2_AUX_MAX_CURRENT,
            EFUSE2_EMETER_MIN_CURRENT, EFUSE2_AUX_MAX_CURRENT, Efuse2MaxAttempts());
        efuse3 = App_Efuse_Create(
            EFUSE_CHANNEL_DI_LHS, EFUSE_CHANNEL_DI_RHS, EfuseSetChannelPointer, EfuseIsChannelEnabledPointer,
            EfuseGetChannelCurrentPointer, EfuseStandbyResetPointer, EFUSE3_LEFT_INVERTER_MIN_CURRENT,
            EFUSE3_LEFT_INVERTER_MAX_CURRENT, EFUSE3_RIGHT_INVERTER_MIN_CURRENT, EFUSE3_RIGHT_INVERTER_MAX_CURRENT,
            Efuse3MaxAttempts());
        efuse4 = App_Efuse_Create(
            EFUSE_CHANNEL_DRS, EFUSE_CHANNEL_FAN, EfuseSetChannelPointer, EfuseIsChannelEnabledPointer,
            EfuseGetChannelCurrentPointer, EfuseStandbyResetPointer, EFUSE4_DRS_MIN_CURRENT, EFUSE4_DRS_MAX_CURRENT,
            EFUSE4_FAN_MIN_CURRENT, EFUSE4_FAN_MAX_CURRENT, Efuse4MaxAttempts());

        rail_monitor = App_RailMonitoring_Create(GetVbatVoltage, Get24vAccVoltage, Get22vAuxVoltage);

        world = App_PdmWorld_Create(
            heartbeat_monitor, rgb_led_sequence, low_voltage_battery, clock, efuse1, efuse2, efuse3, efuse4,
            rail_monitor);

        // Default to starting the state machine in the `init` state
        state_machine = App_SharedStateMachine_Create(world, App_GetInitState());

        // Reset fake functions
        RESET_FAKE(send_non_periodic_msg_PDM_STARTUP);
        RESET_FAKE(send_non_periodic_msg_PDM_AIR_SHUTDOWN);
        RESET_FAKE(send_non_periodic_msg_PDM_MOTOR_SHUTDOWN);
        RESET_FAKE(send_non_periodic_msg_PDM_WATCHDOG_TIMEOUT);
        RESET_FAKE(GetVbatVoltage);
        RESET_FAKE(Get22vAuxVoltage);
        RESET_FAKE(Get24vAccVoltage);
        RESET_FAKE(get_current_ms);
        RESET_FAKE(heartbeat_timeout_callback);
        RESET_FAKE(turn_on_red_led);
        RESET_FAKE(turn_on_green_led);
        RESET_FAKE(turn_on_blue_led);
        RESET_FAKE(do_low_voltage_battery_have_charge_fault);
        RESET_FAKE(do_low_voltage_battery_have_boost_controller_fault);
        RESET_FAKE(Efuse1MaxAttempts);
        RESET_FAKE(Efuse2MaxAttempts);
        RESET_FAKE(Efuse3MaxAttempts);
        RESET_FAKE(Efuse4MaxAttempts);
        RESET_FAKE(EfuseSetChannelPointer);
        RESET_FAKE(EfuseIsChannelEnabledPointer);
        RESET_FAKE(EfuseGetChannelCurrentPointer);
        RESET_FAKE(EfuseStandbyResetPointer);
    }

    void TearDown() override
    {
        TearDownObject(world, App_PdmWorld_Destroy);
        TearDownObject(heartbeat_monitor, App_SharedHeartbeatMonitor_Destroy);
        TearDownObject(rgb_led_sequence, App_SharedRgbLedSequence_Destroy);
        TearDownObject(state_machine, App_SharedStateMachine_Destroy);
        TearDownObject(low_voltage_battery, App_LowVoltageBattery_Destroy);
        TearDownObject(clock, App_SharedClock_Destroy);
        TearDownObject(efuse1, App_Efuse_Destroy);
        TearDownObject(efuse2, App_Efuse_Destroy);
        TearDownObject(efuse3, App_Efuse_Destroy);
        TearDownObject(efuse4, App_Efuse_Destroy);
        TearDownObject(rail_monitor, App_RailMonitoring_Destroy);
    }

    void SetInitialState(const struct State *const initial_state)
    {
        TearDownObject(state_machine, App_SharedStateMachine_Destroy);
        state_machine = App_SharedStateMachine_Create(world, initial_state);
        ASSERT_EQ(initial_state, App_SharedStateMachine_GetCurrentState(state_machine));
    }

    std::vector<const struct State *> GetAllStates(void)
    {
        return std::vector<const struct State *>{ App_GetInitState(), App_GetDriveState(), App_GetFaultState() };
    }

    std::vector<const struct State *> GetAllStatesExceptForInit(void)
    {
        std::vector<const struct State *> vec = GetAllStates();

        vec.erase(std::remove(vec.begin(), vec.end(), App_GetInitState()), vec.end());

        return vec;
    }

    void CheckCurrentCanSignalsInNonInitStates(
        float  min_current,
        float  max_current,
        float &fake_current,
        float (*current_can_signal_getter)(const struct PdmCanTxInterface *),
        uint8_t (*out_of_range_can_signal_getter)(const struct PdmCanTxInterface *),
        uint8_t ok_choice,
        uint8_t underflow_choice,
        uint8_t overflow_choice)
    {
        for (auto &state : GetAllStatesExceptForInit())
        {
            SetInitialState(state);

            // Normal Value
            fake_current = (min_current + max_current) / 2;
            LetTimePass(state_machine, 10);
            EXPECT_EQ(fake_current, current_can_signal_getter(can_tx_interface));
            EXPECT_EQ(ok_choice, out_of_range_can_signal_getter(can_tx_interface));

            // Under-current
            fake_current = std::nextafter(min_current, std::numeric_limits<float>::lowest());
            LetTimePass(state_machine, 10);
            EXPECT_EQ(underflow_choice, out_of_range_can_signal_getter(can_tx_interface));

            // Over-current
            fake_current = std::nextafter(max_current, std::numeric_limits<float>::max());
            LetTimePass(state_machine, 10);
            EXPECT_EQ(overflow_choice, out_of_range_can_signal_getter(can_tx_interface));
        }
    }

    void CheckCurrentCanSignalsInInitState(
        float  min_current,
        float  max_current,
        float &fake_current,
        float (*current_can_signal_getter)(const struct PdmCanTxInterface *),
        uint8_t (*out_of_range_can_signal_getter)(const struct PdmCanTxInterface *),
        uint8_t ok_choice)
    {
        // In the INIT state, the load switches aren't ready yet. Therefore, the
        // current readings reported by the IO layer are assumed to be garbage.
        // To get around this, we simply hard-code the CAN TX current signals
        // as NAN in the INIT state.

        SetInitialState(App_GetInitState());

        // Normal Value
        fake_current = (min_current + max_current) / 2;
        LetTimePass(state_machine, 10);
        EXPECT_TRUE(isnan(current_can_signal_getter(can_tx_interface)));
        EXPECT_EQ(ok_choice, out_of_range_can_signal_getter(can_tx_interface));

        // Under-current
        fake_current = std::nextafter(min_current, std::numeric_limits<float>::lowest());
        LetTimePass(state_machine, 10);
        EXPECT_TRUE(isnan(current_can_signal_getter(can_tx_interface)));
        EXPECT_EQ(ok_choice, out_of_range_can_signal_getter(can_tx_interface));

        // Over-current
        fake_current = std::nextafter(max_current, std::numeric_limits<float>::max());
        LetTimePass(state_machine, 10);
        EXPECT_TRUE(isnan(current_can_signal_getter(can_tx_interface)));
        EXPECT_EQ(ok_choice, out_of_range_can_signal_getter(can_tx_interface));
    }

    void CheckVoltageCanSignalsInAllStates(
        float  min_voltage,
        float  max_voltage,
        float &fake_voltage,
        float (*voltage_can_signal_getter)(const struct PdmCanTxInterface *),
        uint8_t (*out_of_range_can_signal_getter)(const struct PdmCanTxInterface *),
        uint8_t ok_choice,
        uint8_t underflow_choice,
        uint8_t overflow_choice)
    {
        for (auto &state : GetAllStates())
        {
            SetInitialState(state);

            // Normal Value
            fake_voltage = (min_voltage + max_voltage) / 2;
            LetTimePass(state_machine, 10);
            EXPECT_EQ(fake_voltage, voltage_can_signal_getter(can_tx_interface));
            EXPECT_EQ(ok_choice, out_of_range_can_signal_getter(can_tx_interface));

            // Under-voltage
            fake_voltage = std::nextafter(min_voltage, std::numeric_limits<float>::lowest());
            LetTimePass(state_machine, 10);
            EXPECT_EQ(underflow_choice, out_of_range_can_signal_getter(can_tx_interface));

            // Over-voltage
            fake_voltage = std::nextafter(max_voltage, std::numeric_limits<float>::max());
            LetTimePass(state_machine, 10);
            EXPECT_EQ(overflow_choice, out_of_range_can_signal_getter(can_tx_interface));
        }
    }

    void UpdateClock(struct StateMachine *state_machine, uint32_t current_time_ms) override
    {
        struct PdmWorld *world = App_SharedStateMachine_GetWorld(state_machine);
        struct Clock *   clock = App_PdmWorld_GetClock(world);
        App_SharedClock_SetCurrentTimeInMilliseconds(clock, current_time_ms);
    }

    void UpdateSignals(struct StateMachine *state_machine, uint32_t current_time_ms) override
    {
        // PDM doesn't use any signals currently
        UNUSED(state_machine);
        UNUSED(current_time_ms);
    }

    struct World *            world;
    struct StateMachine *     state_machine;
    struct PdmCanTxInterface *can_tx_interface;
    struct PdmCanRxInterface *can_rx_interface;
    struct HeartbeatMonitor * heartbeat_monitor;
    struct RgbLedSequence *   rgb_led_sequence;
    struct LowVoltageBattery *low_voltage_battery;
    struct Clock *            clock;
    struct Efuse *            efuse1;
    struct Efuse *            efuse2;
    struct Efuse *            efuse3;
    struct Efuse *            efuse4;
    struct RailMonitoring *   rail_monitor;
};

// PDM-21
// TEST_F(PdmStateMachineTest, check_init_state_is_broadcasted_over_can)
//{
//    SetInitialState(App_GetInitState());
//
//    EXPECT_EQ(CANMSGS_PDM_STATE_MACHINE_STATE_INIT_CHOICE, App_CanTx_GetPeriodicSignal_STATE(can_tx_interface));
//}
//
//// PDM-21
// TEST_F(PdmStateMachineTest, check_air_open_state_is_broadcasted_over_can)
//{
//    SetInitialState(App_GetInitState());
//
//    EXPECT_EQ(CANMSGS_PDM_STATE_MACHINE_STATE_AIR_OPEN_CHOICE, App_CanTx_GetPeriodicSignal_STATE(can_tx_interface));
//}
//
//// PDM-21
// TEST_F(PdmStateMachineTest, check_air_closed_state_is_broadcasted_over_can)
//{
//    SetInitialState(App_GetDriveState());
//
//    EXPECT_EQ(CANMSGS_PDM_STATE_MACHINE_STATE_AIR_CLOSED_CHOICE, App_CanTx_GetPeriodicSignal_STATE(can_tx_interface));
//}
//
//// PDM-6
// TEST_F(PdmStateMachineTest, check_vbat_voltage_can_signals_in_all_states)
//{
//    CheckVoltageCanSignalsInAllStates(
//        VBAT_MIN_VOLTAGE, VBAT_MAX_VOLTAGE, GetVbatVoltage_fake.return_val, App_CanTx_GetPeriodicSignal_VBAT,
//        App_CanTx_GetPeriodicSignal_VBAT_VOLTAGE_OUT_OF_RANGE,
//        CANMSGS_PDM_NON_CRITICAL_ERRORS_VBAT_VOLTAGE_OUT_OF_RANGE_OK_CHOICE,
//        CANMSGS_PDM_NON_CRITICAL_ERRORS_VBAT_VOLTAGE_OUT_OF_RANGE_UNDERFLOW_CHOICE,
//        CANMSGS_PDM_NON_CRITICAL_ERRORS_VBAT_VOLTAGE_OUT_OF_RANGE_OVERFLOW_CHOICE);
//}
//
//// PDM-6
// TEST_F(PdmStateMachineTest, check_24v_aux_voltage_can_signals_in_all_states)
//{
//    CheckVoltageCanSignalsInAllStates(
//        _24V_AUX_MIN_VOLTAGE, _24V_AUX_MAX_VOLTAGE, Get24vAuxVoltage_fake.return_val,
//        App_CanTx_GetPeriodicSignal__24_V_AUX, App_CanTx_GetPeriodicSignal__24_V_AUX_VOLTAGE_OUT_OF_RANGE,
//        CANMSGS_PDM_NON_CRITICAL_ERRORS__24_V_AUX_VOLTAGE_OUT_OF_RANGE_OK_CHOICE,
//        CANMSGS_PDM_NON_CRITICAL_ERRORS__24_V_AUX_VOLTAGE_OUT_OF_RANGE_UNDERFLOW_CHOICE,
//        CANMSGS_PDM_NON_CRITICAL_ERRORS__24_V_AUX_VOLTAGE_OUT_OF_RANGE_OVERFLOW_CHOICE);
//}
//
//// PDM-6
// TEST_F(PdmStateMachineTest, check_24v_acc_voltage_can_signals_in_all_states)
//{
//    CheckVoltageCanSignalsInAllStates(
//        _24V_ACC_MIN_VOLTAGE, _24V_ACC_MAX_VOLTAGE, Get24vAccVoltage_fake.return_val,
//        App_CanTx_GetPeriodicSignal__24_V_ACC, App_CanTx_GetPeriodicSignal__24_V_ACC_VOLTAGE_OUT_OF_RANGE,
//        CANMSGS_PDM_NON_CRITICAL_ERRORS__24_V_ACC_VOLTAGE_OUT_OF_RANGE_OK_CHOICE,
//        CANMSGS_PDM_NON_CRITICAL_ERRORS__24_V_ACC_VOLTAGE_OUT_OF_RANGE_UNDERFLOW_CHOICE,
//        CANMSGS_PDM_NON_CRITICAL_ERRORS__24_V_ACC_VOLTAGE_OUT_OF_RANGE_OVERFLOW_CHOICE);
//}
//
//// PDM-11, PDM-13
// TEST_F(PdmStateMachineTest, check_aux1_current_can_signals_in_non_init_states)
//{
//    CheckCurrentCanSignalsInNonInitStates(
//        AUX1_MIN_CURRENT, AUX1_MAX_CURRENT, GetAux1Current_fake.return_val,
//        App_CanTx_GetPeriodicSignal_AUXILIARY1_CURRENT, App_CanTx_GetPeriodicSignal_AUX1_CURRENT_OUT_OF_RANGE,
//        CANMSGS_PDM_NON_CRITICAL_ERRORS_AUX1_CURRENT_OUT_OF_RANGE_OK_CHOICE,
//        CANMSGS_PDM_NON_CRITICAL_ERRORS_AUX1_CURRENT_OUT_OF_RANGE_UNDERFLOW_CHOICE,
//        CANMSGS_PDM_NON_CRITICAL_ERRORS_AUX1_CURRENT_OUT_OF_RANGE_OVERFLOW_CHOICE);
//}
//
//// PDM-11, PDM-13
// TEST_F(PdmStateMachineTest, check_aux1_current_can_signals_in_init_state)
//{
//    CheckCurrentCanSignalsInInitState(
//        AUX1_MIN_CURRENT, AUX1_MAX_CURRENT, GetAux1Current_fake.return_val,
//        App_CanTx_GetPeriodicSignal_AUXILIARY1_CURRENT, App_CanTx_GetPeriodicSignal_AUX1_CURRENT_OUT_OF_RANGE,
//        CANMSGS_PDM_NON_CRITICAL_ERRORS_AUX1_CURRENT_OUT_OF_RANGE_OK_CHOICE);
//}
//
//// PDM-11, PDM-13
// TEST_F(PdmStateMachineTest, check_aux2_current_can_signals_in_non_init_states)
//{
//    CheckCurrentCanSignalsInNonInitStates(
//        AUX2_MIN_CURRENT, AUX2_MAX_CURRENT, GetAux2Current_fake.return_val,
//        App_CanTx_GetPeriodicSignal_AUXILIARY2_CURRENT, App_CanTx_GetPeriodicSignal_AUX2_CURRENT_OUT_OF_RANGE,
//        CANMSGS_PDM_NON_CRITICAL_ERRORS_AUX2_CURRENT_OUT_OF_RANGE_OK_CHOICE,
//        CANMSGS_PDM_NON_CRITICAL_ERRORS_AUX2_CURRENT_OUT_OF_RANGE_UNDERFLOW_CHOICE,
//        CANMSGS_PDM_NON_CRITICAL_ERRORS_AUX2_CURRENT_OUT_OF_RANGE_OVERFLOW_CHOICE);
//}
//
//// PDM-11, PDM-13
// TEST_F(PdmStateMachineTest, check_aux2_current_can_signals_in_init_state)
//{
//    CheckCurrentCanSignalsInInitState(
//        AUX2_MIN_CURRENT, AUX2_MAX_CURRENT, GetAux2Current_fake.return_val,
//        App_CanTx_GetPeriodicSignal_AUXILIARY2_CURRENT, App_CanTx_GetPeriodicSignal_AUX2_CURRENT_OUT_OF_RANGE,
//        CANMSGS_PDM_NON_CRITICAL_ERRORS_AUX2_CURRENT_OUT_OF_RANGE_OK_CHOICE);
//}
//
//// PDM-11, PDM-13
// TEST_F(PdmStateMachineTest, check_left_inverter_current_can_signals_in_non_init_states)
//{
//    CheckCurrentCanSignalsInNonInitStates(
//        LEFT_INVERTER_MIN_CURRENT, LEFT_INVERTER_MAX_CURRENT, GetLeftInverterCurrent_fake.return_val,
//        App_CanTx_GetPeriodicSignal_LEFT_INVERTER_CURRENT,
//        App_CanTx_GetPeriodicSignal_LEFT_INVERTER_CURRENT_OUT_OF_RANGE,
//        CANMSGS_PDM_NON_CRITICAL_ERRORS_LEFT_INVERTER_CURRENT_OUT_OF_RANGE_OK_CHOICE,
//        CANMSGS_PDM_NON_CRITICAL_ERRORS_LEFT_INVERTER_CURRENT_OUT_OF_RANGE_UNDERFLOW_CHOICE,
//        CANMSGS_PDM_NON_CRITICAL_ERRORS_LEFT_INVERTER_CURRENT_OUT_OF_RANGE_OVERFLOW_CHOICE);
//}
//
//// PDM-11, PDM-13
// TEST_F(PdmStateMachineTest, check_left_inverter_current_can_signals_in_init_state)
//{
//    CheckCurrentCanSignalsInInitState(
//        LEFT_INVERTER_MIN_CURRENT, LEFT_INVERTER_MAX_CURRENT, GetLeftInverterCurrent_fake.return_val,
//        App_CanTx_GetPeriodicSignal_LEFT_INVERTER_CURRENT,
//        App_CanTx_GetPeriodicSignal_LEFT_INVERTER_CURRENT_OUT_OF_RANGE,
//        CANMSGS_PDM_NON_CRITICAL_ERRORS_LEFT_INVERTER_CURRENT_OUT_OF_RANGE_OK_CHOICE);
//}
//
//// PDM-11, PDM-13
// TEST_F(PdmStateMachineTest, check_right_inverter_current_can_signals_in_non_init_states)
//{
//    CheckCurrentCanSignalsInNonInitStates(
//        RIGHT_INVERTER_MIN_CURRENT, RIGHT_INVERTER_MAX_CURRENT, GetRightInverterCurrent_fake.return_val,
//        App_CanTx_GetPeriodicSignal_RIGHT_INVERTER_CURRENT,
//        App_CanTx_GetPeriodicSignal_RIGHT_INVERTER_CURRENT_OUT_OF_RANGE,
//        CANMSGS_PDM_NON_CRITICAL_ERRORS_RIGHT_INVERTER_CURRENT_OUT_OF_RANGE_OK_CHOICE,
//        CANMSGS_PDM_NON_CRITICAL_ERRORS_RIGHT_INVERTER_CURRENT_OUT_OF_RANGE_UNDERFLOW_CHOICE,
//        CANMSGS_PDM_NON_CRITICAL_ERRORS_RIGHT_INVERTER_CURRENT_OUT_OF_RANGE_OVERFLOW_CHOICE);
//}
//
//// PDM-11, PDM-13
// TEST_F(PdmStateMachineTest, check_right_inverter_current_can_signals_in_init_state)
//{
//    CheckCurrentCanSignalsInInitState(
//        RIGHT_INVERTER_MIN_CURRENT, RIGHT_INVERTER_MAX_CURRENT, GetRightInverterCurrent_fake.return_val,
//        App_CanTx_GetPeriodicSignal_RIGHT_INVERTER_CURRENT,
//        App_CanTx_GetPeriodicSignal_RIGHT_INVERTER_CURRENT_OUT_OF_RANGE,
//        CANMSGS_PDM_NON_CRITICAL_ERRORS_RIGHT_INVERTER_CURRENT_OUT_OF_RANGE_OK_CHOICE);
//}
//
//// PDM-11, PDM-13
// TEST_F(PdmStateMachineTest, check_energy_meter_current_can_signals_in_non_init_states)
//{
//    CheckCurrentCanSignalsInNonInitStates(
//        ENERGY_METER_MIN_CURRENT, ENERGY_METER_MAX_CURRENT, GetEnergyMeterCurrent_fake.return_val,
//        App_CanTx_GetPeriodicSignal_ENERGY_METER_CURRENT,
//        App_CanTx_GetPeriodicSignal_ENERGY_METER_CURRENT_OUT_OF_RANGE,
//        CANMSGS_PDM_NON_CRITICAL_ERRORS_ENERGY_METER_CURRENT_OUT_OF_RANGE_OK_CHOICE,
//        CANMSGS_PDM_NON_CRITICAL_ERRORS_ENERGY_METER_CURRENT_OUT_OF_RANGE_UNDERFLOW_CHOICE,
//        CANMSGS_PDM_NON_CRITICAL_ERRORS_ENERGY_METER_CURRENT_OUT_OF_RANGE_OVERFLOW_CHOICE);
//}
//
//// PDM-11, PDM-13
// TEST_F(PdmStateMachineTest, check_energy_meter_current_can_signals_in_init_state)
//{
//    CheckCurrentCanSignalsInInitState(
//        ENERGY_METER_MIN_CURRENT, ENERGY_METER_MAX_CURRENT, GetEnergyMeterCurrent_fake.return_val,
//        App_CanTx_GetPeriodicSignal_ENERGY_METER_CURRENT,
//        App_CanTx_GetPeriodicSignal_ENERGY_METER_CURRENT_OUT_OF_RANGE,
//        CANMSGS_PDM_NON_CRITICAL_ERRORS_ENERGY_METER_CURRENT_OUT_OF_RANGE_OK_CHOICE);
//}
//
//// PDM-11, PDM-13
// TEST_F(PdmStateMachineTest, check_can_current_can_signals_in_non_init_states)
//{
//    CheckCurrentCanSignalsInNonInitStates(
//        CAN_MIN_CURRENT, CAN_MAX_CURRENT, GetCanCurrent_fake.return_val, App_CanTx_GetPeriodicSignal_CAN_CURRENT,
//        App_CanTx_GetPeriodicSignal_CAN_CURRENT_OUT_OF_RANGE,
//        CANMSGS_PDM_NON_CRITICAL_ERRORS_CAN_CURRENT_OUT_OF_RANGE_OK_CHOICE,
//        CANMSGS_PDM_NON_CRITICAL_ERRORS_CAN_CURRENT_OUT_OF_RANGE_UNDERFLOW_CHOICE,
//        CANMSGS_PDM_NON_CRITICAL_ERRORS_CAN_CURRENT_OUT_OF_RANGE_OVERFLOW_CHOICE);
//}
//
//// PDM-11, PDM-13
// TEST_F(PdmStateMachineTest, check_can_current_can_signals_in_init_state)
//{
//    CheckCurrentCanSignalsInInitState(
//        CAN_MIN_CURRENT, CAN_MAX_CURRENT, GetCanCurrent_fake.return_val, App_CanTx_GetPeriodicSignal_CAN_CURRENT,
//        App_CanTx_GetPeriodicSignal_CAN_CURRENT_OUT_OF_RANGE,
//        CANMSGS_PDM_NON_CRITICAL_ERRORS_CAN_CURRENT_OUT_OF_RANGE_OK_CHOICE);
//}
//
//// PDM-11, PDM-13
// TEST_F(PdmStateMachineTest, check_air_shutdown_current_can_signals_in_non_init_states)
//{
//    CheckCurrentCanSignalsInNonInitStates(
//        AIR_SHUTDOWN_MIN_CURRENT, AIR_SHUTDOWN_MAX_CURRENT, GetAirShutdownCurrent_fake.return_val,
//        App_CanTx_GetPeriodicSignal_AIR_SHUTDOWN_CURRENT,
//        App_CanTx_GetPeriodicSignal_AIR_SHUTDOWN_CURRENT_OUT_OF_RANGE,
//        CANMSGS_PDM_NON_CRITICAL_ERRORS_AIR_SHUTDOWN_CURRENT_OUT_OF_RANGE_OK_CHOICE,
//        CANMSGS_PDM_NON_CRITICAL_ERRORS_AIR_SHUTDOWN_CURRENT_OUT_OF_RANGE_UNDERFLOW_CHOICE,
//        CANMSGS_PDM_NON_CRITICAL_ERRORS_AIR_SHUTDOWN_CURRENT_OUT_OF_RANGE_OVERFLOW_CHOICE);
//}
//
//// PDM-11, PDM-13
// TEST_F(PdmStateMachineTest, check_air_shutdown_current_can_signals_in_init_state)
//{
//    CheckCurrentCanSignalsInInitState(
//        AIR_SHUTDOWN_MIN_CURRENT, AIR_SHUTDOWN_MAX_CURRENT, GetAirShutdownCurrent_fake.return_val,
//        App_CanTx_GetPeriodicSignal_AIR_SHUTDOWN_CURRENT,
//        App_CanTx_GetPeriodicSignal_AIR_SHUTDOWN_CURRENT_OUT_OF_RANGE,
//        CANMSGS_PDM_NON_CRITICAL_ERRORS_AIR_SHUTDOWN_CURRENT_OUT_OF_RANGE_OK_CHOICE);
//}

TEST_F(PdmStateMachineTest, rgb_led_sequence_in_all_states)
{
    unsigned int *call_counts[] = { &turn_on_red_led_fake.call_count, &turn_on_green_led_fake.call_count,
                                    &turn_on_blue_led_fake.call_count };

    for (auto &state : GetAllStates())
    {
        SetInitialState(state);

        RESET_FAKE(turn_on_red_led);
        RESET_FAKE(turn_on_green_led);
        RESET_FAKE(turn_on_blue_led);

        // Verify that we cycle through red, green, blue, red, etc for 99 times.
        // The number 99 can be changed to any other number that is a multiple
        // of 3. The significance of 3 is because we have 3 colors (Red, green,
        // and blue).
        for (size_t i = 0; i < 99; i++)
        {
            LetTimePass(state_machine, 1000);
            ASSERT_EQ(*call_counts[i % 3], i / 3 + 1);
        }
    }
}

// PDM-17
TEST_F(PdmStateMachineTest, exit_air_open_state_when_air_positive_and_air_negative_are_closed)
{
    SetInitialState(App_GetInitState());

    App_CanRx_BMS_Contactors_AirPositive_Update(CONTACTOR_STATE_CLOSED);
    App_CanRx_BMS_Contactors_AirNegative_Update(CONTACTOR_STATE_CLOSED);
    LetTimePass(state_machine, 10);

    ASSERT_EQ(App_GetDriveState(), App_SharedStateMachine_GetCurrentState(state_machine));
}

// PDM-17
TEST_F(PdmStateMachineTest, stay_in_air_open_state_if_only_air_positive_is_closed)
{
    SetInitialState(App_GetInitState());

    App_CanRx_BMS_Contactors_AirNegative_Update(CONTACTOR_STATE_OPEN);
    App_CanRx_BMS_Contactors_AirPositive_Update(CONTACTOR_STATE_CLOSED);
    LetTimePass(state_machine, 10);

    ASSERT_EQ(App_GetInitState(), App_SharedStateMachine_GetCurrentState(state_machine));
}

// PDM-17
TEST_F(PdmStateMachineTest, stay_in_air_open_state_if_only_air_negative_is_closed)
{
    SetInitialState(App_GetInitState());

    App_CanRx_BMS_Contactors_AirPositive_Update(CONTACTOR_STATE_OPEN);
    App_CanRx_BMS_Contactors_AirNegative_Update(CONTACTOR_STATE_CLOSED);
    LetTimePass(state_machine, 10);

    ASSERT_EQ(App_GetInitState(), App_SharedStateMachine_GetCurrentState(state_machine));
}

// PDM-20
TEST_F(PdmStateMachineTest, exit_air_closed_state_when_air_positive_is_opened)
{
    SetInitialState(App_GetDriveState());

    App_CanRx_BMS_Contactors_AirNegative_Update(CONTACTOR_STATE_CLOSED);
    App_CanRx_BMS_Contactors_AirPositive_Update(CONTACTOR_STATE_OPEN);

    LetTimePass(state_machine, 10);

    ASSERT_EQ(App_GetInitState(), App_SharedStateMachine_GetCurrentState(state_machine));
}

// PDM-20
TEST_F(PdmStateMachineTest, exit_air_closed_state_when_air_negative_is_opened)
{
    SetInitialState(App_GetDriveState());

    App_CanRx_BMS_Contactors_AirPositive_Update(CONTACTOR_STATE_CLOSED);
    App_CanRx_BMS_Contactors_AirNegative_Update(CONTACTOR_STATE_OPEN);
    LetTimePass(state_machine, 10);

    ASSERT_EQ(App_GetInitState(), App_SharedStateMachine_GetCurrentState(state_machine));
}

//// PDM-4
// TEST_F(PdmStateMachineTest, set_18650s_charge_fault_in_all_states)
//{
//    for (auto &state : GetAllStates())
//    {
//        SetInitialState(state);
//
//        // Clear the fault to prevent false positive
//        App_CanTx_SetPeriodicSignal_CHARGER_FAULT(can_tx_interface, false);
//
//        do_low_voltage_battery_have_charge_fault_fake.return_val = true;
//        LetTimePass(state_machine, 10);
//
//        ASSERT_TRUE(App_CanTx_GetPeriodicSignal_CHARGER_FAULT(can_tx_interface));
//    }
//}
//
//// PDM-4
// TEST_F(PdmStateMachineTest, clear_18650s_charge_fault_in_all_states)
//{
//    for (auto &state : GetAllStates())
//    {
//        SetInitialState(state);
//
//        // Set the fault to prevent false positive
//        App_CanTx_SetPeriodicSignal_CHARGER_FAULT(can_tx_interface, true);
//
//        do_low_voltage_battery_have_charge_fault_fake.return_val = false;
//        LetTimePass(state_machine, 10);
//
//        ASSERT_FALSE(App_CanTx_GetPeriodicSignal_CHARGER_FAULT(can_tx_interface));
//    }
//}
//
//// PDM-5
// TEST_F(PdmStateMachineTest, set_18650s_boost_controller_fault_in_all_states)
//{
//    for (auto &state : GetAllStates())
//    {
//        SetInitialState(state);
//
//        // Clear the fault to prevent false positive
//        App_CanTx_SetPeriodicSignal_BOOST_PGOOD_FAULT(can_tx_interface, false);
//
//        do_low_voltage_battery_have_boost_controller_fault_fake.return_val = true;
//        LetTimePass(state_machine, 10);
//
//        ASSERT_TRUE(App_CanTx_GetPeriodicSignal_BOOST_PGOOD_FAULT(can_tx_interface));
//    }
//}
//
//// PDM-5
// TEST_F(PdmStateMachineTest, clear_18650s_boost_controller_fault_in_all_states)
//{
//    for (auto &state : GetAllStates())
//    {
//        SetInitialState(state);
//
//        // Set the fault to prevent false positive
//        App_CanTx_SetPeriodicSignal_BOOST_PGOOD_FAULT(can_tx_interface, true);
//
//        do_low_voltage_battery_have_boost_controller_fault_fake.return_val = false;
//        LetTimePass(state_machine, 10);
//
//        ASSERT_FALSE(App_CanTx_GetPeriodicSignal_BOOST_PGOOD_FAULT(can_tx_interface));
//    }
//}
} // namespace StateMachineTest
