#include <gtest/gtest.h>

#include "ecuTestBase.hpp"
#include "app_precharge.hpp"
#include "app_segments.hpp"
#include "app_canAlerts.hpp"
#include "test_fakes.hpp"

using namespace app::can_utils;
using namespace app::precharge;

//TO DO: CAN messages + JSON config

class BmsPrechargeTest : public EcuTestBase
{
  protected:
    // Drives precharge into a start precondition: irs closed, TS voltage at 0, pack voltage 600V, precharge for charging
    // Individual tests override whichever knobs they care about.

    bool for_charging = false;

    void StartOfPrecharge(bool charging)
    {
        for_charging = charging;
        fakes::irs::setNegativeState(ContactorState::CONTACTOR_STATE_CLOSED);
        fakes::ts::setTsVoltage(0.0f);
        fakes::segments::setPackVoltage(600.0f);
        init();
        restart();
        poll(for_charging);
    }
};


//note: expected timing is 2700 ms base on RC curve. Lower bound timer is 50% of this, upper bound is 50% of this.

// not charging, upper bound timer expires (EXPIRED), Vts < Vthreshold (Vpack * 0.9) - FAILED
TEST_F(BmsPrechargeTest, not_charging_ts_voltage_rising_slowly)
{
    StartOfPrecharge(false);
    LetTimePass(PRECHARGE_COMPLETION_UPPER_BOUND - 10);
    fakes::ts::setVoltage(500);
    LetTimePass(20);
    ASSERT_EQ(poll(false), State::FAILED);
}

// Charging Upper bound timer expires (EXPIRED), Vts < Vthreshold (Vpack * 0.9) - FAILED
TEST_F(BmsPrechargeTest, charging_ts_voltage_rising_slowly)
{
    StartOfPrecharge(true);
    LetTimePass(PRECHARGE_COMPLETION_UPPER_BOUND - 10);
    fakes::ts::setVoltage(500);
    LetTimePass(20);
    ASSERT_EQ(poll(true), State::FAILED);
}

//precharge not for charging, ts voltage rising too quickly (lower bound timer RUNNING) - FAILED
TEST_F(BmsPrechargeTest, not_charging_ts_voltage_rising_quickly)
{
    StartOfPrecharge(false);
    LetTimePass(PRECHARGE_COMPLETION_LOWER_BOUND - 500);
    fakes::ts::setVoltage(570);
    LetTimePass(10);
    ASSERT_EQ(poll(false), State::FAILED);
}

//precharge for charging, ts voltage rising too quickly (lower bound timer RUNNING) - SUCCESS
TEST_F(BmsPrechargeTest, charging_ts_voltage_rising_quickly)
{
    StartOfPrecharge(false);
    LetTimePass(PRECHARGE_COMPLETION_LOWER_BOUND - 500);
    fakes::ts::setVoltage(550);
    LetTimePass(10);
    ASSERT_EQ(poll(true), State::SUCCESS);  
}



//AIR negative open - FAILED
TEST_F(BmsPrechargeTest, air_negative_open)
{
    StartOfPrecharge(false);
    fakes::irs::setNegativeState(ContactorState::CONTACTOR_STATE_OPEN);
    LetTimePass(10);
    ASSERT_EQ(poll(false), State::FAILED);
}

//ts voltage < threshold voltage, lower bound timer hasn't expired - RUNNING
TEST_F(BmsPrechargeTest, ts_under_threshold_and_running_lower_timer)
{
    StartOfPrecharge(false);
    LetTimePass(PRECHARGE_COMPLETION_LOWER_BOUND - 500);
    fakes::ts::setVoltage(500);
    LetTimePass(10);
    ASSERT_EQ(poll(false), State::RUNNING);
}

//ts voltage < threshold voltage, upper bound timer hasn't expired - RUNNING
TEST_F(BmsPrechargeTest, ts_under_threshold_and_running_upper_timer)
{
    StartOfPrecharge(false);
    LetTimePass(PRECHARGE_COMPLETION_UPPER_BOUND - 500);
    fakes::ts::setVoltage(500);
    LetTimePass(10);
    ASSERT_EQ(poll(false), State::RUNNING);
}

// not charge, ts voltage > threshold voltage, low bound < timer < upper bound - SUCCESS
TEST_F(BmsPrechargeTest, ts_>_threshold_and_running_upper_timer)
{
    StartOfPrecharge(false);
    LetTimePass(PRECHARGE_COMPLETION_LOWER_BOUND + 500);
    fakes::ts::setVoltage(5700);
    LetTimePass(10);
    ASSERT_EQ(poll(false), State::SUCCESS);
}

// Upper bound passed 5 times, goes into crit failure - FAILED_CRITICAL
TEST_F(BmsPrechargeTest, critical_failure)
{
    StartOfPrecharge(false);
    fakes::ts::setVoltage(0);
    LetTimePass(PRECHARGE_COMPLETION_UPPER_BOUND * 5 + 2000);
    ASSERT_EQ(poll(false), State::FAILED_CRITICAL);
}


