#include "test_BMSBase.hpp"

#include "test_fakes.hpp"

extern "C"
{
#include "states/app_states.h"
#include "app_canRx.h"
#include "app_canTx.h"
#include "app_canAlerts.h"
#include "io_irs.h"
#include "io_time.h"
}

class BmsStateMachineTest : public BMSBaseTest
{
};

TEST_F(BmsStateMachineTest, init_proper_reset)
{
    app_stateMachine_setCurrentState(&drive_state);
    jobs_init();
    ASSERT_STATE_EQ(init_state);
}

TEST_F(BmsStateMachineTest, start_precharge_once_vc_bms_on_AND_irs_negative_closed)
{
    ASSERT_STATE_EQ(init_state);
    LetTimePass(100);
    ASSERT_STATE_EQ(init_state);

    app_canRx_VC_State_update(VC_BMS_ON_STATE);
    fakes::irs::setNegativeState(CONTACTOR_STATE_OPEN);
    LetTimePass(10);
    ASSERT_STATE_EQ(init_state);

    app_canRx_VC_State_update(VC_INVERTER_ON_STATE);
    fakes::irs::setNegativeState(CONTACTOR_STATE_CLOSED);
    LetTimePass(10);
    ASSERT_STATE_EQ(init_state);

    app_canRx_VC_State_update(VC_BMS_ON_STATE);
    fakes::irs::setNegativeState(CONTACTOR_STATE_CLOSED);
    LetTimePass(10);
    ASSERT_STATE_EQ(precharge_drive_state);

    app_canRx_VC_State_update(VC_INIT_STATE);
    LetTimePass(10);
    ASSERT_STATE_EQ(precharge_drive_state); // surely precharge state is stable for at least 20ms
}

TEST_F(BmsStateMachineTest, irs_negative_open_to_init_with_debounce)
{
    app_stateMachine_setCurrentState(&drive_state);
    fakes::irs::setNegativeState(CONTACTOR_STATE_CLOSED);
    LetTimePass(10);
    ASSERT_STATE_EQ(drive_state);

    fakes::irs::setNegativeState(CONTACTOR_STATE_OPEN);
    for (int i = 0; i <= 200; i += 10)
    {
        ASSERT_STATE_EQ(drive_state) << "Expected state: drive_state, but got: "
                                     << app_stateMachine_getCurrentState()->name
                                     << ", time = " << io_time_getCurrentMs();
        LetTimePass(10);
    }
    ASSERT_STATE_EQ(init_state);
}

TEST_F(BmsStateMachineTest, check_contactors_open_in_inert_states)
{
    io_irs_setPositive(CONTACTOR_STATE_CLOSED);
    app_stateMachine_setCurrentState(&fault_state);
    ASSERT_EQ(io_irs_positiveState(), CONTACTOR_STATE_OPEN);

    io_irs_setPositive(CONTACTOR_STATE_CLOSED);
    app_stateMachine_setCurrentState(&init_state);
    ASSERT_EQ(io_irs_positiveState(), CONTACTOR_STATE_OPEN);
}

TEST_F(BmsStateMachineTest, check_state_transition_from_fault_to_init_with_no_faults_set)
{
    app_canAlerts_BMS_Fault_TESTFAULT_set(true);
    LetTimePass(10);
    ASSERT_STATE_EQ(fault_state);

    app_canAlerts_BMS_Fault_TESTFAULT_set(false);
    ASSERT_FALSE(app_canAlerts_AnyBoardHasFault());
    LetTimePass(10);
    ASSERT_STATE_EQ(init_state);
}

TEST_F(BmsStateMachineTest, stays_in_fault_state_if_ir_negative_closes)
{
    app_stateMachine_setCurrentState(&fault_state);
    app_canAlerts_BMS_Fault_TESTFAULT_set(true);
    fakes::irs::setNegativeState(CONTACTOR_STATE_CLOSED);
    LetTimePass(100);
    ASSERT_STATE_EQ(fault_state);
}

