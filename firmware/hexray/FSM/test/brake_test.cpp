#include "gtest/gtest.h"
#include "testMocks.hpp"
#include "jobs.hpp"
#include "test_FSMBase.hpp"

extern "C" {
    #include "app_canTx.h"
    #include "app_canAlerts.h"
}

#include <cmath>

class BrakeModuleTest : public FSMBaseTest
{
  protected:
    void SetUp() override
    {
        //FSMBaseTest::SetUp();
    }
};


void wait(int ms) {
    for(int i = 0; i < ms; i++) {
        if(i % 10 == 0) {
            jobs_run100Hz_tick();
        }
        //jobs_run1kHz_tick(); For future task??
    }
}

TEST_F(BrakeModuleTest, braketest1)
{

    fakes::io_brake::setBrakeActuated(true);
    
    wait(100);

    EXPECT_TRUE(app_anTx_FSM_BrakeActuated_get());
    // EXPECT_FALSE(app_canTx_VC_PumpFailure_get());
}