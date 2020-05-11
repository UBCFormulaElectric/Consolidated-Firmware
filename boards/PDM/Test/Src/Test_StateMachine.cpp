#include <math.h>
#include "Test_Pdm.h"

extern "C"
{
#include "App_SharedHeartbeatMonitor.h"
#include "App_SharedStateMachine.h"
#include "states/App_InitState.h"
#include "states/App_AirOpenState.h"
#include "states/App_AirClosedState.h"
#include "configs/App_VoltageLimits.h"
#include "configs/App_CurrentLimits.h"
#include "configs/App_HeartbeatMonitorConfig.h"
}

namespace StateMachineTest
{
FAKE_VOID_FUNC(
    send_non_periodic_msg_PDM_STARTUP,
    const struct CanMsgs_pdm_startup_t *);
FAKE_VOID_FUNC(
    send_non_periodic_msg_PDM_AIR_SHUTDOWN,
    const struct CanMsgs_pdm_air_shutdown_t *);
FAKE_VOID_FUNC(
    send_non_periodic_msg_PDM_MOTOR_SHUTDOWN,
    const struct CanMsgs_pdm_motor_shutdown_t *);
FAKE_VOID_FUNC(
    send_non_periodic_msg_PDM_WATCHDOG_TIMEOUT,
    const struct CanMsgs_pdm_watchdog_timeout_t *);

FAKE_VALUE_FUNC(float, GetVbatVoltage);
FAKE_VALUE_FUNC(float, Get24vAuxVoltage);
FAKE_VALUE_FUNC(float, Get24vAccVoltage);
FAKE_VALUE_FUNC(float, GetAux1Current);
FAKE_VALUE_FUNC(float, GetAux2Current);
FAKE_VALUE_FUNC(float, GetLeftInverterCurrent);
FAKE_VALUE_FUNC(float, GetRightInverterCurrent);
FAKE_VALUE_FUNC(float, GetEnergyMeterCurrent);
FAKE_VALUE_FUNC(float, GetCanCurrent);
FAKE_VALUE_FUNC(float, GetAirShutdownCurrent);

FAKE_VALUE_FUNC(uint32_t, get_current_ms);
FAKE_VOID_FUNC(
    heartbeat_timeout_callback,
    enum HeartbeatOneHot,
    enum HeartbeatOneHot);

FAKE_VOID_FUNC(turn_on_red_led);
FAKE_VOID_FUNC(turn_on_green_led);
FAKE_VOID_FUNC(turn_on_blue_led);

class PdmStateMachineTest : public testing::Test
{
  protected:
    void SetUp() override
    {
        can_tx_interface = App_CanTx_Create(
            send_non_periodic_msg_PDM_STARTUP,
            send_non_periodic_msg_PDM_AIR_SHUTDOWN,
            send_non_periodic_msg_PDM_MOTOR_SHUTDOWN,
            send_non_periodic_msg_PDM_WATCHDOG_TIMEOUT);

        can_rx_interface = App_CanRx_Create();

        vbat_voltage_in_range_check = App_InRangeCheck_Create(
            GetVbatVoltage, VBAT_MIN_VOLTAGE, VBAT_MAX_VOLTAGE);

        _24v_aux_voltage_in_range_check = App_InRangeCheck_Create(
            Get24vAuxVoltage, _24V_AUX_MIN_VOLTAGE, _24V_AUX_MAX_VOLTAGE);

        _24v_acc_voltage_in_range_check = App_InRangeCheck_Create(
            Get24vAccVoltage, _24V_ACC_MIN_VOLTAGE, _24V_ACC_MAX_VOLTAGE);

        aux1_current_in_range_check = App_InRangeCheck_Create(
            GetAux1Current, AUX1_MIN_CURRENT, AUX1_MAX_CURRENT);

        aux2_current_in_range_check = App_InRangeCheck_Create(
            GetAux2Current, AUX2_MIN_CURRENT, AUX2_MAX_CURRENT);

        left_inverter_current_in_range_check = App_InRangeCheck_Create(
            GetLeftInverterCurrent, LEFT_INVERTER_MIN_CURRENT,
            LEFT_INVERTER_MAX_CURRENT);

        right_inverter_current_in_range_check = App_InRangeCheck_Create(
            GetRightInverterCurrent, RIGHT_INVERTER_MIN_CURRENT,
            RIGHT_INVERTER_MAX_CURRENT);

        energy_meter_current_in_range_check = App_InRangeCheck_Create(
            GetEnergyMeterCurrent, ENERGY_METER_MIN_CURRENT,
            ENERGY_METER_MAX_CURRENT);

        can_current_in_range_check = App_InRangeCheck_Create(
            GetCanCurrent, CAN_MIN_CURRENT, CAN_MAX_CURRENT);

        air_shutdown_current_in_range_check = App_InRangeCheck_Create(
            GetAirShutdownCurrent, AIR_SHUTDOWN_MIN_CURRENT,
            AIR_SHUTDOWN_MAX_CURRENT);

        heartbeat_monitor = App_SharedHeartbeatMonitor_Create(
            get_current_ms, HEARTBEAT_MONITOR_TIMEOUT_PERIOD_MS,
            HEARTBEAT_MONITOR_BOARDS_TO_CHECK, heartbeat_timeout_callback);

        rgb_led_sequence = App_SharedRgbLedSequence_Create(
            turn_on_red_led, turn_on_green_led, turn_on_blue_led);

        world = App_PdmWorld_Create(
            can_tx_interface, can_rx_interface, vbat_voltage_in_range_check,
            _24v_aux_voltage_in_range_check, _24v_acc_voltage_in_range_check,
            aux1_current_in_range_check, aux2_current_in_range_check,
            left_inverter_current_in_range_check,
            right_inverter_current_in_range_check,
            energy_meter_current_in_range_check, can_current_in_range_check,
            air_shutdown_current_in_range_check, heartbeat_monitor,
            rgb_led_sequence);

        // Default to starting the state machine in the `init` state
        state_machine =
            App_SharedStateMachine_Create(world, App_GetInitState());

        // Reset fake functions
        RESET_FAKE(send_non_periodic_msg_PDM_STARTUP);
        RESET_FAKE(send_non_periodic_msg_PDM_AIR_SHUTDOWN);
        RESET_FAKE(send_non_periodic_msg_PDM_MOTOR_SHUTDOWN);
        RESET_FAKE(send_non_periodic_msg_PDM_WATCHDOG_TIMEOUT);
        RESET_FAKE(GetVbatVoltage);
        RESET_FAKE(Get24vAuxVoltage);
        RESET_FAKE(Get24vAccVoltage);
        RESET_FAKE(GetAux1Current);
        RESET_FAKE(GetAux2Current);
        RESET_FAKE(GetLeftInverterCurrent);
        RESET_FAKE(GetRightInverterCurrent);
        RESET_FAKE(GetEnergyMeterCurrent);
        RESET_FAKE(GetCanCurrent);
        RESET_FAKE(GetAirShutdownCurrent);
        RESET_FAKE(get_current_ms);
        RESET_FAKE(heartbeat_timeout_callback);
        RESET_FAKE(turn_on_red_led);
        RESET_FAKE(turn_on_green_led);
        RESET_FAKE(turn_on_blue_led);
    }

