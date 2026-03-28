#include "fsmMocks.hpp"
#include "test_FSMBase.hpp"
#include "app_canTx.hpp"

// TEST NOT COMPLETED:
// Test values are not accurate, App logic is not yet finalized

class AppsModuleTest : public FSMBaseTest
{
};

/*
1) Normal Operation
2)
*/

TEST_F(AppsModuleTest, Normal_AppsConditions)
{
    fakes::io::apps::setPrimaryPressure(30.0f);
    fakes::io::apps::setPrimaryOCSC(false);
    fakes::io::apps::setSecondaryPressure(25.0f);
    fakes::io::apps::setSecondaryOCSC(false);

    LetTimePass(100);

    EXPECT_FALSE(app::can_tx::FSM_Warning_PappsOCSC_get());
    EXPECT_FALSE(app::can_tx::FSM_Warning_SappsOCSC_get());
    EXPECT_FALSE(app::can_tx::FSM_Warning_AppsDisagreement_get());

    EXPECT_EQ(app::can_tx::FSM_PappsRawPedalPercentage_get(), 30.0f);
    EXPECT_EQ(app::can_tx::FSM_SappsRawPedalPercentage_get(), 25.0f);
    EXPECT_EQ(app::can_tx::FSM_PappsMappedPedalPercentage_get(), 30.0f);
    EXPECT_EQ(app::can_tx::FSM_SappsMappedPedalPercentage_get(), 25.0f);
}

TEST_F(AppsModuleTest, OCSC_1)
{
    // Both Apps are OCSC
    fakes::io::apps::setPrimaryPressure(30.0f);
    fakes::io::apps::setPrimaryOCSC(true);
    fakes::io::apps::setSecondaryPressure(25.0f);
    fakes::io::apps::setSecondaryOCSC(true);

    LetTimePass(100);

    EXPECT_TRUE(app::can_tx::FSM_Warning_PappsOCSC_get());
    EXPECT_TRUE(app::can_tx::FSM_Warning_SappsOCSC_get());
    EXPECT_FALSE(app::can_tx::FSM_Warning_AppsDisagreement_get());

    EXPECT_EQ(app::can_tx::FSM_PappsRawPedalPercentage_get(), 30.0f);
    EXPECT_EQ(app::can_tx::FSM_SappsRawPedalPercentage_get(), 25.0f);
    EXPECT_EQ(app::can_tx::FSM_PappsMappedPedalPercentage_get(), 0.0f);
    EXPECT_EQ(app::can_tx::FSM_SappsMappedPedalPercentage_get(), 0.0f);
}

TEST_F(AppsModuleTest, OCSC_2)
{
    // One of the Apps is OCSC
    fakes::io::apps::setPrimaryPressure(30.0f);
    fakes::io::apps::setPrimaryOCSC(true);
    fakes::io::apps::setSecondaryPressure(25.0f);
    fakes::io::apps::setSecondaryOCSC(false);

    LetTimePass(100);

    EXPECT_TRUE(app::can_tx::FSM_Warning_PappsOCSC_get());
    EXPECT_FALSE(app::can_tx::FSM_Warning_SappsOCSC_get());
    EXPECT_FALSE(app::can_tx::FSM_Warning_AppsDisagreement_get());

    EXPECT_EQ(app::can_tx::FSM_PappsRawPedalPercentage_get(), 30.0f);
    EXPECT_EQ(app::can_tx::FSM_SappsRawPedalPercentage_get(), 25.0f);
    EXPECT_EQ(app::can_tx::FSM_PappsMappedPedalPercentage_get(), 0.0f);
    EXPECT_EQ(app::can_tx::FSM_SappsMappedPedalPercentage_get(), 0.0f);
}

TEST_F(AppsModuleTest, AppsDisagreement_1)
{
    // Primary > Secondary Apps
    fakes::io::apps::setPrimaryPressure(30.0f);
    fakes::io::apps::setPrimaryOCSC(false);
    fakes::io::apps::setSecondaryPressure(10.0f);
    fakes::io::apps::setSecondaryOCSC(false);

    LetTimePass(100);

    EXPECT_FALSE(app::can_tx::FSM_Warning_PappsOCSC_get());
    EXPECT_FALSE(app::can_tx::FSM_Warning_SappsOCSC_get());
    EXPECT_TRUE(app::can_tx::FSM_Warning_AppsDisagreement_get());

    EXPECT_EQ(app::can_tx::FSM_PappsRawPedalPercentage_get(), 30.0f);
    EXPECT_EQ(app::can_tx::FSM_SappsRawPedalPercentage_get(), 10.0f);
    EXPECT_EQ(app::can_tx::FSM_PappsMappedPedalPercentage_get(), 0.0f);
    EXPECT_EQ(app::can_tx::FSM_SappsMappedPedalPercentage_get(), 0.0f);
}

