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
TEST_F(VCPowerManagerTest, update_config_test)
{
    app::powerManager::init();

    app::powerManager::PowerManagerConfig new_config{};
    new_config.efuse_configs[0].efuse_enable = true;
    new_config.efuse_configs[0].max_retry    = 2;
    new_config.efuse_configs[0].timeout      = 1;

    app::powerManager::updateConfig(new_config);
    app::powerManager::efuseProtocolTick_100Hz();

    ASSERT_TRUE(app::powerManager::getConfig().efuse_configs[0].efuse_enable);
    ASSERT_EQ(2, app::powerManager::getConfig().efuse_configs[0].max_retry);
    ASSERT_EQ(1, app::powerManager::getConfig().efuse_configs[0].timeout);
    ASSERT_TRUE(rr_pump_efuse.isChannelEnabled());
}