    void TearDown() override
    {
        TearDownObject(world, App_PdmWorld_Destroy);
        TearDownObject(can_tx_interface, App_CanTx_Destroy);
        TearDownObject(can_rx_interface, App_CanRx_Destroy);
        TearDownObject(vbat_voltage_in_range_check, App_InRangeCheck_Destroy);
        TearDownObject(
            _24v_aux_voltage_in_range_check, App_InRangeCheck_Destroy);
        TearDownObject(
            _24v_acc_voltage_in_range_check, App_InRangeCheck_Destroy);
        TearDownObject(aux1_current_in_range_check, App_InRangeCheck_Destroy);
        TearDownObject(aux2_current_in_range_check, App_InRangeCheck_Destroy);
        TearDownObject(
            left_inverter_current_in_range_check, App_InRangeCheck_Destroy);
        TearDownObject(
            right_inverter_current_in_range_check, App_InRangeCheck_Destroy);
        TearDownObject(
            energy_meter_current_in_range_check, App_InRangeCheck_Destroy);
        TearDownObject(can_current_in_range_check, App_InRangeCheck_Destroy);
        TearDownObject(
            air_shutdown_current_in_range_check, App_InRangeCheck_Destroy);
        TearDownObject(heartbeat_monitor, App_SharedHeartbeatMonitor_Destroy);
        TearDownObject(rgb_led_sequence, App_SharedRgbLedSequence_Destroy);
        TearDownObject(state_machine, App_SharedStateMachine_Destroy);
    }

