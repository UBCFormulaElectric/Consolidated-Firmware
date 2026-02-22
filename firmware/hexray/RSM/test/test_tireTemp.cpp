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