TEST_F(BmsStateMachineTest, goes_to_init_state_and_broadcasts_imd_latch)
{
    app_stateMachine_setCurrentState(&drive_state);
    fakes::faultLatches::resetFaultLatch(&imd_ok_latch);
    fakes::irs::setNegativeState(CONTACTOR_STATE_CLOSED);
    LetTimePass(10);
    ASSERT_TRUE(app_canTx_BMS_ImdCurrentlyOk_get());
    ASSERT_TRUE(app_canTx_BMS_ImdLatchOk_get());
    ASSERT_STATE_EQ(drive_state);
    fakes::faultLatches::updateFaultLatch(&imd_ok_latch, FAULT_LATCH_FAULT);
    fakes::irs::setNegativeState(CONTACTOR_STATE_OPEN);
    LetTimePass(300);
    ASSERT_STATE_EQ(init_state);
    ASSERT_FALSE(app_canTx_BMS_ImdCurrentlyOk_get());
    ASSERT_FALSE(app_canTx_BMS_ImdLatchOk_get());
}

TEST_F(BmsStateMachineTest, goes_to_init_state_and_broadcasts_bmsok_latch)
{
    app_stateMachine_setCurrentState(&drive_state);
    fakes::faultLatches::resetFaultLatch(&bms_ok_latch);
    fakes::irs::setNegativeState(CONTACTOR_STATE_CLOSED);
    LetTimePass(10);
    ASSERT_TRUE(app_canTx_BMS_BmsCurrentlyOk_get());
    ASSERT_TRUE(app_canTx_BMS_BmsLatchOk_get());
    ASSERT_STATE_EQ(drive_state);
    fakes::faultLatches::updateFaultLatch(&bms_ok_latch, FAULT_LATCH_FAULT);
    fakes::irs::setNegativeState(CONTACTOR_STATE_OPEN);
    LetTimePass(300);
    ASSERT_STATE_EQ(init_state); // Since we have not set a fault, BMSCurrentlyOk should be true
    ASSERT_TRUE(app_canTx_BMS_BmsCurrentlyOk_get());
    ASSERT_FALSE(app_canTx_BMS_BmsLatchOk_get());
}

TEST_F(BmsStateMachineTest, goes_to_init_state_and_broadcasts_bspd_latch)
{
    app_stateMachine_setCurrentState(&drive_state);
    fakes::faultLatches::resetFaultLatch(&bspd_ok_latch);
    fakes::irs::setNegativeState(CONTACTOR_STATE_CLOSED);
    LetTimePass(10);
    ASSERT_TRUE(app_canTx_BMS_BspdCurrentlyOk_get());
    ASSERT_TRUE(app_canTx_BMS_BspdLatchOk_get());
    ASSERT_STATE_EQ(drive_state);
    fakes::faultLatches::updateFaultLatch(&bspd_ok_latch, FAULT_LATCH_FAULT);
    fakes::irs::setNegativeState(CONTACTOR_STATE_OPEN);
    LetTimePass(300);
    ASSERT_STATE_EQ(init_state);
    ASSERT_FALSE(app_canTx_BMS_BspdCurrentlyOk_get());
    ASSERT_FALSE(app_canTx_BMS_BspdLatchOk_get());
}

// fault tests probably can move to testFaults.cpp
TEST_F(BmsStateMachineTest, goes_to_fault_state_cell_over_voltage_fault)
{
    app_stateMachine_setCurrentState(&drive_state);
    std::array<std::array<float, CELLS_PER_SEGMENT>, NUM_SEGMENTS> cell_voltages_arr{};
    for (size_t seg = 0; seg < NUM_SEGMENTS; ++seg)
    {
        for (size_t cell = 0; cell < CELLS_PER_SEGMENT; ++cell)
        {
            cell_voltages_arr[seg][cell] = 4.1f;
        }
    }
    cell_voltages_arr[NUM_SEGMENTS - 1][CELLS_PER_SEGMENT - 1] = 4.21f; // last cell overvoltage
    io_ltc6813_startCellsAdcConversion();
    fakes::segments::setCellVoltages(cell_voltages_arr);
    fakes::irs::setNegativeState(CONTACTOR_STATE_CLOSED);
    LetTimePass(10010);
    ASSERT_STATE_EQ(fault_state);
    ASSERT_TRUE(app_canAlerts_BMS_Fault_CellOvervoltage_get());
    ASSERT_FALSE(app_canTx_BMS_BmsCurrentlyOk_get());
}