    void SetInitialState(const struct State *const initial_state)
    {
        TearDownObject(state_machine, App_SharedStateMachine_Destroy);
        state_machine = App_SharedStateMachine_Create(world, initial_state);
        ASSERT_EQ(
            initial_state,
            App_SharedStateMachine_GetCurrentState(state_machine));
    }

    std::vector<const struct State *> GetAllStates(void)
    {
        return std::vector<const struct State *>{
            App_GetInitState(),
            App_GetAirClosedState(),
            App_GetAirOpenState(),
        };
    }

    std::vector<const struct State *> GetAllStatesExceptForInit(void)
    {
        std::vector<const struct State *> vec = GetAllStates();

        vec.erase(
            std::remove(vec.begin(), vec.end(), App_GetInitState()), vec.end());

        return vec;
    }

    void CheckCurrentCanSignalsInNonInitStates(
        float  min_current,
        float  max_current,
        float &fake_current,
        float (*current_can_signal_getter)(const struct PdmCanTxInterface *),
        uint8_t (*out_of_range_can_signal_getter)(
            const struct PdmCanTxInterface *),
        uint8_t ok_choice,
        uint8_t underflow_choice,
        uint8_t overflow_choice)
    {
        for (auto &state : GetAllStatesExceptForInit())
        {
            SetInitialState(state);

            // Normal Value
            fake_current = (min_current + max_current) / 2;
            App_SharedStateMachine_Tick1kHz(state_machine);
            EXPECT_EQ(
                fake_current, current_can_signal_getter(can_tx_interface));
            EXPECT_EQ(
                ok_choice, out_of_range_can_signal_getter(can_tx_interface));

            // Under-current
            fake_current = std::nextafter(
                min_current, std::numeric_limits<float>::lowest());
            App_SharedStateMachine_Tick1kHz(state_machine);
            EXPECT_EQ(
                underflow_choice,
                out_of_range_can_signal_getter(can_tx_interface));

            // Over-current
            fake_current =
                std::nextafter(max_current, std::numeric_limits<float>::max());
            App_SharedStateMachine_Tick1kHz(state_machine);
            EXPECT_EQ(
                overflow_choice,
                out_of_range_can_signal_getter(can_tx_interface));
        }
    }

    void CheckCurrentCanSignalsInInitState(
        float  min_current,
        float  max_current,
        float &fake_current,
        float (*current_can_signal_getter)(const struct PdmCanTxInterface *),
        uint8_t (*out_of_range_can_signal_getter)(
            const struct PdmCanTxInterface *),
        uint8_t ok_choice)
    {
        // In the INIT state, the load switches aren't ready yet. Therefore, the
        // current readings reported by the IO layer are assumed to be garbage.
        // To get around this, we simply hard-code the CAN TX current signals
        // as NAN in the INIT state.

        SetInitialState(App_GetInitState());

        // Normal Value
        fake_current = (min_current + max_current) / 2;
        App_SharedStateMachine_Tick1kHz(state_machine);
        EXPECT_TRUE(isnanf(current_can_signal_getter(can_tx_interface)));
        EXPECT_EQ(ok_choice, out_of_range_can_signal_getter(can_tx_interface));

        // Under-current
        fake_current =
            std::nextafter(min_current, std::numeric_limits<float>::lowest());
        App_SharedStateMachine_Tick1kHz(state_machine);
        EXPECT_TRUE(isnanf(current_can_signal_getter(can_tx_interface)));
        EXPECT_EQ(ok_choice, out_of_range_can_signal_getter(can_tx_interface));

        // Over-current
        fake_current =
            std::nextafter(max_current, std::numeric_limits<float>::max());
        App_SharedStateMachine_Tick1kHz(state_machine);
        EXPECT_TRUE(isnanf(current_can_signal_getter(can_tx_interface)));
        EXPECT_EQ(ok_choice, out_of_range_can_signal_getter(can_tx_interface));
    }