TEST_F(AppsModuleTest, AppsDisagreement_2)
{
    // Primary > Secondary Apps
    fakes::io::apps::setPrimaryPressure(10.0f);
    fakes::io::apps::setPrimaryOCSC(false);
    fakes::io::apps::setSecondaryPressure(30.0f);
    fakes::io::apps::setSecondaryOCSC(false);

    LetTimePass(100);

    EXPECT_FALSE(app::can_tx::FSM_Warning_PappsOCSC_get());
    EXPECT_FALSE(app::can_tx::FSM_Warning_SappsOCSC_get());
    EXPECT_TRUE(app::can_tx::FSM_Warning_AppsDisagreement_get());

    EXPECT_EQ(app::can_tx::FSM_PappsRawPedalPercentage_get(), 10.0f);
    EXPECT_EQ(app::can_tx::FSM_SappsRawPedalPercentage_get(), 30.0f);
    EXPECT_EQ(app::can_tx::FSM_PappsMappedPedalPercentage_get(), 0.0f);
    EXPECT_EQ(app::can_tx::FSM_SappsMappedPedalPercentage_get(), 0.0f);
}

TEST_F(AppsModuleTest, AppsDisagreement_CANOverFlow)
{
    // Primary > Secondary Apps, CAN Overflow
    fakes::io::apps::setPrimaryPressure(120.0f);
    fakes::io::apps::setPrimaryOCSC(false);
    fakes::io::apps::setSecondaryPressure(100.0f);
    fakes::io::apps::setSecondaryOCSC(false);

    LetTimePass(100);

    EXPECT_FALSE(app::can_tx::FSM_Warning_PappsOCSC_get());
    EXPECT_FALSE(app::can_tx::FSM_Warning_SappsOCSC_get());
    EXPECT_TRUE(app::can_tx::FSM_Warning_AppsDisagreement_get());

    EXPECT_EQ(app::can_tx::FSM_PappsRawPedalPercentage_get(), 100.0f);
    EXPECT_EQ(app::can_tx::FSM_SappsRawPedalPercentage_get(), 100.0f);
    EXPECT_EQ(app::can_tx::FSM_PappsMappedPedalPercentage_get(), 0.0f);
    EXPECT_EQ(app::can_tx::FSM_SappsMappedPedalPercentage_get(), 0.0f);
}

TEST_F(AppsModuleTest, AppsDisagreement_CANUnderFlow)
{
    // Negative Value; Primary < Secondary Apps; CAN Underflow
    fakes::io::apps::setPrimaryPressure(-30.0f);
    fakes::io::apps::setPrimaryOCSC(false);
    fakes::io::apps::setSecondaryPressure(-10.0f);
    fakes::io::apps::setSecondaryOCSC(false);

    LetTimePass(100);

    EXPECT_FALSE(app::can_tx::FSM_Warning_PappsOCSC_get());
    EXPECT_FALSE(app::can_tx::FSM_Warning_SappsOCSC_get());
    EXPECT_TRUE(app::can_tx::FSM_Warning_AppsDisagreement_get());

    EXPECT_EQ(app::can_tx::FSM_PappsRawPedalPercentage_get(), 0.0f);
    EXPECT_EQ(app::can_tx::FSM_SappsRawPedalPercentage_get(), 0.0f);
    EXPECT_EQ(app::can_tx::FSM_PappsMappedPedalPercentage_get(), 0.0f);
    EXPECT_EQ(app::can_tx::FSM_SappsMappedPedalPercentage_get(), 0.0f);
}

TEST_F(AppsModuleTest, CANOverFlow)
{
    // Primary > Secondary Apps, CAN Overflow
    fakes::io::apps::setPrimaryPressure(105.0f);
    fakes::io::apps::setPrimaryOCSC(false);
    fakes::io::apps::setSecondaryPressure(95.0f);
    fakes::io::apps::setSecondaryOCSC(false);

    LetTimePass(100);

    EXPECT_FALSE(app::can_tx::FSM_Warning_PappsOCSC_get());
    EXPECT_FALSE(app::can_tx::FSM_Warning_SappsOCSC_get());
    EXPECT_FALSE(app::can_tx::FSM_Warning_AppsDisagreement_get());

    EXPECT_EQ(app::can_tx::FSM_PappsRawPedalPercentage_get(), 100.0f);
    EXPECT_EQ(app::can_tx::FSM_SappsRawPedalPercentage_get(), 95.0f);
    EXPECT_EQ(app::can_tx::FSM_PappsMappedPedalPercentage_get(), 100.0f);
    EXPECT_EQ(app::can_tx::FSM_SappsMappedPedalPercentage_get(), 95.0f);
}

TEST_F(AppsModuleTest, CANUnderFlow)
{
    // Primary < Secondary Apps, CAN Underflow
    fakes::io::apps::setPrimaryPressure(-5.0f);
    fakes::io::apps::setPrimaryOCSC(false);
    fakes::io::apps::setSecondaryPressure(5.0f);
    fakes::io::apps::setSecondaryOCSC(false);

    LetTimePass(100);

    EXPECT_FALSE(app::can_tx::FSM_Warning_PappsOCSC_get());
    EXPECT_FALSE(app::can_tx::FSM_Warning_SappsOCSC_get());
    EXPECT_FALSE(app::can_tx::FSM_Warning_AppsDisagreement_get());

    EXPECT_EQ(app::can_tx::FSM_PappsRawPedalPercentage_get(), 0.0f);
    EXPECT_EQ(app::can_tx::FSM_SappsRawPedalPercentage_get(), 5.0f);
    EXPECT_EQ(app::can_tx::FSM_PappsMappedPedalPercentage_get(), 0.0f);
    EXPECT_EQ(app::can_tx::FSM_SappsMappedPedalPercentage_get(), 5.0f);
}

