#include "test/test_RSMBase.hpp"
#include "test_fakes.hpp"
#include <gtest/gtest.h>

extern "C"
{
#include "app_canTx.h"
}

class RSMTireTempTest : public RSMBaseTest
{
};

TEST_F(RSMTireTempTest, FindTireTemp)
{
    fakes::io::tireTemp::set(50);

    LetTimePass(100);

    EXPECT_EQ(50, app_canTx_RSM_TireTemperature_get());
    EXPECT_FALSE(app_canTx_RSM_Info_TireTemperatureOutOfRange_get());
}