    void CheckVoltageCanSignalsInAllStates(
        float  min_voltage,
        float  max_voltage,
        float &fake_voltage,
        float (*voltage_can_signal_getter)(const struct PdmCanTxInterface *),
        uint8_t (*out_of_range_can_signal_getter)(
            const struct PdmCanTxInterface *),
        uint8_t ok_choice,
        uint8_t underflow_choice,
        uint8_t overflow_choice)
    {
        for (auto &state : GetAllStates())
        {
            SetInitialState(state);

            // Normal Value
            fake_voltage = (min_voltage + max_voltage) / 2;
            App_SharedStateMachine_Tick1kHz(state_machine);
            EXPECT_EQ(
                fake_voltage, voltage_can_signal_getter(can_tx_interface));
            EXPECT_EQ(
                ok_choice, out_of_range_can_signal_getter(can_tx_interface));

            // Under-voltage
            fake_voltage = std::nextafter(
                min_voltage, std::numeric_limits<float>::lowest());
            App_SharedStateMachine_Tick1kHz(state_machine);
            EXPECT_EQ(
                underflow_choice,
                out_of_range_can_signal_getter(can_tx_interface));

            // Over-voltage
            fake_voltage =
                std::nextafter(max_voltage, std::numeric_limits<float>::max());
            App_SharedStateMachine_Tick1kHz(state_machine);
            EXPECT_EQ(
                overflow_choice,
                out_of_range_can_signal_getter(can_tx_interface));
        }
    }

