#include <gtest/gtest.h>
#include "test/test_VCBase.hpp"
#include "app_loadswitches.hpp"
#include "vc_fakes.hpp"
#include "util_errorCodes.hpp"
#include "app_canTx.hpp"
#include "app_canRx.hpp"

class VCLoadSwitchTest : public VCBaseTest
{
};

// test braodcasting of channels
TEST_F(VCLoadSwitchTest, load_switch_broadcast)
{
    efuse_broadcast();
}
