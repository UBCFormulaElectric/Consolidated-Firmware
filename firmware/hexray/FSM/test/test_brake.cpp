#include "fsmMocks.hpp"
#include "test_FSMBase.hpp"

extern "C"
{
#include "app_canTx.h"
#include "app_canAlerts.h"
}

class BrakeModuleTest : public FSMBaseTest{
};

TEST_F(BrakeModuleTest, Normal_BrakeConditions_1)
{
    fakes::io::brake::setBrakeActuated(true);
    fakes::io::brake::setFrontPressurePsi(300.0f);
    fakes::io::brake::setOCSC(false);

    LetTimePass(100);

    EXPECT_TRUE(app_canTx_FSM_BrakeActuated_get());
    EXPECT_EQ(app_canTx_FSM_FrontBrakePressure_get(), 300);
    EXPECT_FALSE(app_canAlerts_FSM_Info_FrontBrakePressureOCSC_get());
    EXPECT_FALSE(app_canTx_FSM_Info_FrontBrakePressureOutOfRange_get());
}

TEST_F(BrakeModuleTest, Normal_BrakeConditions_2)
{
    fakes::io::brake::setBrakeActuated(true);
    fakes::io::brake::setFrontPressurePsi(999.4f); // Edge Case: 999.4psi
    fakes::io::brake::setOCSC(false);

    LetTimePass(100);

    EXPECT_TRUE(app_canTx_FSM_BrakeActuated_get());
    EXPECT_EQ(app_canTx_FSM_FrontBrakePressure_get(), 999);
    EXPECT_FALSE(app_canAlerts_FSM_Info_FrontBrakePressureOCSC_get());
    EXPECT_FALSE(app_canTx_FSM_Info_FrontBrakePressureOutOfRange_get());
}

TEST_F(BrakeModuleTest, Brake_Released)
{
    fakes::io::brake::setBrakeActuated(false);
    fakes::io::brake::setFrontPressurePsi(0.0f);
    fakes::io::brake::setOCSC(false);

    LetTimePass(100);

    EXPECT_FALSE(app_canTx_FSM_BrakeActuated_get());
    EXPECT_EQ(app_canTx_FSM_FrontBrakePressure_get(), 0);
    EXPECT_FALSE(app_canAlerts_FSM_Info_FrontBrakePressureOCSC_get());
    EXPECT_FALSE(app_canTx_FSM_Info_FrontBrakePressureOutOfRange_get());
}

TEST_F(BrakeModuleTest, PSIOutOfRange_OverFlow)
{
    fakes::io::brake::setBrakeActuated(true);
    fakes::io::brake::setFrontPressurePsi(2200.0f); //Positive Psi, WITHIN CAN threshold psi
    fakes::io::brake::setOCSC(false);

    LetTimePass(100);

    EXPECT_TRUE(app_canTx_FSM_BrakeActuated_get());
    EXPECT_EQ(app_canTx_FSM_FrontBrakePressure_get(), 2200);
    EXPECT_FALSE(app_canAlerts_FSM_Info_FrontBrakePressureOCSC_get());
    EXPECT_TRUE(app_canTx_FSM_Info_FrontBrakePressureOutOfRange_get());
}

TEST_F(BrakeModuleTest, PSIOutOfRange_CANOverFlow)
{
    fakes::io::brake::setBrakeActuated(true);
    fakes::io::brake::setFrontPressurePsi(3000.0f); //Positive Psi, LARGER than CAN threshold psi(2500)
    fakes::io::brake::setOCSC(false);

    LetTimePass(100);

    EXPECT_TRUE(app_canTx_FSM_BrakeActuated_get());
    EXPECT_EQ(app_canTx_FSM_FrontBrakePressure_get(), 2500);
    EXPECT_FALSE(app_canAlerts_FSM_Info_FrontBrakePressureOCSC_get());
    EXPECT_TRUE(app_canTx_FSM_Info_FrontBrakePressureOutOfRange_get());
}

TEST_F(BrakeModuleTest, PSIOutOfRange_CANUnderFlow)
{
    fakes::io::brake::setBrakeActuated(false);
    fakes::io::brake::setFrontPressurePsi(-100.0f); // Negative Psi, SMALLER than CAN threshold psi(0)
    fakes::io::brake::setOCSC(false);

    LetTimePass(100);

    EXPECT_FALSE(app_canTx_FSM_BrakeActuated_get());
    EXPECT_EQ(app_canTx_FSM_FrontBrakePressure_get(), 2500); // CAN only accepts positive values
    EXPECT_FALSE(app_canAlerts_FSM_Info_FrontBrakePressureOCSC_get());
    EXPECT_TRUE(app_canTx_FSM_Info_FrontBrakePressureOutOfRange_get());
}

TEST_F(BrakeModuleTest, Multiple_Faults_1)
{
    fakes::io::brake::setBrakeActuated(true);
    fakes::io::brake::setFrontPressurePsi(1500.0f); //  Brake Actuated, Over PSI
    fakes::io::brake::setOCSC(true);    //  Opened or Shorted Circuit

    LetTimePass(100);

    EXPECT_TRUE(app_canTx_FSM_BrakeActuated_get());
    EXPECT_EQ(app_canTx_FSM_FrontBrakePressure_get(), 1500);
    EXPECT_TRUE(app_canAlerts_FSM_Info_FrontBrakePressureOCSC_get());
    EXPECT_TRUE(app_canTx_FSM_Info_FrontBrakePressureOutOfRange_get());
}

TEST_F(BrakeModuleTest, Multiple_Faults_2)
{
    fakes::io::brake::setBrakeActuated(false);
    fakes::io::brake::setFrontPressurePsi(-100.0f); //  Brake NOT Actuated, Negative PSI
    fakes::io::brake::setOCSC(true);    //  Opened or Shorted Circuit

    LetTimePass(100);

    EXPECT_FALSE(app_canTx_FSM_BrakeActuated_get());
    EXPECT_EQ(app_canTx_FSM_FrontBrakePressure_get(), 2500);
    EXPECT_TRUE(app_canAlerts_FSM_Info_FrontBrakePressureOCSC_get());
    EXPECT_TRUE(app_canTx_FSM_Info_FrontBrakePressureOutOfRange_get());
}