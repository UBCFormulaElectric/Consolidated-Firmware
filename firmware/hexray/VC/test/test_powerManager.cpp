#include <gtest/gtest.h>
#include "test/test_VCBase.hpp"
#include "app_powerManager.hpp"
#include "vc_fakes.hpp"
#include "util_errorCodes.hpp"
#include "app_canTx.hpp"
#include "app_canRx.hpp"

class VCPowerManagerTest : public VCBaseTest
{
};

// checking the config is updated correctly
TEST_F(VCPowerManagerTest, update_config_test) {}
// testing the enabling and disabling of efuses
TEST_F(VCPowerManagerTest, efuse_control_test) {}
