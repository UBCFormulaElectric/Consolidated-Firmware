#include "Test_Bms.h"

DEFINE_FFF_GLOBALS;
//
// namespace ChargeTest
//{
//    FAKE_VALUE_FUNC(float, get_ts_current);
//    FAKE_VALUE_FUNC(bool, is_charger_connected);
//    FAKE_VALUE_FUNC(bool, is_air_negative_closed);
//    FAKE_VALUE_FUNC(bool, is_air_positive_closed);
//    FAKE_VALUE_FUNC(bool, has_charging_completed);
//    FAKE_VALUE_FUNC(bool, has_external_shutdown_occured);
//    FAKE_VALUE_FUNC(bool, charge_over_can);
//    FAKE_VOID_FUNC(disable_charge_can_message);
//    FAKE_VOID_FUNC(enable_charger);
//    FAKE_VOID_FUNC(disable_charger);
//    FAKE_VOID_FUNC(open_air_positive);
//    FAKE_VOID_FUNC(close_air_positive);
//    FAKE_VOID_FUNC(enable_pre_charge);
//    FAKE_VOID_FUNC(disable_pre_charge);
//
//    class BmsChargeTest : public BaseStateMachineTest
//    {
//    protected:
//        void Setup() override
//        {
//
//        }
//        void Teardown() override
//        {
//
//        }
//        void SetInitialState(const struct State *const initial_state)
//        {
//
//        }
//        std::vector<const struct State *> GetAllStates(void)
//        {
//
//        }
//    };
//
//    TEST_F(BmsChargeTest, check_if_system_stays_in_init)
//    {
//
//    }
//
//    TEST_F(BmsChargeTest, check_if_system_enters_precharge_with_canmsg_and_charger_connected)
//    {
//
//    }
//
//    TEST_F(BmsChargeTest, check_if_system_enters_charge_state_after_precharge){
//
//    }
//
//    TEST_F(BmsChargeTest, check_if_system_exits_charge_state_given_false_canmsg){
//
//    }
//
//    TEST_F(BmsChargeTest, check_if_system_exits_charge_state_given_charger_disconnected){
//
//    }
//
//    TEST_F(BmsChargeTest, check_if_system_exits_charge_state_given_external_shutdown){
//
//    }
//
//    TEST_F(BmsChargeTest, check_if_system_exists_charge_state_given_charging_is_complete){
//
//    }
//}