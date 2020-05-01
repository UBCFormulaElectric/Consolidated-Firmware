#include <math.h>
#include "Test_Pdm.h"

extern "C"
{
#include "App_SharedHeartbeatMonitor.h"
#include "App_SharedStateMachine.h"
#include "states/App_InitState.h"
#include "states/App_AirOpenState.h"
#include "states/App_AirClosedState.h"
#include "App_VoltageLimits.h"
#include "App_CurrentLimits.h"
}

FAKE_VOID_FUNC(
    send_non_periodic_msg_PDM_STARTUP,
    struct CanMsgs_pdm_startup_t *);
FAKE_VOID_FUNC(
    send_non_periodic_msg_PDM_AIR_SHUTDOWN,
    struct CanMsgs_pdm_air_shutdown_t *);
FAKE_VOID_FUNC(
    send_non_periodic_msg_PDM_MOTOR_SHUTDOWN,
    struct CanMsgs_pdm_motor_shutdown_t *);
FAKE_VOID_FUNC(
    send_non_periodic_msg_PDM_WATCHDOG_TIMEOUT,
    struct CanMsgs_pdm_watchdog_timeout_t *);

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
        constexpr uint32_t DEFAULT_HEARTBEAT_TIMEOUT_PERIOD_MS = 500U;
        constexpr enum HeartbeatOneHot DEFAULT_HEARTBEAT_BOARDS_TO_CHECK =
            BMS_HEARTBEAT_ONE_HOT;

        can_tx_interface = App_CanTx_Create(
            send_non_periodic_msg_PDM_STARTUP,
            send_non_periodic_msg_PDM_AIR_SHUTDOWN,
            send_non_periodic_msg_PDM_MOTOR_SHUTDOWN,
            send_non_periodic_msg_PDM_WATCHDOG_TIMEOUT);
        can_rx_interface   = App_CanRx_Create();
        vbat_voltage_check = App_InRangeCheck_Create(
            GetVbatVoltage, VBAT_MIN_VOLTAGE, VBAT_MAX_VOLTAGE);
        _24v_aux_voltage_check = App_InRangeCheck_Create(
            Get24vAuxVoltage, _24V_AUX_MIN_VOLTAGE, _24V_AUX_MAX_VOLTAGE);
        _24v_acc_voltage_check = App_InRangeCheck_Create(
            Get24vAccVoltage, _24V_ACC_MIN_VOLTAGE, _24V_ACC_MAX_VOLTAGE);
        aux1_current_check = App_InRangeCheck_Create(
            GetAux1Current, AUX1_MIN_CURRENT, AUX1_MAX_CURRENT);
        aux2_current_check = App_InRangeCheck_Create(
            GetAux1Current, AUX2_MIN_CURRENT, AUX2_MAX_CURRENT);
        left_inverter_current_check = App_InRangeCheck_Create(
            GetLeftInverterCurrent, LEFT_INVERTER_MIN_CURRENT,
            LEFT_INVERTER_MAX_CURRENT);
        right_inverter_current_check = App_InRangeCheck_Create(
            GetRightInverterCurrent, RIGHT_INVERTER_MIN_CURRENT,
            RIGHT_INVERTER_MAX_CURRENT);
        energy_meter_current_check = App_InRangeCheck_Create(
            GetEnergyMeterCurrent, ENERGY_METER_MIN_CURRENT,
            ENERGY_METER_MAX_CURRENT);
        can_current_check = App_InRangeCheck_Create(
            GetCanCurrent, CAN_MIN_CURRENT, CAN_MAX_CURRENT);
        air_shutdown_current_check = App_InRangeCheck_Create(
            GetAirShutdownCurrent, AIR_SHDN_MIN_CURRENT, AIR_SHDN_MAX_CURRENT);
        heartbeat_monitor = App_SharedHeartbeatMonitor_Create(
            get_current_ms, DEFAULT_HEARTBEAT_TIMEOUT_PERIOD_MS,
            DEFAULT_HEARTBEAT_BOARDS_TO_CHECK, heartbeat_timeout_callback);
        rgb_led_sequence = App_SharedRgbLedSequence_Create(
            turn_on_red_led, turn_on_green_led, turn_on_blue_led);
        world = App_PdmWorld_Create(
            can_tx_interface, can_rx_interface, vbat_voltage_check,
            _24v_aux_voltage_check, _24v_acc_voltage_check, aux1_current_check,
            aux2_current_check, left_inverter_current_check,
            right_inverter_current_check, energy_meter_current_check,
            can_current_check, air_shutdown_current_check, heartbeat_monitor,
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
        ASSERT_TRUE(world != NULL);
        ASSERT_TRUE(state_machine != NULL);
        ASSERT_TRUE(can_tx_interface != NULL);
        ASSERT_TRUE(can_rx_interface != NULL);
        ASSERT_TRUE(vbat_voltage_check != NULL);
        ASSERT_TRUE(_24v_aux_voltage_check != NULL);
        ASSERT_TRUE(_24v_acc_voltage_check != NULL);
        ASSERT_TRUE(aux1_current_check != NULL);
        ASSERT_TRUE(aux2_current_check != NULL);
        ASSERT_TRUE(left_inverter_current_check != NULL);
        ASSERT_TRUE(right_inverter_current_check != NULL);
        ASSERT_TRUE(energy_meter_current_check != NULL);
        ASSERT_TRUE(can_current_check != NULL);
        ASSERT_TRUE(air_shutdown_current_check != NULL);
        ASSERT_TRUE(heartbeat_monitor != NULL);
        ASSERT_TRUE(rgb_led_sequence != NULL);

        App_PdmWorld_Destroy(world);
        App_SharedStateMachine_Destroy(state_machine);
        App_CanTx_Destroy(can_tx_interface);
        App_CanRx_Destroy(can_rx_interface);
        App_InRangeCheck_Destroy(vbat_voltage_check);
        App_InRangeCheck_Destroy(_24v_aux_voltage_check);
        App_InRangeCheck_Destroy(_24v_acc_voltage_check);
        App_InRangeCheck_Destroy(aux1_current_check);
        App_InRangeCheck_Destroy(aux2_current_check);
        App_InRangeCheck_Destroy(left_inverter_current_check);
        App_InRangeCheck_Destroy(right_inverter_current_check);
        App_InRangeCheck_Destroy(energy_meter_current_check);
        App_InRangeCheck_Destroy(can_current_check);
        App_InRangeCheck_Destroy(air_shutdown_current_check);
        App_SharedHeartbeatMonitor_Destroy(heartbeat_monitor);
        App_SharedRgbLedSequence_Destroy(rgb_led_sequence);

        world                        = NULL;
        state_machine                = NULL;
        can_tx_interface             = NULL;
        can_rx_interface             = NULL;
        vbat_voltage_check           = NULL;
        _24v_aux_voltage_check       = NULL;
        _24v_acc_voltage_check       = NULL;
        aux1_current_check           = NULL;
        aux2_current_check           = NULL;
        left_inverter_current_check  = NULL;
        right_inverter_current_check = NULL;
        energy_meter_current_check   = NULL;
        can_current_check            = NULL;
        air_shutdown_current_check   = NULL;
        heartbeat_monitor            = NULL;
        rgb_led_sequence             = NULL;
    }

    void SetInitialState(const struct State *const initial_state)
    {
        App_SharedStateMachine_Destroy(state_machine);
        state_machine = App_SharedStateMachine_Create(world, initial_state);
        ASSERT_TRUE(state_machine != NULL);
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
        float (*current_can_signal_getter)(struct PdmCanTxInterface *),
        uint8_t (*undercurrent_can_signal_getter)(struct PdmCanTxInterface *),
        uint8_t (*overcurrent_can_signal_getter)(struct PdmCanTxInterface *))
    {
        for (auto &state : GetAllStatesExceptForInit())
        {
            SetInitialState(state);

            // Normal Value
            fake_current = (min_current + max_current) / 2;
            App_SharedStateMachine_Tick(state_machine);
            EXPECT_EQ(
                fake_current, current_can_signal_getter(can_tx_interface));
            EXPECT_EQ(undercurrent_can_signal_getter(can_tx_interface), false);
            EXPECT_EQ(overcurrent_can_signal_getter(can_tx_interface), false);

            // Under-current
            fake_current = std::nextafter(
                min_current, std::numeric_limits<float>::lowest());
            App_SharedStateMachine_Tick(state_machine);
            EXPECT_EQ(
                fake_current, current_can_signal_getter(can_tx_interface));
            EXPECT_EQ(undercurrent_can_signal_getter(can_tx_interface), true);
            EXPECT_EQ(overcurrent_can_signal_getter(can_tx_interface), false);

            // Over-current
            fake_current =
                std::nextafter(max_current, std::numeric_limits<float>::max());
            App_SharedStateMachine_Tick(state_machine);
            EXPECT_EQ(
                fake_current, current_can_signal_getter(can_tx_interface));
            EXPECT_EQ(undercurrent_can_signal_getter(can_tx_interface), true);
            EXPECT_EQ(overcurrent_can_signal_getter(can_tx_interface), true);
        }
    }

    void CheckCurrentCanSignalsInInitState(
        float  min_current,
        float  max_current,
        float &fake_current,
        float (*current_can_signal_getter)(struct PdmCanTxInterface *),
        uint8_t (*undercurrent_can_signal_getter)(struct PdmCanTxInterface *),
        uint8_t (*overcurrent_can_signal_getter)(struct PdmCanTxInterface *))
    {
        // The INIT state is unique because the load switches are not ready
        // to be used yet so we expect NAN to be set
        SetInitialState(App_GetInitState());

        // Normal Value
        fake_current = (min_current + max_current) / 2;
        App_SharedStateMachine_Tick(state_machine);
        EXPECT_TRUE(isnanf(current_can_signal_getter(can_tx_interface)));
        EXPECT_EQ(undercurrent_can_signal_getter(can_tx_interface), false);
        EXPECT_EQ(overcurrent_can_signal_getter(can_tx_interface), false);

        // Under-current
        fake_current =
            std::nextafter(min_current, std::numeric_limits<float>::lowest());
        App_SharedStateMachine_Tick(state_machine);
        EXPECT_TRUE(isnanf(current_can_signal_getter(can_tx_interface)));
        EXPECT_EQ(undercurrent_can_signal_getter(can_tx_interface), false);
        EXPECT_EQ(overcurrent_can_signal_getter(can_tx_interface), false);

        // Over-current
        fake_current =
            std::nextafter(max_current, std::numeric_limits<float>::max());
        App_SharedStateMachine_Tick(state_machine);
        EXPECT_TRUE(isnanf(current_can_signal_getter(can_tx_interface)));
        EXPECT_EQ(undercurrent_can_signal_getter(can_tx_interface), false);
        EXPECT_EQ(overcurrent_can_signal_getter(can_tx_interface), false);
    }

    void CheckVoltageCanSignalsInAllStates(
        float  min_voltage,
        float  max_voltage,
        float &fake_voltage,
        float (*voltage_can_signal_getter)(struct PdmCanTxInterface *),
        uint8_t (*undervoltage_can_signal_getter)(struct PdmCanTxInterface *),
        uint8_t (*overvoltage_can_signal_getter)(struct PdmCanTxInterface *))
    {
        for (auto &state : GetAllStates())
        {
            SetInitialState(state);

            // Normal Value
            fake_voltage = (min_voltage + max_voltage) / 2;
            App_SharedStateMachine_Tick(state_machine);
            EXPECT_EQ(
                fake_voltage, voltage_can_signal_getter(can_tx_interface));
            EXPECT_EQ(undervoltage_can_signal_getter(can_tx_interface), false);
            EXPECT_EQ(overvoltage_can_signal_getter(can_tx_interface), false);

            // Under-voltage
            fake_voltage = std::nextafter(
                min_voltage, std::numeric_limits<float>::lowest());
            App_SharedStateMachine_Tick(state_machine);
            EXPECT_EQ(
                fake_voltage, voltage_can_signal_getter(can_tx_interface));
            EXPECT_EQ(undervoltage_can_signal_getter(can_tx_interface), true);
            EXPECT_EQ(overvoltage_can_signal_getter(can_tx_interface), false);

            // Over-voltage
            fake_voltage =
                std::nextafter(max_voltage, std::numeric_limits<float>::max());
            App_SharedStateMachine_Tick(state_machine);
            EXPECT_EQ(
                fake_voltage, voltage_can_signal_getter(can_tx_interface));
            EXPECT_EQ(undervoltage_can_signal_getter(can_tx_interface), true);
            EXPECT_EQ(overvoltage_can_signal_getter(can_tx_interface), true);
        }
    }

    struct World *            world;
    struct StateMachine *     state_machine;
    struct PdmCanTxInterface *can_tx_interface;
    struct PdmCanRxInterface *can_rx_interface;
    struct InRangeCheck *     vbat_voltage_check;
    struct InRangeCheck *     _24v_aux_voltage_check;
    struct InRangeCheck *     _24v_acc_voltage_check;
    struct InRangeCheck *     aux1_current_check;
    struct InRangeCheck *     aux2_current_check;
    struct InRangeCheck *     left_inverter_current_check;
    struct InRangeCheck *     right_inverter_current_check;
    struct InRangeCheck *     energy_meter_current_check;
    struct InRangeCheck *     can_current_check;
    struct InRangeCheck *     air_shutdown_current_check;
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
        App_CanTx_GetPeriodicSignal_UNDERVOLTAGE_VBAT,
        App_CanTx_GetPeriodicSignal_OVERVOLTAGE_VBAT);
}

TEST_F(PdmStateMachineTest, check_aux1_current_can_signals_in_non_init_states)
{
    CheckCurrentCanSignalsInNonInitStates(
        AUX1_MIN_CURRENT, AUX1_MAX_CURRENT, GetAux1Current_fake.return_val,
        App_CanTx_GetPeriodicSignal_AUXILIARY1_CURRENT,
        App_CanTx_GetPeriodicSignal_UNDERCURRENT_AUX_1,
        App_CanTx_GetPeriodicSignal_OVERCURRENT_AUX_1);
}

TEST_F(PdmStateMachineTest, check_aux1_current_can_signals_in_init_state)
{
    CheckCurrentCanSignalsInInitState(
        AUX1_MIN_CURRENT, AUX1_MAX_CURRENT, GetAux1Current_fake.return_val,
        App_CanTx_GetPeriodicSignal_AUXILIARY1_CURRENT,
        App_CanTx_GetPeriodicSignal_UNDERCURRENT_AUX_1,
        App_CanTx_GetPeriodicSignal_OVERCURRENT_AUX_1);
}