TEST_F(AppsModuleTest, Multiple_Faults_1)
{
    // AppsDisagreement; OCSC
    fakes::io::apps::setPrimaryPressure(30.0f);
    fakes::io::apps::setPrimaryOCSC(true);
    fakes::io::apps::setSecondaryPressure(50.0f);
    fakes::io::apps::setSecondaryOCSC(false);

    LetTimePass(100);

    EXPECT_TRUE(app::can_tx::FSM_Warning_PappsOCSC_get());
    EXPECT_FALSE(app::can_tx::FSM_Warning_SappsOCSC_get());
    EXPECT_TRUE(app::can_tx::FSM_Warning_AppsDisagreement_get());

    EXPECT_EQ(app::can_tx::FSM_PappsRawPedalPercentage_get(), 30.0f);
    EXPECT_EQ(app::can_tx::FSM_SappsRawPedalPercentage_get(), 50.0f);
    EXPECT_EQ(app::can_tx::FSM_PappsMappedPedalPercentage_get(), 0.0f);
    EXPECT_EQ(app::can_tx::FSM_SappsMappedPedalPercentage_get(), 0.0f);
}

TEST_F(AppsModuleTest, Multiple_Faults_2)
{
    // AppsDisagreement; OCSC
    fakes::io::apps::setPrimaryPressure(100.0f);
    fakes::io::apps::setPrimaryOCSC(true);
    fakes::io::apps::setSecondaryPressure(0.0f);
    fakes::io::apps::setSecondaryOCSC(true);

    LetTimePass(100);

    EXPECT_TRUE(app::can_tx::FSM_Warning_PappsOCSC_get());
    EXPECT_TRUE(app::can_tx::FSM_Warning_SappsOCSC_get());
    EXPECT_TRUE(app::can_tx::FSM_Warning_AppsDisagreement_get());

    EXPECT_EQ(app::can_tx::FSM_PappsRawPedalPercentage_get(), 100.0f);
    EXPECT_EQ(app::can_tx::FSM_SappsRawPedalPercentage_get(), 0.0f);
    EXPECT_EQ(app::can_tx::FSM_PappsMappedPedalPercentage_get(), 0.0f);
    EXPECT_EQ(app::can_tx::FSM_SappsMappedPedalPercentage_get(), 0.0f);
}

TEST_F(AppsModuleTest, Multiple_Faults_3)
{
    // AppsDisagreement; OCSC; CAN Overflow
    fakes::io::apps::setPrimaryPressure(200.0f);
    fakes::io::apps::setPrimaryOCSC(false);
    fakes::io::apps::setSecondaryPressure(50.0f);
    fakes::io::apps::setSecondaryOCSC(true);

    LetTimePass(100);

    EXPECT_FALSE(app::can_tx::FSM_Warning_PappsOCSC_get());
    EXPECT_TRUE(app::can_tx::FSM_Warning_SappsOCSC_get());
    EXPECT_TRUE(app::can_tx::FSM_Warning_AppsDisagreement_get());

    EXPECT_EQ(app::can_tx::FSM_PappsRawPedalPercentage_get(), 100.0f);
    EXPECT_EQ(app::can_tx::FSM_SappsRawPedalPercentage_get(), 50.0f);
    EXPECT_EQ(app::can_tx::FSM_PappsMappedPedalPercentage_get(), 0.0f);
    EXPECT_EQ(app::can_tx::FSM_SappsMappedPedalPercentage_get(), 0.0f);
}

TEST_F(AppsModuleTest, Multiple_Faults_4)
{
    // AppsDisagreement; OCSC; CAN UnderFlow
    fakes::io::apps::setPrimaryPressure(-50.0f);
    fakes::io::apps::setPrimaryOCSC(true);
    fakes::io::apps::setSecondaryPressure(10.0f);
    fakes::io::apps::setSecondaryOCSC(false);

    LetTimePass(100);

    EXPECT_TRUE(app::can_tx::FSM_Warning_PappsOCSC_get());
    EXPECT_FALSE(app::can_tx::FSM_Warning_SappsOCSC_get());
    EXPECT_TRUE(app::can_tx::FSM_Warning_AppsDisagreement_get());

    EXPECT_EQ(app::can_tx::FSM_PappsRawPedalPercentage_get(), 0.0f);
    EXPECT_EQ(app::can_tx::FSM_SappsRawPedalPercentage_get(), 10.0f);
    EXPECT_EQ(app::can_tx::FSM_PappsMappedPedalPercentage_get(), 0.0f);
    EXPECT_EQ(app::can_tx::FSM_SappsMappedPedalPercentage_get(), 0.0f);
}