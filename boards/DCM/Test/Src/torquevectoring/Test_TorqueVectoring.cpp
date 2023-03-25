//
// Created by formulae on 25/03/23.
//
/**
 * Notes:
 * extern "C": specifies C linkage for functions to compiler
 *
 *
 *
*/

#include "Test_Dcm.h"
#include "App_SharedMacros.h"
#include "App_CanRx.h"

extern "C"
{
#include "torquevectoring/App_PowerLimiting.h"
#include "torquevectoring/App_ActiveDifferential.h"
#include "torquevectoring/App_TorqueVectoringConstants.h"
#include "torquevectoring/App_TorqueVectoring.h"
#include "math.h"
}
/**
 * Define SetUp and TearDown to be called before every TEST_F call
*/
class TorqueVectoringTest: public testing::Test
{
protected:
    //TODO: Find out if this is necessary if no IO functions are called.
    void SetUp() override
    {
        return;
    }
    void TearDown() override
    {
        return;
    }
};

TEST_F(TorqueVectoringTest, accelerator_pedal_not_pressed){
    App_CanRx_FSM_Apps_PappsMappedPedalPercentage_Update(0.0);
    App_CanRx_FSM_Wheels_LeftWheelSpeed_Update(50.0);
    App_CanRx_FSM_Wheels_RightWheelSpeed_Update(50.0);
    App_CanRx_INVL_MotorPositionInfo_MotorSpeed_Update(135);
    App_CanRx_INVR_MotorPositionInfo_MotorSpeed_Update(135);
    App_CanRx_BMS_TractiveSystem_TsCurrent_Update(100);
    App_CanRx_BMS_TractiveSystem_TsVoltage_Update(390);
    App_CanRx_INVL_Temperatures3_MotorTemperature_Update(50);
    App_CanRx_INVR_Temperatures3_MotorTemperature_Update(50);
    App_CanRx_BMS_AvailablePower_AvailablePower_Update(50);
    App_CanRx_FSM_Steering_SteeringAngle_Update(30);

    App_TorqueVectoring_Run();
//check the IO functions are being called the proper amount of times

}