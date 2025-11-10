#include "fsmMocks.hpp"
#include "test_FSMBase.hpp"

extern "C"
{
#include "app_canTx.h"
#include "app_canAlerts.h"
}

class BrakeModuleTest : public FSMBaseTest{
};

TEST_F(BrakeModuleTest, normal_BrakeConditions)
{
    fakes::io::brake::setBrakeActuated(true);
    fakes::io::brake::setFrontPressurePsi(30.0f);
    fakes::io::brake::setOCSC(false);

    LetTimePass(100);

    EXPECT_TRUE(app_canTx_FSM_BrakeActuated_get());
    EXPECT_EQ(app_canTx_FSM_FrontBrakePressure_get(), 30.0f);
    EXPECT_FALSE(app_canAlerts_FSM_Info_FrontBrakePressureOCSC_get());
    EXPECT_FALSE(app_canTx_FSM_Info_FrontBrakePressureOutOfRange_get());
}

TEST_F(BrakeModuleTest, faulty_BrakeConditions_1)
{
    fakes::io::brake::setBrakeActuated(false);
    fakes::io::brake::setFrontPressurePsi(3000.0f); //Positive Psi, larger than threshold psi
    fakes::io::brake::setOCSC(true);

    LetTimePass(100);

    EXPECT_FALSE(app_canTx_FSM_BrakeActuated_get());
    EXPECT_EQ(app_canTx_FSM_FrontBrakePressure_get(), 2500.0f);
    EXPECT_TRUE(app_canAlerts_FSM_Info_FrontBrakePressureOCSC_get());
    EXPECT_TRUE(app_canTx_FSM_Info_FrontBrakePressureOutOfRange_get());
}

TEST_F(BrakeModuleTest, faulty_BrakeConditions_2)
{
    fakes::io::brake::setBrakeActuated(false);
    fakes::io::brake::setFrontPressurePsi(-100.0f); // Negative Psi, smaller than threshold psi
    fakes::io::brake::setOCSC(true);

    LetTimePass(100);

    EXPECT_FALSE(app_canTx_FSM_BrakeActuated_get());
    EXPECT_EQ(app_canTx_FSM_FrontBrakePressure_get(), 2500.0f); // CAN only accepts positive values
    EXPECT_TRUE(app_canAlerts_FSM_Info_FrontBrakePressureOCSC_get());
    EXPECT_TRUE(app_canTx_FSM_Info_FrontBrakePressureOutOfRange_get());
}