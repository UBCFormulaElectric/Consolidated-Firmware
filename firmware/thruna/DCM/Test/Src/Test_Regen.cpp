#include "Test_Dcm.h"

extern "C"
{
#include "App_SharedStateMachine.h"
#include "App_SharedMacros.h"
#include "states/App_InitState.h"
#include "states/App_DriveState.h"
#include "Regen.h"
#include "App_CanTx.h"
#include "App_CanRx.h"
#include "App_CanUtils.h"
}

class TestRegen : public testing::Test
{
  protected:
    void SetUp() override { 
        App_CanTx_Init();
        App_CanRx_Init();
    }

    BinaryLed led = {};
};

TEST_F()