TEST_F(BmsStateMachineTest, goes_to_fault_state_cell_under_voltage_fault)
{
    app_stateMachine_setCurrentState(&drive_state);
    std::array<std::array<float, CELLS_PER_SEGMENT>, NUM_SEGMENTS> cell_voltages_arr{};
    for (size_t seg = 0; seg < NUM_SEGMENTS; ++seg)
    {
        for (size_t cell = 0; cell < CELLS_PER_SEGMENT; ++cell)
        {
            cell_voltages_arr[seg][cell] = 2.6f;
        }
    }
    cell_voltages_arr[NUM_SEGMENTS - 1][CELLS_PER_SEGMENT - 1] = 2.49f;
    io_ltc6813_startCellsAdcConversion();
    fakes::segments::setCellVoltages(cell_voltages_arr);
    fakes::irs::setNegativeState(CONTACTOR_STATE_CLOSED);
    LetTimePass(10010);
    ASSERT_STATE_EQ(fault_state);
    ASSERT_TRUE(app_canAlerts_BMS_Fault_CellUndervoltage_get());
    ASSERT_FALSE(app_canTx_BMS_BmsCurrentlyOk_get());
}

// TODO: Implement proper mocking of the cell temperatures with transfer functions for the aux regs
// TEST_F(BmsStateMachineTest, goes_to_state_cell_over_temp_fault)
// {
//     app_stateMachine_setCurrentState(&drive_state);
//     std::array<std::array<float, AUX_REGS_PER_SEGMENT>, NUM_SEGMENTS> cell_temps_arr{};
//     for (size_t seg = 0; seg < NUM_SEGMENTS; ++seg)
//     {
//         for (size_t cell = 0; cell < AUX_REGS_PER_SEGMENT; ++cell)
//         {
//             cell_temps_arr[seg][cell] = 50.0f;
//         }
//     }
//     cell_temps_arr[NUM_SEGMENTS - 1][AUX_REGS_PER_SEGMENT - 1] = 61.0f; // last cell over temperature
//     fakes::segments::setCellTemperatures(cell_temps_arr);
//     fakes::irs::setNegativeState(CONTACTOR_STATE_CLOSED);
//     LetTimePass(10010);
//     LetTimePass(1000);
//     ASSERT_STATE_EQ(fault_state);
//     ASSERT_TRUE(app_canAlerts_BMS_Fault_CellOvertemp_get());
//     ASSERT_FALSE(app_canTx_BMS_BmsCurrentlyOk_get());
// }

// precharge tests
// TODO set these values
static constexpr float undervoltage = 200.0f, target_voltage = 600.0f;
static constexpr int   too_fast_time = 20, just_good_time = 1220;

static constexpr int precharge_timeout = 3000, precharge_cooldown = 1500;
static constexpr int timing_tolerance = 500;

static constexpr int precharge_retries = 3;
TEST_F(BmsStateMachineTest, precharge_success_test)
{
    fakes::segments::setPackVoltageEvenly(target_voltage);
    fakes::irs::setNegativeState(CONTACTOR_STATE_CLOSED);
    fakes::tractiveSystem::setVoltage(0);
    app_stateMachine_setCurrentState(&precharge_drive_state);
    LetTimePass(10);

    for (int i = 0; i < just_good_time; i += 10)
    {
        ASSERT_STATE_EQ(precharge_drive_state);
        ASSERT_EQ(io_irs_prechargeState(), CONTACTOR_STATE_CLOSED);
        ASSERT_EQ(app_canTx_BMS_PrechargeRelay_get(), CONTACTOR_STATE_CLOSED);
        LetTimePass(10);
    }

    fakes::tractiveSystem::setVoltage(target_voltage);
    LetTimePass(10);
    ASSERT_STATE_EQ(drive_state);
    ASSERT_EQ(io_irs_prechargeState(), CONTACTOR_STATE_OPEN);
    ASSERT_EQ(app_canTx_BMS_PrechargeRelay_get(), CONTACTOR_STATE_OPEN);
}