    struct World *            world;
    struct StateMachine *     state_machine;
    struct PdmCanTxInterface *can_tx_interface;
    struct PdmCanRxInterface *can_rx_interface;
    struct InRangeCheck *     vbat_voltage_in_range_check;
    struct InRangeCheck *     _24v_aux_voltage_in_range_check;
    struct InRangeCheck *     _24v_acc_voltage_in_range_check;
    struct InRangeCheck *     aux1_current_in_range_check;
    struct InRangeCheck *     aux2_current_in_range_check;
    struct InRangeCheck *     left_inverter_current_in_range_check;
    struct InRangeCheck *     right_inverter_current_in_range_check;
    struct InRangeCheck *     energy_meter_current_in_range_check;
    struct InRangeCheck *     can_current_in_range_check;
    struct InRangeCheck *     air_shutdown_current_in_range_check;
    struct HeartbeatMonitor * heartbeat_monitor;
    struct RgbLedSequence *   rgb_led_sequence;
};

TEST_F(PdmStateMachineTest, check_init_state_is_broadcasted_over_can)
{
    SetInitialState(App_GetInitState());

    EXPECT_EQ(
        CANMSGS_PDM_STATE_MACHINE_STATE_INIT_CHOICE,
        App_CanTx_GetPeriodicSignal_STATE(can_tx_interface));
}

TEST_F(PdmStateMachineTest, check_air_open_state_is_broadcasted_over_can)
{
    SetInitialState(App_GetAirOpenState());

    EXPECT_EQ(
        CANMSGS_PDM_STATE_MACHINE_STATE_AIR_OPEN_CHOICE,
        App_CanTx_GetPeriodicSignal_STATE(can_tx_interface));
}

TEST_F(PdmStateMachineTest, check_air_closed_state_is_broadcasted_over_can)
{
    SetInitialState(App_GetAirClosedState());

    EXPECT_EQ(
        CANMSGS_PDM_STATE_MACHINE_STATE_AIR_CLOSED_CHOICE,
        App_CanTx_GetPeriodicSignal_STATE(can_tx_interface));
}

TEST_F(PdmStateMachineTest, check_vbat_voltage_can_signals_in_all_states)
{
    CheckVoltageCanSignalsInAllStates(
        VBAT_MIN_VOLTAGE, VBAT_MAX_VOLTAGE, GetVbatVoltage_fake.return_val,
        App_CanTx_GetPeriodicSignal_VBAT,
        App_CanTx_GetPeriodicSignal_VBAT_VOLTAGE_OUT_OF_RANGE,
        CANMSGS_PDM_ERRORS_VBAT_VOLTAGE_OUT_OF_RANGE_OK_CHOICE,
        CANMSGS_PDM_ERRORS_VBAT_VOLTAGE_OUT_OF_RANGE_UNDERFLOW_CHOICE,
        CANMSGS_PDM_ERRORS_VBAT_VOLTAGE_OUT_OF_RANGE_OVERFLOW_CHOICE);
}

TEST_F(PdmStateMachineTest, check_24v_aux_voltage_can_signals_in_all_states)
{
    CheckVoltageCanSignalsInAllStates(
        _24V_AUX_MIN_VOLTAGE, _24V_AUX_MAX_VOLTAGE,
        Get24vAuxVoltage_fake.return_val, App_CanTx_GetPeriodicSignal__24_V_AUX,
        App_CanTx_GetPeriodicSignal__24_V_AUX_VOLTAGE_OUT_OF_RANGE,
        CANMSGS_PDM_ERRORS__24_V_AUX_VOLTAGE_OUT_OF_RANGE_OK_CHOICE,
        CANMSGS_PDM_ERRORS__24_V_AUX_VOLTAGE_OUT_OF_RANGE_UNDERFLOW_CHOICE,
        CANMSGS_PDM_ERRORS__24_V_AUX_VOLTAGE_OUT_OF_RANGE_OVERFLOW_CHOICE);
}

TEST_F(PdmStateMachineTest, check_24v_acc_voltage_can_signals_in_all_states)
{
    CheckVoltageCanSignalsInAllStates(
        _24V_ACC_MIN_VOLTAGE, _24V_ACC_MAX_VOLTAGE,
        Get24vAccVoltage_fake.return_val, App_CanTx_GetPeriodicSignal__24_V_ACC,
        App_CanTx_GetPeriodicSignal__24_V_ACC_VOLTAGE_OUT_OF_RANGE,
        CANMSGS_PDM_ERRORS__24_V_ACC_VOLTAGE_OUT_OF_RANGE_OK_CHOICE,
        CANMSGS_PDM_ERRORS__24_V_ACC_VOLTAGE_OUT_OF_RANGE_UNDERFLOW_CHOICE,
        CANMSGS_PDM_ERRORS__24_V_ACC_VOLTAGE_OUT_OF_RANGE_OVERFLOW_CHOICE);
}

TEST_F(PdmStateMachineTest, check_aux1_current_can_signals_in_non_init_states)
{
    CheckCurrentCanSignalsInNonInitStates(
        AUX1_MIN_CURRENT, AUX1_MAX_CURRENT, GetAux1Current_fake.return_val,
        App_CanTx_GetPeriodicSignal_AUXILIARY1_CURRENT,
        App_CanTx_GetPeriodicSignal_AUX1_CURRENT_OUT_OF_RANGE,
        CANMSGS_PDM_ERRORS_AUX1_CURRENT_OUT_OF_RANGE_OK_CHOICE,
        CANMSGS_PDM_ERRORS_AUX1_CURRENT_OUT_OF_RANGE_UNDERFLOW_CHOICE,
        CANMSGS_PDM_ERRORS_AUX1_CURRENT_OUT_OF_RANGE_OVERFLOW_CHOICE);
}

TEST_F(PdmStateMachineTest, check_aux1_current_can_signals_in_init_state)
{
    CheckCurrentCanSignalsInInitState(
        AUX1_MIN_CURRENT, AUX1_MAX_CURRENT, GetAux1Current_fake.return_val,
        App_CanTx_GetPeriodicSignal_AUXILIARY1_CURRENT,
        App_CanTx_GetPeriodicSignal_AUX1_CURRENT_OUT_OF_RANGE,
        CANMSGS_PDM_ERRORS_AUX1_CURRENT_OUT_OF_RANGE_OK_CHOICE);
}

TEST_F(PdmStateMachineTest, check_aux2_current_can_signals_in_non_init_states)
{
    CheckCurrentCanSignalsInNonInitStates(
        AUX2_MIN_CURRENT, AUX2_MAX_CURRENT, GetAux2Current_fake.return_val,
        App_CanTx_GetPeriodicSignal_AUXILIARY2_CURRENT,
        App_CanTx_GetPeriodicSignal_AUX2_CURRENT_OUT_OF_RANGE,
        CANMSGS_PDM_ERRORS_AUX2_CURRENT_OUT_OF_RANGE_OK_CHOICE,
        CANMSGS_PDM_ERRORS_AUX2_CURRENT_OUT_OF_RANGE_UNDERFLOW_CHOICE,
        CANMSGS_PDM_ERRORS_AUX2_CURRENT_OUT_OF_RANGE_OVERFLOW_CHOICE);
}

TEST_F(PdmStateMachineTest, check_aux2_current_can_signals_in_init_state)
{
    CheckCurrentCanSignalsInInitState(
        AUX2_MIN_CURRENT, AUX2_MAX_CURRENT, GetAux2Current_fake.return_val,
        App_CanTx_GetPeriodicSignal_AUXILIARY2_CURRENT,
        App_CanTx_GetPeriodicSignal_AUX2_CURRENT_OUT_OF_RANGE,
        CANMSGS_PDM_ERRORS_AUX2_CURRENT_OUT_OF_RANGE_OK_CHOICE);
}

TEST_F(
    PdmStateMachineTest,
    check_left_inverter_current_can_signals_in_non_init_states)
{
    CheckCurrentCanSignalsInNonInitStates(
        LEFT_INVERTER_MIN_CURRENT, LEFT_INVERTER_MAX_CURRENT,
        GetLeftInverterCurrent_fake.return_val,
        App_CanTx_GetPeriodicSignal_LEFT_INVERTER_CURRENT,
        App_CanTx_GetPeriodicSignal_LEFT_INVERTER_CURRENT_OUT_OF_RANGE,
        CANMSGS_PDM_ERRORS_LEFT_INVERTER_CURRENT_OUT_OF_RANGE_OK_CHOICE,
        CANMSGS_PDM_ERRORS_LEFT_INVERTER_CURRENT_OUT_OF_RANGE_UNDERFLOW_CHOICE,
        CANMSGS_PDM_ERRORS_LEFT_INVERTER_CURRENT_OUT_OF_RANGE_OVERFLOW_CHOICE);
}

TEST_F(
    PdmStateMachineTest,
    check_left_inverter_current_can_signals_in_init_state)
{
    CheckCurrentCanSignalsInInitState(
        LEFT_INVERTER_MIN_CURRENT, LEFT_INVERTER_MAX_CURRENT,
        GetLeftInverterCurrent_fake.return_val,
        App_CanTx_GetPeriodicSignal_LEFT_INVERTER_CURRENT,
        App_CanTx_GetPeriodicSignal_LEFT_INVERTER_CURRENT_OUT_OF_RANGE,
        CANMSGS_PDM_ERRORS_LEFT_INVERTER_CURRENT_OUT_OF_RANGE_OK_CHOICE);
}

TEST_F(
    PdmStateMachineTest,
    check_right_inverter_current_can_signals_in_non_init_states)
{
    CheckCurrentCanSignalsInNonInitStates(
        RIGHT_INVERTER_MIN_CURRENT, RIGHT_INVERTER_MAX_CURRENT,
        GetRightInverterCurrent_fake.return_val,
        App_CanTx_GetPeriodicSignal_RIGHT_INVERTER_CURRENT,
        App_CanTx_GetPeriodicSignal_RIGHT_INVERTER_CURRENT_OUT_OF_RANGE,
        CANMSGS_PDM_ERRORS_RIGHT_INVERTER_CURRENT_OUT_OF_RANGE_OK_CHOICE,
        CANMSGS_PDM_ERRORS_RIGHT_INVERTER_CURRENT_OUT_OF_RANGE_UNDERFLOW_CHOICE,
        CANMSGS_PDM_ERRORS_RIGHT_INVERTER_CURRENT_OUT_OF_RANGE_OVERFLOW_CHOICE);
}

TEST_F(
    PdmStateMachineTest,
    check_right_inverter_current_can_signals_in_init_state)
{
    CheckCurrentCanSignalsInInitState(
        RIGHT_INVERTER_MIN_CURRENT, RIGHT_INVERTER_MAX_CURRENT,
        GetRightInverterCurrent_fake.return_val,
        App_CanTx_GetPeriodicSignal_RIGHT_INVERTER_CURRENT,
        App_CanTx_GetPeriodicSignal_RIGHT_INVERTER_CURRENT_OUT_OF_RANGE,
        CANMSGS_PDM_ERRORS_RIGHT_INVERTER_CURRENT_OUT_OF_RANGE_OK_CHOICE);
}

TEST_F(
    PdmStateMachineTest,
    check_energy_meter_current_can_signals_in_non_init_states)
{
    CheckCurrentCanSignalsInNonInitStates(
        ENERGY_METER_MIN_CURRENT, ENERGY_METER_MAX_CURRENT,
        GetEnergyMeterCurrent_fake.return_val,
        App_CanTx_GetPeriodicSignal_ENERGY_METER_CURRENT,
        App_CanTx_GetPeriodicSignal_ENERGY_METER_CURRENT_OUT_OF_RANGE,
        CANMSGS_PDM_ERRORS_ENERGY_METER_CURRENT_OUT_OF_RANGE_OK_CHOICE,
        CANMSGS_PDM_ERRORS_ENERGY_METER_CURRENT_OUT_OF_RANGE_UNDERFLOW_CHOICE,
        CANMSGS_PDM_ERRORS_ENERGY_METER_CURRENT_OUT_OF_RANGE_OVERFLOW_CHOICE);
}

TEST_F(
    PdmStateMachineTest,
    check_energy_meter_current_can_signals_in_init_state)
{
    CheckCurrentCanSignalsInInitState(
        ENERGY_METER_MIN_CURRENT, ENERGY_METER_MAX_CURRENT,
        GetEnergyMeterCurrent_fake.return_val,
        App_CanTx_GetPeriodicSignal_ENERGY_METER_CURRENT,
        App_CanTx_GetPeriodicSignal_ENERGY_METER_CURRENT_OUT_OF_RANGE,
        CANMSGS_PDM_ERRORS_ENERGY_METER_CURRENT_OUT_OF_RANGE_OK_CHOICE);
}

TEST_F(PdmStateMachineTest, check_can_current_can_signals_in_non_init_states)
{
    CheckCurrentCanSignalsInNonInitStates(
        CAN_MIN_CURRENT, CAN_MAX_CURRENT, GetCanCurrent_fake.return_val,
        App_CanTx_GetPeriodicSignal_CAN_CURRENT,
        App_CanTx_GetPeriodicSignal_CAN_CURRENT_OUT_OF_RANGE,
        CANMSGS_PDM_ERRORS_CAN_CURRENT_OUT_OF_RANGE_OK_CHOICE,
        CANMSGS_PDM_ERRORS_CAN_CURRENT_OUT_OF_RANGE_UNDERFLOW_CHOICE,
        CANMSGS_PDM_ERRORS_CAN_CURRENT_OUT_OF_RANGE_OVERFLOW_CHOICE);
}

TEST_F(PdmStateMachineTest, check_can_current_can_signals_in_init_state)
{
    CheckCurrentCanSignalsInInitState(
        CAN_MIN_CURRENT, CAN_MAX_CURRENT, GetCanCurrent_fake.return_val,
        App_CanTx_GetPeriodicSignal_CAN_CURRENT,
        App_CanTx_GetPeriodicSignal_CAN_CURRENT_OUT_OF_RANGE,
        CANMSGS_PDM_ERRORS_CAN_CURRENT_OUT_OF_RANGE_OK_CHOICE);
}

TEST_F(
    PdmStateMachineTest,
    check_air_shutdown_current_can_signals_in_non_init_states)
{
    CheckCurrentCanSignalsInNonInitStates(
        AIR_SHUTDOWN_MIN_CURRENT, AIR_SHUTDOWN_MAX_CURRENT,
        GetAirShutdownCurrent_fake.return_val,
        App_CanTx_GetPeriodicSignal_AIR_SHUTDOWN_CURRENT,
        App_CanTx_GetPeriodicSignal_AIR_SHUTDOWN_CURRENT_OUT_OF_RANGE,
        CANMSGS_PDM_ERRORS_AIR_SHUTDOWN_CURRENT_OUT_OF_RANGE_OK_CHOICE,
        CANMSGS_PDM_ERRORS_AIR_SHUTDOWN_CURRENT_OUT_OF_RANGE_UNDERFLOW_CHOICE,
        CANMSGS_PDM_ERRORS_AIR_SHUTDOWN_CURRENT_OUT_OF_RANGE_OVERFLOW_CHOICE);
}

TEST_F(
    PdmStateMachineTest,
    check_air_shutdown_current_can_signals_in_init_state)
{
    CheckCurrentCanSignalsInInitState(
        AIR_SHUTDOWN_MIN_CURRENT, AIR_SHUTDOWN_MAX_CURRENT,
        GetAirShutdownCurrent_fake.return_val,
        App_CanTx_GetPeriodicSignal_AIR_SHUTDOWN_CURRENT,
        App_CanTx_GetPeriodicSignal_AIR_SHUTDOWN_CURRENT_OUT_OF_RANGE,
        CANMSGS_PDM_ERRORS_AIR_SHUTDOWN_CURRENT_OUT_OF_RANGE_OK_CHOICE);
}

} // namespace StateMachineTest
