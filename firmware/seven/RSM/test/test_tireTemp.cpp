#include "test/test_RSMBase.hpp"
#include "test_fakes.hpp"
#include <gtest/gtest.h>
#include "app_canTx.hpp"

class RSMTireTempTest : public RSMBaseTest
{
};

TEST_F(RSMTireTempTest, FindTireTemp)
{
    fakes::io::tireTemp::set(50);

    LetTimePass(100);

    EXPECT_EQ(50, app::can_tx::RSM_TireTemperature_get());
    EXPECT_FALSE(app::can_tx::RSM_Info_TireTemperatureOutOfRange_get());
}

TEST_F(RSMTireTempTest, tireTempUpperOutOfRange)
{
    fakes::io::tireTemp::set(201);

    LetTimePass(100);

    EXPECT_EQ(0, app::can_tx::RSM_TireTemperature_get());
    EXPECT_TRUE(app::can_tx::RSM_Info_TireTemperatureOutOfRange_get());
}

TEST_F(RSMTireTempTest, tireTempLowerOutOfRange)
{
    fakes::io::tireTemp::set(-21);

    LetTimePass(100);

    EXPECT_EQ(0, app::can_tx::RSM_TireTemperature_get());
    EXPECT_TRUE(app::can_tx::RSM_Info_TireTemperatureOutOfRange_get());
}

TEST_F(RSMTireTempTest, tireTempinvalidToValid)
{
    fakes::io::tireTemp::set(-21);

    LetTimePass(100);

    fakes::io::tireTemp::set(25);

    LetTimePass(100);

    EXPECT_EQ(25, app::can_tx::RSM_TireTemperature_get());
    EXPECT_FALSE(app::can_tx::RSM_Info_TireTemperatureOutOfRange_get());
}