TEST_F(BmsStateMachineTest, precharge_retry_test_and_undervoltage_rising_slowly)
{
    fakes::segments::setPackVoltageEvenly(target_voltage);
    fakes::irs::setNegativeState(CONTACTOR_STATE_CLOSED);
    fakes::tractiveSystem::setVoltage(undervoltage);
    app_stateMachine_setCurrentState(&precharge_drive_state);
    LetTimePass(10);

    for (int retry = 0; retry < precharge_retries; retry++)
    {
        int closed_time;
        for (closed_time = 0;
             io_irs_prechargeState() == CONTACTOR_STATE_CLOSED && closed_time < precharge_timeout + timing_tolerance;
             closed_time += 10)
        {
            ASSERT_STATE_EQ(precharge_drive_state);
            ASSERT_EQ(app_canTx_BMS_PrechargeRelay_get(), CONTACTOR_STATE_CLOSED);
            LetTimePass(10);
        }
        ASSERT_NEAR(closed_time, precharge_timeout, 100)
            << "Expected precharge to be closed for approximately " << precharge_timeout << "ms, but was "
            << closed_time << "ms, time=" << io_time_getCurrentMs();

        // cooldown
        if (retry == precharge_retries - 1)
            break;
        int open_time;
        for (open_time = 0;
             io_irs_prechargeState() == CONTACTOR_STATE_OPEN && open_time < precharge_cooldown + timing_tolerance;
             open_time += 10)
        {
            ASSERT_EQ(app_canTx_BMS_PrechargeRelay_get(), CONTACTOR_STATE_OPEN);
            LetTimePass(10);
        }
        ASSERT_NEAR(open_time, precharge_cooldown, 100) << "Expected precharge to be open for approximately "
                                                        << precharge_cooldown << "ms, but was " << open_time << "ms"
                                                        << ", time = " << io_time_getCurrentMs();
    }

    ASSERT_STATE_EQ(precharge_latch_state);
}

TEST_F(BmsStateMachineTest, precharge_rising_too_quickly)
{
    fakes::segments::setPackVoltageEvenly(target_voltage);
    fakes::irs::setNegativeState(CONTACTOR_STATE_CLOSED);
    fakes::tractiveSystem::setVoltage(0.0f);
    app_stateMachine_setCurrentState(&precharge_drive_state);
    LetTimePass(10);

    for (int i = 0; i < too_fast_time; i += 10)
    {
        ASSERT_STATE_EQ(precharge_drive_state);
        ASSERT_EQ(io_irs_prechargeState(), CONTACTOR_STATE_CLOSED);
        ASSERT_EQ(app_canTx_BMS_PrechargeRelay_get(), CONTACTOR_STATE_CLOSED);
        LetTimePass(10);
    }

    fakes::tractiveSystem::setVoltage(target_voltage);
    LetTimePass(10);
    ASSERT_EQ(io_irs_prechargeState(), CONTACTOR_STATE_OPEN);
    ASSERT_EQ(app_canTx_BMS_PrechargeRelay_get(), CONTACTOR_STATE_OPEN);
    // we presume that it is in the retry phase as described above now
}

// charging tests
TEST_F(BmsStateMachineTest, faults_after_shutdown_loop_activates_while_charging) {}
TEST_F(BmsStateMachineTest, stops_charging_and_faults_if_charger_disconnects_in_charge_state) {}
TEST_F(BmsStateMachineTest, charger_connected_no_can_msg_init_state) {}
TEST_F(BmsStateMachineTest, charger_connected_can_msg_init_state) {}
TEST_F(BmsStateMachineTest, no_charger_connected_missing_hb_init_state) {}
TEST_F(BmsStateMachineTest, charger_connected_successful_precharge_stays) {}
TEST_F(BmsStateMachineTest, keeps_charging_with_no_interrupts) {}
TEST_F(BmsStateMachineTest, stops_charging_after_false_charging_msg) {}
TEST_F(BmsStateMachineTest, fault_from_charger_fault) {}